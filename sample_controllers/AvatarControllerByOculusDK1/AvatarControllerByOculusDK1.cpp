/*
 * AvatarControllerByOculusDK1
 *
 *  Created on: 2015/03/12
 *      Author: Nozaki
 */

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <sigverse/plugin/common/sensor/OculusRiftDK1SensorData.h>
#include <cmath>
#include "AvatarControllerByOculusDK1.h"

///@brief Initialize this controller.
void AvatarControllerByOculusDK1::onInit(InitEvent &evt)
{
	readIniFileAndInitialize();

	this->oculusDK1DeviceManager = OculusDK1DeviceManager(this->serviceName, this->deviceType, this->deviceUniqueID);

	//For initialize.
	getObj(myname());
}


///@brief Movement of the robot.
double AvatarControllerByOculusDK1::onAction(ActionEvent &evt)
{
	bool oculusDK1Available = checkService(this->oculusDK1DeviceManager.serviceName);

	if (oculusDK1Available && this->oculusDK1DeviceManager.service == NULL)
	{
		this->oculusDK1DeviceManager.service = connectToService(this->oculusDK1DeviceManager.serviceName);
	}
	else if (!oculusDK1Available && this->oculusDK1DeviceManager.service != NULL)
	{
		this->oculusDK1DeviceManager.service = NULL;
	}

	return 1.0;
}

void AvatarControllerByOculusDK1::onRecvMsg(RecvMsgEvent &evt)
{
	const std::string allMsg = evt.getMsg();

	//std::cout << allMsg << std::endl;

	std::map<std::string, std::vector<std::string> > sensorDataMap = OculusRiftDK1SensorData::decodeSensorData(allMsg);

	if (sensorDataMap.find(MSG_KEY_DEV_TYPE) == sensorDataMap.end()){ return; }

	if(sensorDataMap[MSG_KEY_DEV_TYPE][0]     !=this->oculusDK1DeviceManager.deviceType    ){ return; }
	if(sensorDataMap[MSG_KEY_DEV_UNIQUE_ID][0]!=this->oculusDK1DeviceManager.deviceUniqueID){ return; }

	OculusRiftDK1SensorData sensorData;
	sensorData.setSensorData(sensorDataMap);

	ManNiiPosture posture = OculusDK1DeviceManager::convertEulerAngle2ManNiiPosture(sensorData.getEulerAngle());

	SimObj *obj = getObj(myname());
	OculusDK1DeviceManager::setJointQuaternions2ManNii(obj, posture);
}


extern "C" Controller * createController()
{
	return new AvatarControllerByOculusDK1;
}


