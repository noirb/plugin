/*
 * AvatarControllerByOptiTrack
 *
 *  Created on: 2015/03/12
 *      Author: Nozaki
 */

#include <math.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <sigverse/plugin/common/SigCmn.h>
#include "AvatarControllerByOptiTrack.h"


///@brief Initialize this controller.
void AvatarControllerByOptiTrack::onInit(InitEvent &evt)
{
	this->readIniFileAndInitialize();

	SimObj *myself = getObj(myname());

	/* Adjustment of knee angles to sit on the chair */
	myself->setJointAngle(ManNiiPosture::manNiiJointTypeStr(ManNiiPosture::RARM_JOINT0).c_str(), SigCmn::deg2rad(+90));
	myself->setJointAngle(ManNiiPosture::manNiiJointTypeStr(ManNiiPosture::LARM_JOINT0).c_str(), SigCmn::deg2rad(+90));

	/* Root of both legs */
	myself->setJointAngle(ManNiiPosture::manNiiJointTypeStr(ManNiiPosture::RLEG_JOINT2).c_str(), SigCmn::deg2rad(-90));
	myself->setJointAngle(ManNiiPosture::manNiiJointTypeStr(ManNiiPosture::LLEG_JOINT2).c_str(), SigCmn::deg2rad(-90));
	/* Both knee */
	myself->setJointAngle(ManNiiPosture::manNiiJointTypeStr(ManNiiPosture::RLEG_JOINT4).c_str(), SigCmn::deg2rad(+90));
	myself->setJointAngle(ManNiiPosture::manNiiJointTypeStr(ManNiiPosture::LLEG_JOINT4).c_str(), SigCmn::deg2rad(+90));

	this->posture = ManNiiPosture();
}


///@brief Movement of the robot.
double AvatarControllerByOptiTrack::onAction(ActionEvent &evt)
{
	bool optiTrackAvailable = checkService(this->optiTrackDeviceManager.serviceName);

	if (optiTrackAvailable && this->optiTrackDeviceManager.service == NULL)
	{
		this->optiTrackDeviceManager.service = connectToService(this->optiTrackDeviceManager.serviceName);
	}
	else if (!optiTrackAvailable && this->optiTrackDeviceManager.service != NULL)
	{
		this->optiTrackDeviceManager.service = NULL;
	}

	return 1.0;
}

void AvatarControllerByOptiTrack::onRecvMsg(RecvMsgEvent &evt)
{
	try
	{
		const std::string msg = evt.getMsg();

//		std::cout << "msg:" << msg << std::endl;

		bool handsTypeChanged = this->optiTrackDeviceManager.changeHandsType(msg);

		if(handsTypeChanged){ return; }

		OptiTrackSensorData sensorData;
		std::map<std::string, std::vector<std::string> > sensorDataMap = sensorData.decodeSensorData(msg);

		if (sensorDataMap.find(MSG_KEY_DEV_TYPE) == sensorDataMap.end()){ return; }

		if(sensorDataMap[MSG_KEY_DEV_TYPE][0]     !=this->optiTrackDeviceManager.deviceType    ){ return; }
		if(sensorDataMap[MSG_KEY_DEV_UNIQUE_ID][0]!=this->optiTrackDeviceManager.deviceUniqueID){ return; }

		sensorData.setSensorData(sensorDataMap);

		this->optiTrackDeviceManager.setSensorData2ManNiiPosture(this->posture, sensorData);

		SimObj *obj = getObj(myname());
		OptiTrackDeviceManager::setJointQuaternions2ManNii(obj, this->posture);
	}
	catch (...)
	{
		std::cout << "catch (...) in ManNiiAvatarControllerByOptiTrack::onRecvMsg" << std::endl;
	}
}


///@brief Read parameter file.
void AvatarControllerByOptiTrack::readIniFileAndInitialize()
{
	try
	{
		std::ifstream ifs(parameterFileName.c_str());

		// Parameter file is "not" exists.
		if (ifs.fail())
		{
			std::cout << "Not exist : " << parameterFileName << std::endl;
			exit(-1);
		}

		// Parameter file is exists.
		std::cout << "Read " << parameterFileName << std::endl;
		boost::property_tree::ptree pt;
		boost::property_tree::read_ini(parameterFileName, pt);

		std::string serviceName    = pt.get<std::string>(PARAMETER_FILE_KEY_GENERAL_SERVICE_NAME);
		std::string deviceType     = pt.get<std::string>(PARAMETER_FILE_KEY_GENERAL_DEVICE_TYPE);
		std::string deviceUniqueID = pt.get<std::string>(PARAMETER_FILE_KEY_GENERAL_DEVICE_UNIQUE_ID);

		std::cout << PARAMETER_FILE_KEY_GENERAL_SERVICE_NAME     << ":" << serviceName    << std::endl;
		std::cout << PARAMETER_FILE_KEY_GENERAL_DEVICE_TYPE      << ":" << deviceType     << std::endl;
		std::cout << PARAMETER_FILE_KEY_GENERAL_DEVICE_UNIQUE_ID << ":" << deviceUniqueID << std::endl;

		this->optiTrackDeviceManager = OptiTrackDeviceManager(serviceName, deviceType, deviceUniqueID);
	}
	catch (boost::exception &ex)
	{
		std::cout << parameterFileName << " ERR :" << *boost::diagnostic_information_what(ex) << std::endl;
		exit(-1);
	}
	catch (...)
	{
		std::cout << "Some exception occurred in readIniFileAndInitialize()." << std::endl;
		exit(-1);
	}
}


extern "C" Controller * createController()
{
	return new AvatarControllerByOptiTrack;
}
