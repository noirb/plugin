/*
 * AvatarControllerByPerceptionNeuronAndOpenVR
 *
 *  Created on: 2016/09/11
 */

#include "AvatarControllerByPerceptionNeuronAndOpenVR.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <sigverse/plugin/common/sensor/SensorData.h>
#include <cmath>

///@brief Initialize this controller.
void AvatarControllerByPerceptionNeuronAndOpenVR::onInit(InitEvent &evt)
{
    this->readIniFileAndInitialize();

    SimObj *myself = getObj(myname());

    this->perceptionNeuronDeviceManager.getInitialPositionAndRotation(myself);

    std::cout << "PerceptionNeuron + OpenVR controller started" << std::endl;
}


double AvatarControllerByPerceptionNeuronAndOpenVR::onAction(ActionEvent &evt)
{
    static int loopCnt = 0;

    dReal waistQuaternionArray[4];

    // Get the absolute quaternion of waist link.
    SimObj *obj = getObj(myname());
    obj->getPartsQuaternion(waistQuaternionArray[0], waistQuaternionArray[1], waistQuaternionArray[2], waistQuaternionArray[3], "W_L1");
    this->waistLinkQuaternion.setQuaternion(waistQuaternionArray[0], waistQuaternionArray[1], waistQuaternionArray[2], waistQuaternionArray[3]);

    if(loopCnt % 100 == 0)
    {
        bool perceptionNeuronAvailable = checkService(this->perceptionNeuronDeviceManager.serviceName);

        if (perceptionNeuronAvailable && this->perceptionNeuronDeviceManager.service == NULL)
        {
            this->perceptionNeuronDeviceManager.service = connectToService(this->perceptionNeuronDeviceManager.serviceName);
        }
        else if (!perceptionNeuronAvailable && this->perceptionNeuronDeviceManager.service != NULL)
        {
            this->perceptionNeuronDeviceManager.service = NULL;
        }

        bool openVRAvailable = checkService(this->openVRDeviceManager.serviceName);

        if (openVRAvailable && this->openVRDeviceManager.service == NULL)
        {
            this->openVRDeviceManager.service = connectToService(this->openVRDeviceManager.serviceName);
        }
        else if (!openVRAvailable && this->openVRDeviceManager.service != NULL)
        {
            this->openVRDeviceManager.service = NULL;
        }
    }
    loopCnt++;

    return 0.01;
}

///@brief Receive Message.
void AvatarControllerByPerceptionNeuronAndOpenVR::onRecvMsg(RecvMsgEvent &evt)
{
    try
    {
        std::string allMsg = evt.getMsg();

    //		std::cout << "msg:" << allMsg << std::endl;

        // Decode message to sensor data of Perception Neuron.
        std::map<std::string, std::vector<std::string> > sensorDataMap = PerceptionNeuronSensorData::decodeSensorData(allMsg);

        /*
         * Device
         */
        if (sensorDataMap.find(MSG_KEY_DEV_TYPE) != sensorDataMap.end())
        {
            std::string deviceTypeValue = sensorDataMap[MSG_KEY_DEV_TYPE][0];
            std::string deviceUniqueId  = sensorDataMap[MSG_KEY_DEV_UNIQUE_ID][0];

            /*
             * Perception Neuron
             */
            if (deviceTypeValue == this->perceptionNeuronDeviceManager.deviceType && deviceUniqueId == this->perceptionNeuronDeviceManager.deviceUniqueID)
            {
                // Make sensor data.
                PerceptionNeuronSensorData sensorData;

                sensorData.setSensorData(sensorDataMap);

                ManBvhPosture posture = this->perceptionNeuronDeviceManager.convertSensorData2ManBvhPosture(sensorData);

                if (this->usingHMD)
                {
                    posture.joint[ManBvhPosture::NECK_JOINT].isValid = false;
                }

                // Set the posture to avatar.
                SimObj *obj = getObj(myname());
                this->perceptionNeuronDeviceManager.setPosture2ManBvhYXZ(obj, posture);
            }
            /*
             * OpenVR HMD
             */
            else if (deviceTypeValue == this->openVRDeviceManager.deviceType && deviceUniqueId ==  this->openVRDeviceManager.deviceUniqueID)
            {
                /*
                 * Move the head to the main body.
                 */
                OpenVRSensorData sensorData;
                sensorData.setSensorData(sensorDataMap);

                ManNiiPosture posture = OpenVRDeviceManager::convertQuaternion2ManNiiPosture(sensorData.getQuaternion(OpenVRSensorData::DeviceType::HMD));

                Quaternion headQuaternion = posture.joint[ManNiiPosture::HEAD_JOINT0].quaternion;

                // Change the absolute quaternion into the relative quaternion.
                headQuaternion = Quaternion::multiplyNI(this->waistLinkQuaternion, headQuaternion);

                SimObj *obj = getObj(myname());

                if(!this->usingHMD)
                {
                    obj->setJointAngle("NECK_JOINT_X", 0.0);
                    obj->setJointAngle("NECK_JOINT_Y", 0.0);
                    obj->setJointAngle("NECK_JOINT_Z", 0.0);

                    this->usingHMD = true;
                }

                this->openVRDeviceManager.setJointQuaternion(obj, "NECK_JOINT_Y", headQuaternion);
            }
        }
    }
    catch(SimObj::Exception &err)
    {
        LOG_MSG(("Exception: %s", err.msg()));
    }
    catch(std::string &err)
    {
        LOG_MSG(("Exception: %s", err.c_str()));
    }
    catch(...)
    {
        std::cout << "some error occurred." << std::endl;
    }
}


