/*
 * PerceptionNeuronDeviceManager.h
 *
 *  Created on: 2016/03/02
 *      Author: Yamada
 */
#ifndef SIGVERSE_PERCEPTION_NEURON_DEVICE_MANAGER_H
#define SIGVERSE_PERCEPTION_NEURON_DEVICE_MANAGER_H

#include <sigverse/commonlib/SimObj.h>
#include <sigverse/commonlib/ControllerImpl.h>

#include <sigverse/plugin/controller/common/ManBvhPosture.h>
#include <sigverse/plugin/common/sensor/PerceptionNeuronSensorData.h>
#include <sigverse/plugin/common/sensor/SensorData.h>

class PerceptionNeuronDeviceManager
{
public:
	PerceptionNeuronDeviceManager();
	PerceptionNeuronDeviceManager(std::string &name, std::string &deviceType, std::string &deviceUniqueID);

	///@brief Get initial positions and rotations.
	void getInitialPositionAndRotation(SimObj *myself);

private:
	void setPosition2ManBvh(SimObj *obj, const ManBvhPosture &manBvhPosture);
public:
	void setJointAngle2ManBvh(SimObj *obj, const ManBvhPosture &manBvhPosture, const ManBvhPosture::ManBvhJointType &manBvhJointType);
	void setPosture2ManBvh(SimObj *obj, const ManBvhPosture &manBvhPosture);

	///@brief Convert Perception Neuron joint rotation to avatar posture structure.
	ManBvhPosture convertSensorData2ManBvhPosture(const PerceptionNeuronSensorData &sensorData);

	BaseService *service;
	std::string serviceName;
	std::string deviceType;
	std::string deviceUniqueID;

	SigCmn::Vector3 iniPos;
	SigCmn::Vector3 startpos, startRot;
	bool started;
};

#endif //SIGVERSE_PERCEPTION_NEURON_DEVICE_MANAGER_H
