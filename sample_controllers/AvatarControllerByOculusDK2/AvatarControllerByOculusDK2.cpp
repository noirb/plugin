/*
 * AvatarControllerByOculusDK2
 *
 *  Created on: 2015/07/30
 *      Author: Wada
 */

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <sigverse/plugin/common/sensor/OculusRiftDK2SensorData.h>
#include <cmath>
#include "AvatarControllerByOculusDK2.h"

///@brief Initialize this controller.
void AvatarControllerByOculusDK2::onInit(InitEvent &evt)
{
	readIniFileAndInitialize();
	

	this->oculusDK2DeviceManager = OculusDK2DeviceManager(this->serviceName, this->deviceType, this->deviceUniqueID);

	//For initialize.
	getObj(myname());
}


///@brief Movement of the robot.
double AvatarControllerByOculusDK2::onAction(ActionEvent &evt)
{
	bool oculusDK2Available = checkService(this->oculusDK2DeviceManager.serviceName);

	if (oculusDK2Available && this->oculusDK2DeviceManager.service == NULL)
	{
		this->oculusDK2DeviceManager.service = connectToService(this->oculusDK2DeviceManager.serviceName);
	}
	else if (!oculusDK2Available && this->oculusDK2DeviceManager.service != NULL)
	{
		this->oculusDK2DeviceManager.service = NULL;
	}

	return 1.0;
}

void AvatarControllerByOculusDK2::onRecvMsg(RecvMsgEvent &evt)
{
	const std::string allMsg = evt.getMsg();

	//std::cout << allMsg << std::endl;

	std::map<std::string, std::vector<std::string> > sensorDataMap = OculusRiftDK2SensorData::decodeSensorData(allMsg);

	if (sensorDataMap.find(MSG_KEY_DEV_TYPE) == sensorDataMap.end()){ return; }

	if(sensorDataMap[MSG_KEY_DEV_TYPE][0]     !=this->oculusDK2DeviceManager.deviceType    ){ return; }
	if(sensorDataMap[MSG_KEY_DEV_UNIQUE_ID][0]!=this->oculusDK2DeviceManager.deviceUniqueID){ return; }

	OculusRiftDK2SensorData sensorData;
	sensorData.setSensorData(sensorDataMap);

	ManNiiPosture posture = OculusDK2DeviceManager::convertQuaternion2ManNiiPosture(sensorData.getQuaternion());

	SimObj *obj = getObj(myname());
	OculusDK2DeviceManager::setJointQuaternions2ManNii(obj, posture);
}


extern "C" Controller * createController()
{
	return new AvatarControllerByOculusDK2;
}


