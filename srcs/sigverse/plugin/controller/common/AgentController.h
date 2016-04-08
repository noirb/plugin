/*
 * AgentController.h
 *
 *  Created on: 2015/03/12
 *      Author: Nozaki
 */

#ifndef SIGVERSE_AGENT_CONTROLLER_H
#define SIGVERSE_AGENT_CONTROLLER_H

#include <sigverse/commonlib/Controller.h>

#include <sigverse/plugin/controller/common/Posture.h>

class AgentController : public Controller
{
public:
	///@brief Parameter file name.
	static const std::string parameterFileName;

	std::string serviceName;
	std::string deviceType;
	std::string deviceUniqueID;

protected:
	void readIniFileAndInitialize();
};

#endif // SIGVERSE_AGENT_CONTROLLER_H
