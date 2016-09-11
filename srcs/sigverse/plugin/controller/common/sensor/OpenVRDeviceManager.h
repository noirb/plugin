/*
 * OpenVRDeviceManager.h
 *
 *  Created on: 2016/09/10
  */
#ifndef SIGVERSE_OPENVR_DEVICE_MANAGER_H
#define SIGVERSE_OPENVR_DEVICE_MANAGER_H

#include <sigverse/commonlib/SimObj.h>
#include <sigverse/commonlib/ControllerImpl.h>

#include <sigverse/plugin/controller/common/ManNiiPosture.h>
#include <sigverse/plugin/controller/common/ManBvhPosture.h>
#include <sigverse/plugin/common/sensor/SensorData.h>

class OpenVRDeviceManager
{
public:
	static const double normalization_range;

	static const dQuaternion defaultHeadJoint0Quaternion;

	BaseService *service;

	std::string serviceName;
	std::string deviceType;
	std::string deviceUniqueID;

	OpenVRDeviceManager();
	OpenVRDeviceManager(std::string &name, std::string &deviceType, std::string &deviceUniqueID);

	///@brief Convert euler angle to avatar posture structure.
	static ManNiiPosture convertQuaternion2ManNiiPosture(const SigCmn::Vector4 &quaternion);

	static void setJointQuaternions2ManNii(SimObj *obj, const ManNiiPosture &posture);
	static void setJointQuaternion(SimObj *obj, const std::string &jointName, const Quaternion &quaternion);
};

#endif //SIGVERSE_OPENVR_DEVICE_MANAGER_H
