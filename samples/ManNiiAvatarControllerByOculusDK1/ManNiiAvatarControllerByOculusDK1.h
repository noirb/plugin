/*
 * ManNiiAvatarControllerByOculusDK1.h
 *
 *  Created on: 2015/03/12
 *      Author: Nozaki
 */
#ifndef SIGVERSE_MAN_NII_AVATAR_CONTROLLER_BY_OCULUS_DK1_H
#define SIGVERSE_MAN_NII_AVATAR_CONTROLLER_BY_OCULUS_DK1_H

#include <sigverse/commonlib/Controller.h>
#include <sigverse/commonlib/ControllerEvent.h>

#include <sigverse/controller/common/device/OculusDK1DeviceManager.h>
#include <sigverse/controller/common/ManNiiPosture.h>
#include <sigverse/controller/common/ManNiiAvatarController.h>
#include <sigverse/devicecommon/device/OculusRiftDK1SensorData.h>


class ManNiiAvatarControllerByOculusDK1 : public ManNiiAvatarController
{
public:
	///@brief Parameter file name.
	static const std::string parameterFileName;

	///@brief Movement of the robot.
	double onAction(ActionEvent &evt);

	///@brief Message heard by the robot.
	void onRecvMsg(RecvMsgEvent &evt);

	///@brief Initialize this controller.
	void onInit(InitEvent &evt);

	void readIniFileAndInitialize();

	OculusDK1DeviceManager oculusDK1DeviceManager;

	ManNiiPosture posture;
};

///@brief Parameter file name.
const std::string ManNiiAvatarControllerByOculusDK1::parameterFileName = "OculusRiftDK1.ini";

#endif // SIGVERSE_MAN_NII_AVATAR_CONTROLLER_BY_OCULUS_DK1_H
