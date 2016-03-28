/*
 * AvatarControllerByPerceptionNeuron
 *
 *  Created on: 2016/03/02
 *      Author: Yamada
 */

#include "AvatarControllerByPerceptionNeuron.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <sigverse/plugin/common/sensor/SensorData.h>
#include <cmath>

///@brief Initialize this controller.
void AvatarControllerByPerceptionNeuron::onInit(InitEvent &evt)
{
	this->readIniFileAndInitialize();

	SimObj *myself = getObj(myname());

	this->perceptionNeuronDeviceManager.getInitialPositionAndRotation(myself);
}


double AvatarControllerByPerceptionNeuron::onAction(ActionEvent &evt)
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

	return 1.0;
}

///@brief Receive Message.
void AvatarControllerByPerceptionNeuron::onRecvMsg(RecvMsgEvent &evt)
{
	try
	{
		std::string allMsg = evt.getMsg();

//		std::cout << "msg:" << allMsg << std::endl;

		// Decode message to sensor data of Perception Neuron.
		std::map<std::string, std::vector<std::string> > sensorDataMap = PerceptionNeuronSensorData::decodeSensorData(allMsg);

		if (sensorDataMap.find(MSG_KEY_DEV_TYPE) == sensorDataMap.end()){ return; }
		if(sensorDataMap[MSG_KEY_DEV_TYPE][0]     !=this->perceptionNeuronDeviceManager.deviceType    ){ return; }
		if(sensorDataMap[MSG_KEY_DEV_UNIQUE_ID][0]!=this->perceptionNeuronDeviceManager.deviceUniqueID){ return; }

		// Make sensor data.
		PerceptionNeuronSensorData sensorData;

		sensorData.setSensorData(sensorDataMap);

		ManBvhPosture posture = this->perceptionNeuronDeviceManager.convertSensorData2ManBvhPosture(sensorData);

		// Set the posture to avatar.
		SimObj *obj = getObj(myname());
		this->perceptionNeuronDeviceManager.setPosture2ManBvhYXZ(obj, posture);
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
void AvatarControllerByPerceptionNeuron::readIniFileAndInitialize()
{
	try
	{
		std::ifstream ifs(parameterFileName.c_str());

		// Parameter file is "not" exists.
		if (ifs.fail())
		{
			std::cout << "Not exist : " << parameterFileName << std::endl;
			exit(-1);
		}

		// Parameter file is exists.
		std::cout << "Read " << parameterFileName << std::endl;
		boost::property_tree::ptree pt;
		boost::property_tree::read_ini(parameterFileName, pt);

		std::string serviceName    = pt.get<std::string>(PARAMETER_FILE_KEY_GENERAL_SERVICE_NAME);
		std::string deviceType     = pt.get<std::string>(PARAMETER_FILE_KEY_GENERAL_DEVICE_TYPE);
		std::string deviceUniqueID = pt.get<std::string>(PARAMETER_FILE_KEY_GENERAL_DEVICE_UNIQUE_ID);

		std::cout << PARAMETER_FILE_KEY_GENERAL_SERVICE_NAME     << ":" << serviceName    << std::endl;
		std::cout << PARAMETER_FILE_KEY_GENERAL_DEVICE_TYPE      << ":" << deviceType     << std::endl;
		std::cout << PARAMETER_FILE_KEY_GENERAL_DEVICE_UNIQUE_ID << ":" << deviceUniqueID << std::endl;

		this->perceptionNeuronDeviceManager = PerceptionNeuronDeviceManager(serviceName, deviceType, deviceUniqueID);
	}
	catch (boost::exception &ex)
	{
		std::cout << parameterFileName << " ERR :" << *boost::diagnostic_information_what(ex) << std::endl;
		exit(-1);
	}
	catch (...)
	{
		std::cout << "Some exception occurred in readIniFileAndInitialize()." << std::endl;
		exit(-1);
	}
}


extern "C" Controller * createController()
{
	return new AvatarControllerByPerceptionNeuron;
}

