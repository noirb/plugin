/*
 * AvatarControllerByOptiTrack
 *
 *  Created on: 2015/03/12
 *      Author: Nozaki
 */
#ifndef SIGVERSE_AVATAR_CONTROLLER_BY_OPTITRACK_H
#define SIGVERSE_AVATAR_CONTROLLER_BY_OPTITRACK_H

#include <sigverse/commonlib/Controller.h>
#include <sigverse/commonlib/ControllerEvent.h>

#include <sigverse/plugin/controller/common/sensor/OptiTrackDeviceManager.h>
#include <sigverse/plugin/controller/common/ManNiiAvatarController.h>
#include <sigverse/plugin/controller/common/ManNiiPosture.h>
#include <sigverse/plugin/common/sensor/OptiTrackSensorData.h>


class AvatarControllerByOptiTrack : public ManNiiAvatarController
{
private:
	void setRigidBody2ManNiiJoint(ManNiiPosture::ManNiiJoint &manNiiJoint, const OptiTrackSensorData::sRigidBodyDataSgv &rigidBodySgv);

public:

	///@brief Movement of the robot.
	double onAction(ActionEvent &evt);

	///@brief Message heard by the robot.
	void onRecvMsg(RecvMsgEvent &evt);

	///@brief Initialize this controller.
	void onInit(InitEvent &evt);

	OptiTrackDeviceManager optiTrackDeviceManager;

	ManNiiPosture posture;
};


///@brief Parameter file name.
const std::string AgentController::parameterFileName = "OptiTrack.ini";

#endif // SIGVERSE_AVATAR_CONTROLLER_BY_OPTITRACK_H
