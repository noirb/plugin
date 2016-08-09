/*
 * AvatarControllerByOculusCV1
 *
 *  Created on: 2016/08/09
  */
#ifndef SIGVERSE_AVATAR_CONTROLLER_BY_OCULUS_CV1_H
#define SIGVERSE_AVATAR_CONTROLLER_BY_OCULUS_CV1_H

#include <sigverse/commonlib/Controller.h>
#include <sigverse/commonlib/ControllerEvent.h>

#include <sigverse/plugin/controller/common/sensor/OculusCV1DeviceManager.h>
#include <sigverse/plugin/controller/common/ManNiiPosture.h>
#include <sigverse/plugin/controller/common/ManNiiAvatarController.h>
#include <sigverse/plugin/common/sensor/OculusRiftCV1SensorData.h>


class AvatarControllerByOculusCV1 : public ManNiiAvatarController
{
public:
	///@brief Movement of the robot.
	double onAction(ActionEvent &evt);

	///@brief Message heard by the robot.
	void onRecvMsg(RecvMsgEvent &evt);

	///@brief Initialize this controller.
	void onInit(InitEvent &evt);

	OculusCV1DeviceManager oculusCV1DeviceManager;

	ManNiiPosture posture;
};

///@brief Parameter file name.
const std::string AgentController::parameterFileName = "OculusRiftCV1.ini";

#endif // SIGVERSE_AVATAR_CONTROLLER_BY_OCULUS_CV1_H
