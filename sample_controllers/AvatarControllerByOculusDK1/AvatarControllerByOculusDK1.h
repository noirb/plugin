/*
 * AvatarControllerByOculusDK1
 *
 *  Created on: 2015/03/12
 *      Author: Nozaki
 */
#ifndef SIGVERSE_AVATAR_CONTROLLER_BY_OCULUS_DK1_H
#define SIGVERSE_AVATAR_CONTROLLER_BY_OCULUS_DK1_H

#include <sigverse/commonlib/Controller.h>
#include <sigverse/commonlib/ControllerEvent.h>

#include <sigverse/plugin/controller/common/sensor/OculusDK1DeviceManager.h>
#include <sigverse/plugin/controller/common/ManNiiPosture.h>
#include <sigverse/plugin/controller/common/ManNiiAvatarController.h>
#include <sigverse/plugin/common/sensor/OculusRiftDK1SensorData.h>


class AvatarControllerByOculusDK1 : public ManNiiAvatarController
{
public:
	///@brief Movement of the robot.
	double onAction(ActionEvent &evt);

	///@brief Message heard by the robot.
	void onRecvMsg(RecvMsgEvent &evt);

	///@brief Initialize this controller.
	void onInit(InitEvent &evt);

	OculusDK1DeviceManager oculusDK1DeviceManager;

	ManNiiPosture posture;
};

///@brief Parameter file name.
const std::string AgentController::parameterFileName = "OculusRiftDK1.ini";

#endif // SIGVERSE_AVATAR_CONTROLLER_BY_OCULUS_DK1_H
