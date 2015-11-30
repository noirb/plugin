/*
 * ManNiiAvatarController.h
 *
 *  Created on: 2015/03/12
 *      Author: Nozaki
 */
#ifndef SIGVERSE_MAN_NII_AVATAR_CONTROLLER_BY_KINECT_V2_H
#define SIGVERSE_MAN_NII_AVATAR_CONTROLLER_BY_KINECT_V2_H

#include <sigverse/commonlib/Controller.h>
#include <sigverse/commonlib/ControllerEvent.h>

#include <sigverse/plugin/controller/common/ManNiiAvatarController.h>
#include <sigverse/plugin/controller/common/ManNiiPosture.h>
#include <sigverse/plugin/controller/common/sensor/KinectV2DeviceManager.h>
#include <sigverse/plugin/common/sensor/KinectV2SensorData.h>


class ManNiiAvatarControllerByKinectV2 : public ManNiiAvatarController
{
public:
	///@brief Parameter file name.
	static const std::string parameterFileName;

	//Parameter file information
	static const std::string paramFileKeyKinectV2SensorDataMode;
	static const std::string paramFileKeyKinectV2ScaleRatio;

	///@brief Movement of the robot.
	double onAction(ActionEvent &evt);

	///@brief Message heard by the robot.
	void onRecvMsg(RecvMsgEvent &evt);

	///@brief Initialize this controller.
	void onInit(InitEvent &evt);

	void readIniFileAndInitialize();

	KinectV2DeviceManager kinectV2DeviceManager;
};

///@brief Parameter file name.
const std::string ManNiiAvatarControllerByKinectV2::parameterFileName = "KinectV2.ini";

//Parameter file information
const std::string ManNiiAvatarControllerByKinectV2::paramFileKeyKinectV2SensorDataMode = "KinectV2.sensor_data_mode";
const std::string ManNiiAvatarControllerByKinectV2::paramFileKeyKinectV2ScaleRatio     = "KinectV2.scale_ratio";

#endif //__MAN_NII_AVATAR_CONTROLLER_BY_KINECT_V2_H
