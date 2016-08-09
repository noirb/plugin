/*
 * AvatarControllerByOculusCV1
 *
 *  Created on: 2016/08/09
  */

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <sigverse/plugin/common/sensor/OculusRiftCV1SensorData.h>
#include <cmath>
#include "AvatarControllerByOculusCV1.h"

///@brief Initialize this controller.
void AvatarControllerByOculusCV1::onInit(InitEvent &evt)
{
	readIniFileAndInitialize();
	

	this->oculusCV1DeviceManager = OculusCV1DeviceManager(this->serviceName, this->deviceType, this->deviceUniqueID);

	//For initialize.
	getObj(myname());
}


///@brief Movement of the robot.
double AvatarControllerByOculusCV1::onAction(ActionEvent &evt)
{
	bool oculusCV1Available = checkService(this->oculusCV1DeviceManager.serviceName);

	if (oculusCV1Available && this->oculusCV1DeviceManager.service == NULL)
	{
		this->oculusCV1DeviceManager.service = connectToService(this->oculusCV1DeviceManager.serviceName);
	}
	else if (!oculusCV1Available && this->oculusCV1DeviceManager.service != NULL)
	{
		this->oculusCV1DeviceManager.service = NULL;
	}

	return 1.0;
}

void AvatarControllerByOculusCV1::onRecvMsg(RecvMsgEvent &evt)
{
	const std::string allMsg = evt.getMsg();

	//std::cout << allMsg << std::endl;

	std::map<std::string, std::vector<std::string> > sensorDataMap = OculusRiftCV1SensorData::decodeSensorData(allMsg);

	if (sensorDataMap.find(MSG_KEY_DEV_TYPE) == sensorDataMap.end()){ return; }

	if(sensorDataMap[MSG_KEY_DEV_TYPE][0]     !=this->oculusCV1DeviceManager.deviceType    ){ return; }
	if(sensorDataMap[MSG_KEY_DEV_UNIQUE_ID][0]!=this->oculusCV1DeviceManager.deviceUniqueID){ return; }

	OculusRiftCV1SensorData sensorData;
	sensorData.setSensorData(sensorDataMap);

	ManNiiPosture posture = OculusCV1DeviceManager::convertQuaternion2ManNiiPosture(sensorData.getQuaternion());

	SimObj *obj = getObj(myname());
	OculusCV1DeviceManager::setJointQuaternions2ManNii(obj, posture);
}


extern "C" Controller * createController()
{
	return new AvatarControllerByOculusCV1;
}


