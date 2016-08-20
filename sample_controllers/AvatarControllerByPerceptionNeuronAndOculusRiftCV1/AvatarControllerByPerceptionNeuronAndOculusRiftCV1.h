/*
 * AvatarControllerByPerceptionNeuronAndOculusRiftCV1
 *
 *  Created on: 2016/08/20
 */
#ifndef SIGVERSE_AVATAR_CONTROLLER_BY_PERCEPTION_NEURON_AND_OCULUS_RIFT_CV1_H
#define SIGVERSE_AVATAR_CONTROLLER_BY_PERCEPTION_NEURON_AND_OCULUS_RIFT_CV1_H

#include <sigverse/commonlib/Controller.h>
#include <sigverse/commonlib/ControllerEvent.h>

#include <sigverse/plugin/controller/common/AgentController.h>
#include <sigverse/plugin/controller/common/ManBvhPosture.h>
#include <sigverse/plugin/controller/common/sensor/PerceptionNeuronDeviceManager.h>
#include <sigverse/plugin/controller/common/sensor/OculusCV1DeviceManager.h>
#include <sigverse/plugin/common/sensor/PerceptionNeuronSensorData.h>
#include <sigverse/plugin/common/sensor/OculusRiftCV1SensorData.h>

class AvatarControllerByPerceptionNeuronAndOculusRiftCV1 : public AgentController
{
public:
	static const std::string paramFileKeyPerceptionNeuronServiceName;
	static const std::string paramFileKeyPerceptionNeuronDevicetype;
	static const std::string paramFileKeyPerceptionNeuronDeviceUniqueID;

	static const std::string paramFileKeyOculusCV1ServiceName;
	static const std::string paramFileKeyOculusCV1Devicetype;
	static const std::string paramFileKeyOculusCV1DeviceUniqueID;

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
	OculusCV1DeviceManager oculusCV1DeviceManager;
};

///@brief Parameter file name.
const std::string AgentController::parameterFileName = "PerceptionNeuronAndOculusRiftCV1.ini";

const std::string AvatarControllerByPerceptionNeuronAndOculusRiftCV1::paramFileKeyPerceptionNeuronServiceName   = "PerceptionNeuron.service_name";
const std::string AvatarControllerByPerceptionNeuronAndOculusRiftCV1::paramFileKeyPerceptionNeuronDevicetype    = "PerceptionNeuron.device_type";
const std::string AvatarControllerByPerceptionNeuronAndOculusRiftCV1::paramFileKeyPerceptionNeuronDeviceUniqueID= "PerceptionNeuron.device_unique_id";

const std::string AvatarControllerByPerceptionNeuronAndOculusRiftCV1::paramFileKeyOculusCV1ServiceName   = "OculusCV1.service_name";
const std::string AvatarControllerByPerceptionNeuronAndOculusRiftCV1::paramFileKeyOculusCV1Devicetype    = "OculusCv1.device_type";
const std::string AvatarControllerByPerceptionNeuronAndOculusRiftCV1::paramFileKeyOculusCV1DeviceUniqueID= "OculusCV1.device_unique_id";


#endif //SIGVERSE_AVATAR_CONTROLLER_BY_PERCEPTION_NEURON_AND_OCULUS_RIFT_CV1_H
