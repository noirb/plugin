/*
 * AvatarControllerByPerceptionNeuronAndOculusRiftDK2
 *
 *  Created on: 2016/05/31
 *      Author: Yamada
 */
#ifndef SIGVERSE_AVATAR_CONTROLLER_BY_PERCEPTION_NEURON_AND_OCULUS_RIFT_DK2_H
#define SIGVERSE_AVATAR_CONTROLLER_BY_PERCEPTION_NEURON_AND_OCULUS_RIFT_DK2_H

#include <sigverse/commonlib/Controller.h>
#include <sigverse/commonlib/ControllerEvent.h>

#include <sigverse/plugin/controller/common/AgentController.h>
#include <sigverse/plugin/controller/common/ManBvhPosture.h>
#include <sigverse/plugin/controller/common/sensor/PerceptionNeuronDeviceManager.h>
#include <sigverse/plugin/controller/common/sensor/OculusDK2DeviceManager.h>
#include <sigverse/plugin/common/sensor/PerceptionNeuronSensorData.h>
#include <sigverse/plugin/common/sensor/OculusRiftDK2SensorData.h>

class AvatarControllerByPerceptionNeuronAndOculusRiftDK2 : public AgentController
{
public:
	static const std::string paramFileKeyPerceptionNeuronServiceName;
	static const std::string paramFileKeyPerceptionNeuronDevicetype;
	static const std::string paramFileKeyPerceptionNeuronDeviceUniqueID;

	static const std::string paramFileKeyOculusDK2ServiceName;
	static const std::string paramFileKeyOculusDK2Devicetype;
	static const std::string paramFileKeyOculusDK2DeviceUniqueID;

	///@brief Movement of the robot.
	double onAction(ActionEvent &evt);

	///@brief Receive Message.
	void onRecvMsg(RecvMsgEvent &evt);

	///@brief Initialize this controller.
	void onInit(InitEvent &evt);

	void readIniFileAndInitialize();

	bool usingOculus = false;
	Quaternion waistLinkQuaternion; // for the correction of head posture.

	PerceptionNeuronDeviceManager perceptionNeuronDeviceManager;
	OculusDK2DeviceManager oculusDK2DeviceManager;
};

///@brief Parameter file name.
const std::string AgentController::parameterFileName = "PerceptionNeuronAndOculusRiftDK2.ini";

const std::string AvatarControllerByPerceptionNeuronAndOculusRiftDK2::paramFileKeyPerceptionNeuronServiceName   = "PerceptionNeuron.service_name";
const std::string AvatarControllerByPerceptionNeuronAndOculusRiftDK2::paramFileKeyPerceptionNeuronDevicetype    = "PerceptionNeuron.device_type";
const std::string AvatarControllerByPerceptionNeuronAndOculusRiftDK2::paramFileKeyPerceptionNeuronDeviceUniqueID= "PerceptionNeuron.device_unique_id";

const std::string AvatarControllerByPerceptionNeuronAndOculusRiftDK2::paramFileKeyOculusDK2ServiceName   = "OculusDK2.service_name";
const std::string AvatarControllerByPerceptionNeuronAndOculusRiftDK2::paramFileKeyOculusDK2Devicetype    = "OculusDK2.device_type";
const std::string AvatarControllerByPerceptionNeuronAndOculusRiftDK2::paramFileKeyOculusDK2DeviceUniqueID= "OculusDK2.device_unique_id";


#endif //SIGVERSE_AVATAR_CONTROLLER_BY_PERCEPTION_NEURON_AND_OCULUS_RIFT_DK2_H
