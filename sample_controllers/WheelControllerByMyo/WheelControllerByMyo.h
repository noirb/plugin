/*
 * WheelControllerByMyo
 *
 *  Created on: 2016/04/08
 *      Author: Yamada
 */
#ifndef SIGVERSE_WHEEL_CONTROLLER_BY_MYO_H
#define SIGVERSE_WHEEL_CONTROLLER_BY_MYO_H

#include <sigverse/commonlib/Controller.h>
#include <sigverse/commonlib/ControllerEvent.h>

#include <sigverse/plugin/controller/common/AgentController.h>
#include <sigverse/plugin/controller/common/sensor/MyoDeviceManager.h>
#include <sigverse/plugin/common/sensor/MyoSensorData.h>


class WheelControllerByMyo : public AgentController
{
public:
	///@brief Movement of the robot.
	double onAction(ActionEvent &evt);

	///@brief Message heard by the robot.
	void onRecvMsg(RecvMsgEvent &evt);

	///@brief Initialize this controller.
	void onInit(InitEvent &evt);

	MyoDeviceManager myoDeviceManager;
};

///@brief Parameter file name.
const std::string AgentController::parameterFileName = "Myo.ini";

#endif // SIGVERSE_WHEEL_CONTROLLER_BY_MYO_H
