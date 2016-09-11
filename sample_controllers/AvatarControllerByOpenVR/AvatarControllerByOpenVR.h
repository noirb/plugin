/*
 * AvatarControllerByOpenVR
 *
 *  Created on: 2016/09/11
  */
#ifndef SIGVERSE_AVATAR_CONTROLLER_BY_OPENVR_H
#define SIGVERSE_AVATAR_CONTROLLER_BY_OPENVR_H

#include <sigverse/commonlib/Controller.h>
#include <sigverse/commonlib/ControllerEvent.h>

#include <sigverse/plugin/controller/common/sensor/OpenVRDeviceManager.h>
#include <sigverse/plugin/controller/common/ManNiiPosture.h>
#include <sigverse/plugin/controller/common/ManNiiAvatarController.h>
#include <sigverse/plugin/common/sensor/OpenVRSensorData.h>


class AvatarControllerByOpenVR : public ManNiiAvatarController
{
public:
    ///@brief Movement of the robot.
    double onAction(ActionEvent &evt);

    ///@brief Message heard by the robot.
    void onRecvMsg(RecvMsgEvent &evt);

    ///@brief Initialize this controller.
    void onInit(InitEvent &evt);

    OpenVRDeviceManager openVRDeviceManager;

    ManNiiPosture posture;
};

///@brief Parameter file name.
const std::string AgentController::parameterFileName = "OpenVR.ini";

#endif // SIGVERSE_AVATAR_CONTROLLER_BY_OPENVR_H
