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

	this->perceptionNeuronDeviceManager = PerceptionNeuronDeviceManager(this->serviceName, this->deviceType, this->deviceUniqueID);

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


extern "C" Controller * createController()
{
	return new AvatarControllerByPerceptionNeuron;
}

