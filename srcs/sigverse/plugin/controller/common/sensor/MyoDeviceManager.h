/*
 * MyoDeviceManager.h
 *
 *  Created on: 2016/04/08
 *      Author: Yamada
 */
#ifndef SIGVERSE_MYO_DEVICE_MANAGER_H
#define SIGVERSE_MYO_DEVICE_MANAGER_H

#include <sigverse/commonlib/SimObj.h>
#include <sigverse/commonlib/ControllerImpl.h>

class MyoDeviceManager
{
public:
	BaseService *service;

	std::string serviceName;
	std::string deviceType;
	std::string deviceUniqueID;

	MyoDeviceManager();
	MyoDeviceManager(std::string &name, std::string &deviceType, std::string &deviceUniqueID);
};

#endif //SIGVERSE_MYO_DEVICE_MANAGER_H
