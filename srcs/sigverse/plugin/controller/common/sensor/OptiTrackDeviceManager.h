/*
 * OptiTrackDeviceManager.h
 *
 *  Created on: 2015/07/10
 *      Author: tome-yamada
 */
#ifndef SIGVERSE_OPTITRACK_DEVICE_MANAGER_H
#define SIGVERSE_OPTITRACK_DEVICE_MANAGER_H

#include <sigverse/commonlib/SimObj.h>
#include <sigverse/commonlib/ControllerImpl.h>

#include <sigverse/plugin/controller/common/ManNiiPosture.h>
#include <sigverse/plugin/common/sensor/OptiTrackSensorData.h>
#include <sigverse/plugin/common/sensor/SensorData.h>

class OptiTrackDeviceManager
{
public:
	//Hand that are linked with the device.
	enum HandsType{ RIGHT_HAND, LEFT_HAND };

	OptiTrackDeviceManager();
	OptiTrackDeviceManager(std::string &name, std::string &deviceType, std::string &deviceUniqueID);

	static const std::string handsType2Str(HandsType handsType);

	static void setJointQuaternions2ManNii(SimObj *obj, const ManNiiPosture &posture);

	static void setRigidBody2ManNiiJoint(ManNiiPosture::ManNiiJoint &manNiiJoint, const OptiTrackSensorData::sRigidBodyDataSgv &rigidBodySgv);

	///@brief Convert sensor data to avatar posture structure.
	void setSensorData2ManNiiPosture(ManNiiPosture &posture, const OptiTrackSensorData &sensorData);

	bool changeHandsType(std::string msg);


	BaseService *service;

	std::string serviceName;
	std::string deviceType;
	std::string deviceUniqueID;

	HandsType handsType;
};

#endif //SIGVERSE_OPTITRACK_DEVICE_MANAGER_H
