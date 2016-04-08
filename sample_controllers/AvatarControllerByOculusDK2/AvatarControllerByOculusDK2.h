/*
 * AvatarControllerByOculusDK2
 *
 *  Created on: 2015/07/30
 *      Author: Wada
 */
#ifndef SIGVERSE_AVATAR_CONTROLLER_BY_OCULUS_DK2_H
#define SIGVERSE_AVATAR_CONTROLLER_BY_OCULUS_DK2_H

#include <sigverse/commonlib/Controller.h>
#include <sigverse/commonlib/ControllerEvent.h>

#include <sigverse/plugin/controller/common/sensor/OculusDK2DeviceManager.h>
#include <sigverse/plugin/controller/common/ManNiiPosture.h>
#include <sigverse/plugin/controller/common/ManNiiAvatarController.h>
#include <sigverse/plugin/common/sensor/OculusRiftDK2SensorData.h>


class AvatarControllerByOculusDK2 : public ManNiiAvatarController
{
public:
	///@brief Movement of the robot.
	double onAction(ActionEvent &evt);

	///@brief Message heard by the robot.
	void onRecvMsg(RecvMsgEvent &evt);

	///@brief Initialize this controller.
	void onInit(InitEvent &evt);

	OculusDK2DeviceManager oculusDK2DeviceManager;

	ManNiiPosture posture;
};

///@brief Parameter file name.
const std::string AgentController::parameterFileName = "OculusRiftDK2.ini";

#endif // SIGVERSE_AVATAR_CONTROLLER_BY_OCULUS_DK2_H
