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

//	std::cout << allMsg << std::endl;

	std::map<std::string, std::vector<std::string> > sensorDataMap = MyoSensorData::decodeSensorData(allMsg);

	if (sensorDataMap.find(MSG_KEY_DEV_TYPE) == sensorDataMap.end()){ return; }

	if(sensorDataMap[MSG_KEY_DEV_TYPE][0]     !=this->myoDeviceManager.deviceType    ){ return; }
	if(sensorDataMap[MSG_KEY_DEV_UNIQUE_ID][0]!=this->myoDeviceManager.deviceUniqueID){ return; }

	MyoSensorData sensorData;
	sensorData.setSensorData(sensorDataMap);

//	std::cout << "roll=" << sensorData.roll << ",pitch=" << sensorData.pitch << ",yaw=" << sensorData.yaw << ",pose=" << sensorData.poseStr << std::endl;
//	std::cout << "emg1=" << sensorData.emgData[0] << ",emg2=" << sensorData.emgData[1] << ", emg3=" << sensorData.emgData[2] << std::endl;

	SimObj *obj = getObj(myname());

	float vel = 0.0;

	for(int i=0; i<MyoSensorData::EMG_SENSOR_NUM; i++)
	{
		vel += std::abs(sensorData.emgData[i]);
	}

	double rvel=0.0, lvel=0.0;

	// Turn left.
	if(sensorData.poseStr==MyoSensorData::mapPoseType2Str.at(MyoSensorData::WaveIn))
	{
		rvel = 3.0*vel;
		lvel = 0.05*vel;
	}
	// Turn right.
	if(sensorData.poseStr==MyoSensorData::mapPoseType2Str.at(MyoSensorData::WaveOut))
	{
		rvel = 0.05*vel;
		lvel = 3.0*vel;
	}
	// Brake.
	if(sensorData.poseStr==MyoSensorData::mapPoseType2Str.at(MyoSensorData::Fist))
	{
		rvel = -3.0*vel;
		lvel = -3.0*vel;
	}

//	std::cout << "rvel=" << rvel << ",lvel=" << lvel << std::endl;

	obj->setJointVelocity("JOINT_RWHEEL", rvel, 500.0);
	obj->setJointVelocity("JOINT_LWHEEL", lvel, 500.0);
}


extern "C" Controller * createController()
{
	return new WheelControllerByMyo;
}


