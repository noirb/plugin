/*
 * MyoDeviceManager.cpp
 *
 *  Created on: 2016/04/08
 *      Author: Yamada
 */

#include <sigverse/plugin/controller/common/sensor/MyoDeviceManager.h>

MyoDeviceManager::MyoDeviceManager()
{
	this->service        = NULL;
	this->serviceName    = "";
	this->deviceType     = "";
	this->deviceUniqueID = "";
}

MyoDeviceManager::MyoDeviceManager(std::string &name, std::string &deviceType, std::string &deviceUniqueID)
{
	this->service        = NULL;
	this->serviceName    = name;
	this->deviceType     = deviceType;
	this->deviceUniqueID = deviceUniqueID;
}

