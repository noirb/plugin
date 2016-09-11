/*
 * AvatarControllerByPerceptionNeuronAndOpenVR
 *
 *  Created on: 2016/09/11
 */
#ifndef SIGVERSE_AVATAR_CONTROLLER_BY_PERCEPTION_NEURON_AND_OPENVR_H
#define SIGVERSE_AVATAR_CONTROLLER_BY_PERCEPTION_NEURON_AND_OPENVR_H

#include <sigverse/commonlib/Controller.h>
#include <sigverse/commonlib/ControllerEvent.h>

#include <sigverse/plugin/controller/common/AgentController.h>
#include <sigverse/plugin/controller/common/ManBvhPosture.h>
#include <sigverse/plugin/controller/common/sensor/PerceptionNeuronDeviceManager.h>
#include <sigverse/plugin/controller/common/sensor/OpenVRDeviceManager.h>
#include <sigverse/plugin/common/sensor/PerceptionNeuronSensorData.h>
#include <sigverse/plugin/common/sensor/OpenVRSensorData.h>

class AvatarControllerByPerceptionNeuronAndOpenVR : public AgentController
{
public:
    static const std::string paramFileKeyPerceptionNeuronServiceName;
    static const std::string paramFileKeyPerceptionNeuronDevicetype;
    static const std::string paramFileKeyPerceptionNeuronDeviceUniqueID;

    static const std::string paramFileKeyOpenVRServiceName;
    static const std::string paramFileKeyOpenVRDevicetype;
    static const std::string paramFileKeyOpenVRDeviceUniqueID;

    ///@brief Movement of the robot.
    double onAction(ActionEvent &evt);

    ///@brief Receive Message.
    void onRecvMsg(RecvMsgEvent &evt);

    ///@brief Initialize this controller.
    void onInit(InitEvent &evt);

    void readIniFileAndInitialize();

    bool usingHMD = false;
    Quaternion waistLinkQuaternion; // for the correction of head posture.

    PerceptionNeuronDeviceManager perceptionNeuronDeviceManager;
    OpenVRDeviceManager           openVRDeviceManager;
};

///@brief Parameter file name.
const std::string AgentController::parameterFileName = "PerceptionNeuronAndOpenVR.ini";

const std::string AvatarControllerByPerceptionNeuronAndOpenVR::paramFileKeyPerceptionNeuronServiceName    = "PerceptionNeuron.service_name";
const std::string AvatarControllerByPerceptionNeuronAndOpenVR::paramFileKeyPerceptionNeuronDevicetype     = "PerceptionNeuron.device_type";
const std::string AvatarControllerByPerceptionNeuronAndOpenVR::paramFileKeyPerceptionNeuronDeviceUniqueID = "PerceptionNeuron.device_unique_id";

const std::string AvatarControllerByPerceptionNeuronAndOpenVR::paramFileKeyOpenVRServiceName              = "OpenVR.service_name";
const std::string AvatarControllerByPerceptionNeuronAndOpenVR::paramFileKeyOpenVRDevicetype               = "OpenVR.device_type";
const std::string AvatarControllerByPerceptionNeuronAndOpenVR::paramFileKeyOpenVRDeviceUniqueID           = "OpenVR.device_unique_id";


#endif //SIGVERSE_AVATAR_CONTROLLER_BY_PERCEPTION_NEURON_AND_OPENVR_H