///@brief Read parameter file.
///@return When couldn't read parameter file, return false;
void AvatarControllerByPerceptionNeuronAndOpenVR::readIniFileAndInitialize()
{
    try
    {
        std::ifstream ifs(parameterFileName.c_str());

        std::string perceptionNeuronServiceName;
        std::string perceptionNeuronDeviceType;
        std::string perceptionNeuronDeviceUniqueID;

        std::string openVRServiceName;
        std::string openVRDeviceType;
        std::string openVRDeviceUniqueID;

        // Parameter file is "not" exists.
        if (ifs.fail())
        {
            std::cout << "Not exist : " << parameterFileName << std::endl;
            exit(-1);
        }

        // Parameter file is exists.
        std::cout << "PerceptionNeuron+OpenVR controller reading: " << parameterFileName << std::endl;
        boost::property_tree::ptree pt;
        boost::property_tree::read_ini(parameterFileName, pt);

        perceptionNeuronServiceName    = pt.get<std::string>(paramFileKeyPerceptionNeuronServiceName);
        perceptionNeuronDeviceType     = pt.get<std::string>(paramFileKeyPerceptionNeuronDevicetype);
        perceptionNeuronDeviceUniqueID = pt.get<std::string>(paramFileKeyPerceptionNeuronDeviceUniqueID);

        openVRServiceName    = pt.get<std::string>(paramFileKeyOpenVRServiceName);
        openVRDeviceType     = pt.get<std::string>(paramFileKeyOpenVRDevicetype);
        openVRDeviceUniqueID = pt.get<std::string>(paramFileKeyOpenVRDeviceUniqueID);

        std::cout << paramFileKeyPerceptionNeuronServiceName    << ":" << perceptionNeuronServiceName    << std::endl;
        std::cout << paramFileKeyPerceptionNeuronDevicetype     << ":" << perceptionNeuronDeviceType     << std::endl;
        std::cout << paramFileKeyPerceptionNeuronDeviceUniqueID << ":" << perceptionNeuronDeviceUniqueID << std::endl;

        std::cout << paramFileKeyOpenVRServiceName    << ":" << openVRServiceName    << std::endl;
        std::cout << paramFileKeyOpenVRDevicetype     << ":" << openVRDeviceType     << std::endl;
        std::cout << paramFileKeyOpenVRDeviceUniqueID << ":" << openVRDeviceUniqueID << std::endl;

        this->perceptionNeuronDeviceManager = PerceptionNeuronDeviceManager(perceptionNeuronServiceName, perceptionNeuronDeviceType, perceptionNeuronDeviceUniqueID);
        this->openVRDeviceManager        = OpenVRDeviceManager(openVRServiceName, openVRDeviceType, openVRDeviceUniqueID);
    }
    catch (boost::exception &ex)
    {
        std::cout << parameterFileName << " ERR :" << *boost::diagnostic_information_what(ex) << std::endl;
    }
}



extern "C" Controller * createController()
{
    return new AvatarControllerByPerceptionNeuronAndOpenVR;
}

