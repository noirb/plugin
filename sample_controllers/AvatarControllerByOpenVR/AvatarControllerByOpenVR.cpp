/*
 * AvatarControllerByOpenVR
 *
 *  Created on: 2016/09/11
  */

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <sigverse/plugin/common/sensor/OpenVRSensorData.h>
#include <cmath>
#include "AvatarControllerByOpenVR.h"

///@brief Initialize this controller.
void AvatarControllerByOpenVR::onInit(InitEvent &evt)
{
    readIniFileAndInitialize();


    this->openVRDeviceManager = OpenVRDeviceManager(this->serviceName, this->deviceType, this->deviceUniqueID);

    //For initialize.
    getObj(myname());
}


///@brief Movement of the robot.
double AvatarControllerByOpenVR::onAction(ActionEvent &evt)
{
    bool openVRAvailable = checkService(this->openVRDeviceManager.serviceName);

    if (openVRAvailable && this->openVRDeviceManager.service == NULL)
    {
        this->openVRDeviceManager.service = connectToService(this->openVRDeviceManager.serviceName);
    }
    else if (!openVRAvailable && this->openVRDeviceManager.service != NULL)
    {
        this->openVRDeviceManager.service = NULL;
    }

    return 1.0;
}

void AvatarControllerByOpenVR::onRecvMsg(RecvMsgEvent &evt)
{
    const std::string allMsg = evt.getMsg();

    //std::cout << allMsg << std::endl;

    std::map<std::string, std::vector<std::string> > sensorDataMap = OpenVRSensorData::decodeSensorData(allMsg);

    if (sensorDataMap.find(MSG_KEY_DEV_TYPE) == sensorDataMap.end()){ return; }

    if(sensorDataMap[MSG_KEY_DEV_TYPE][0]     !=this->openVRDeviceManager.deviceType    ){ return; }
    if(sensorDataMap[MSG_KEY_DEV_UNIQUE_ID][0]!=this->openVRDeviceManager.deviceUniqueID){ return; }

    OpenVRSensorData sensorData;
    sensorData.setSensorData(sensorDataMap);

    ManNiiPosture posture = OpenVRDeviceManager::convertQuaternion2ManNiiPosture(sensorData.getQuaternion(OpenVRSensorData::DeviceType::HMD));

    SimObj *obj = getObj(myname());
    OpenVRDeviceManager::setJointQuaternions2ManNii(obj, posture);
}


extern "C" Controller * createController()
{
    return new AvatarControllerByOpenVR;
}


