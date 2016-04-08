/*
 * AvatarControllerByPerceptionNeuron
 *
 *  Created on: 2016/03/02
 *      Author: Yamada
 */
#ifndef SIGVERSE_AVATAR_CONTROLLER_BY_PERCEPTION_NEURON_H
#define SIGVERSE_AVATAR_CONTROLLER_BY_PERCEPTION_NEURON_H

#include <sigverse/commonlib/Controller.h>
#include <sigverse/commonlib/ControllerEvent.h>

#include <sigverse/plugin/controller/common/AgentController.h>
#include <sigverse/plugin/controller/common/ManBvhPosture.h>
#include <sigverse/plugin/controller/common/sensor/PerceptionNeuronDeviceManager.h>
#include <sigverse/plugin/common/sensor/PerceptionNeuronSensorData.h>


class AvatarControllerByPerceptionNeuron : public AgentController
{
public:
	///@brief Movement of the robot.
	double onAction(ActionEvent &evt);

	///@brief Receive Message.
	void onRecvMsg(RecvMsgEvent &evt);

	///@brief Initialize this controller.
	void onInit(InitEvent &evt);

	PerceptionNeuronDeviceManager perceptionNeuronDeviceManager;
};

///@brief Parameter file name.
const std::string AgentController::parameterFileName = "PerceptionNeuron.ini";

#endif //SIGVERSE_AVATAR_CONTROLLER_BY_PERCEPTION_NEURON_H
