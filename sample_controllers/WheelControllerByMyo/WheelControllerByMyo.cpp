/*
 * WheelControllerByMyo
 *
 *  Created on: 2016/04/08
 *	  Author: Yamada
 */

#include "WheelControllerByMyo.h"

#include <sigverse/plugin/common/sensor/MyoSensorData.h>

///@brief Initialize this controller.
void WheelControllerByMyo::onInit(InitEvent &evt)
{
	this->readIniFileAndInitialize();

	this->myoDeviceManager = MyoDeviceManager(this->serviceName, this->deviceType, this->deviceUniqueID);

	//For initialize.
	getObj(myname());
}


///@brief Movement of the robot.
double WheelControllerByMyo::onAction(ActionEvent &evt)
{
	bool myoAvailable = checkService(this->myoDeviceManager.serviceName);

	if (myoAvailable && this->myoDeviceManager.service == NULL)
	{
		this->myoDeviceManager.service = connectToService(this->myoDeviceManager.serviceName);
	}
	else if (!myoAvailable && this->myoDeviceManager.service != NULL)
	{
		this->myoDeviceManager.service = NULL;
	}

	return 1.0;
}

void WheelControllerByMyo::onRecvMsg(RecvMsgEvent &evt)
{
	const std::string allMsg = evt.getMsg();

	//std::cout << allMsg << std::endl;

	std::map<std::string, std::vector<std::string> > sensorDataMap = MyoSensorData::decodeSensorData(allMsg);

	if (sensorDataMap.find(MSG_KEY_DEV_TYPE) == sensorDataMap.end()){ return; }

	if(sensorDataMap[MSG_KEY_DEV_TYPE][0]	 !=this->myoDeviceManager.deviceType	){ return; }
	if(sensorDataMap[MSG_KEY_DEV_UNIQUE_ID][0]!=this->myoDeviceManager.deviceUniqueID){ return; }

	MyoSensorData sensorData;
	sensorData.setSensorData(sensorDataMap);


	SimObj *obj = getObj(myname());

	float vel = 0.0;

	for(int i=0; i<MyoSensorData::EMG_SENSOR_NUM; i++)
	{
		vel += std::abs(sensorData.emgData[i]);
	}

	double lvel = +vel;
	double rvel = -vel;

	if(sensorData.poseStr==MyoSensorData::mapPoseType2Str.at(MyoSensorData::Fist))
	{
		lvel *= -1.0;
		rvel *= -1.0;
	}

	obj->setJointVelocity("JOINT_LWHEEL", lvel, 500.0);
	obj->setJointVelocity("JOINT_RWHEEL", rvel, 500.0);
}


extern "C" Controller * createController()
{
	return new WheelControllerByMyo;
}


