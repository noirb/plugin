/*
 * PerceptionNeuronDeviceManager.h
 *
 *  Created on: 2015/07/09
 *      Author: tome-yamada
 */
#ifndef SIGVERSE_KINECT_V2_DEVICE_MANAGER_H
#define SIGVERSE_KINECT_V2_DEVICE_MANAGER_H

#include <sigverse/commonlib/SimObj.h>
#include <sigverse/commonlib/ControllerImpl.h>

#include <sigverse/plugin/controller/common/ManNiiPosture.h>
#include <sigverse/plugin/common/sensor/PerceptionNeuronSensorData.h>
#include <sigverse/plugin/common/sensor/SensorData.h>

class PerceptionNeuronDeviceManager
{
public:
	static const double normalization_range;

	PerceptionNeuronDeviceManager();
	PerceptionNeuronDeviceManager(std::string &name, std::string &deviceType, std::string &deviceUniqueID, double scaleRatio);

	void initPositionAndRotation(SimObj *myself);

	static void correctSlope(PerceptionNeuronSensorData::PerceptionNeuronJointPosition* jointPositions, const double slopeRadX);

	static ManNiiPosture convertSensorData2ManNiiPosture(const PerceptionNeuronSensorData &sensorData);
	///@brief Convert Kinect V2 joint orientation to avatar posture structure.
	static ManNiiPosture convertPerceptionNeuronJointOrientations2ManNiiPosture(const PerceptionNeuronSensorData::PerceptionNeuronJointOrientation* PerceptionNeuronJoints);
	///@brief Convert Kinect V2 joint position to avatar posture structure.
	static ManNiiPosture convertPerceptionNeuronJointPosition2ManNiiPosture(const PerceptionNeuronSensorData::PerceptionNeuronJointPosition* positionArray);

	static void setJointQuaternion2ManNii(SimObj *obj, const ManNiiPosture::ManNiiJoint &joint);
	static void setJointQuaternions2ManNii(SimObj *obj, const ManNiiPosture &manNiiPosture, const PerceptionNeuronSensorData &sensorData);

	void setRootPosition(SimObj *obj, const SigCmn::Vector3 &pos);


	BaseService *service;
	std::string serviceName;
	std::string deviceType;
	std::string deviceUniqueID;

	SigCmn::Vector3 rootPos;

	double scaleRatio;

	SigCmn::Vector3 iniPos;
	double yrot;

	SigCmn::Vector3 startpos;
	bool started;
};

#endif //SIGVERSE_KINECT_V2_DEVICE_MANAGER_H
