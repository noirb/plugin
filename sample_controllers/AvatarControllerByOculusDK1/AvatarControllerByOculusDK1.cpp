/*
 * AvatarControllerByOculusDK1
 *
 *  Created on: 2015/03/12
 *      Author: Nozaki
 */

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <sigverse/plugin/common/sensor/OculusRiftDK1SensorData.h>
#include <cmath>
#include "AvatarControllerByOculusDK1.h"

///@brief Initialize this controller.
void AvatarControllerByOculusDK1::onInit(InitEvent &evt)
{
	readIniFileAndInitialize();

	//For initialize.
	getObj(myname());
}


///@brief Movement of the robot.
double AvatarControllerByOculusDK1::onAction(ActionEvent &evt)
{
	bool oculusDK1Available = checkService(this->oculusDK1DeviceManager.serviceName);

	if (oculusDK1Available && this->oculusDK1DeviceManager.service == NULL)
	{
		this->oculusDK1DeviceManager.service = connectToService(this->oculusDK1DeviceManager.serviceName);
	}
	else if (!oculusDK1Available && this->oculusDK1DeviceManager.service != NULL)
	{
		this->oculusDK1DeviceManager.service = NULL;
	}

	return 1.0;
}

void AvatarControllerByOculusDK1::onRecvMsg(RecvMsgEvent &evt)
{
	const std::string allMsg = evt.getMsg();

	//std::cout << allMsg << std::endl;

	std::map<std::string, std::vector<std::string> > sensorDataMap = OculusRiftDK1SensorData::decodeSensorData(allMsg);

	if (sensorDataMap.find(MSG_KEY_DEV_TYPE) == sensorDataMap.end()){ return; }

	if(sensorDataMap[MSG_KEY_DEV_TYPE][0]     !=this->oculusDK1DeviceManager.deviceType    ){ return; }
	if(sensorDataMap[MSG_KEY_DEV_UNIQUE_ID][0]!=this->oculusDK1DeviceManager.deviceUniqueID){ return; }

	OculusRiftDK1SensorData sensorData;
	sensorData.setSensorData(sensorDataMap);

	ManNiiPosture posture = OculusDK1DeviceManager::convertEulerAngle2ManNiiPosture(sensorData.getEulerAngle());

	SimObj *obj = getObj(myname());
	OculusDK1DeviceManager::setJointQuaternions2ManNii(obj, posture);
}



///@brief Read parameter file.
void AvatarControllerByOculusDK1::readIniFileAndInitialize()
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

		this->oculusDK1DeviceManager = OculusDK1DeviceManager(serviceName, deviceType, deviceUniqueID);
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
	return new AvatarControllerByOculusDK1;
}


