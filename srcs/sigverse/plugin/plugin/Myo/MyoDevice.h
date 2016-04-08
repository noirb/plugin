#pragma once

#include <SIGService.h>
#include <sigverse/plugin/common/sensor/MyoSensorData.h>
#include <sigverse/plugin/plugin/common/Device.h>
#include <myo/myo.hpp>

class MyoDevice : public Device
{
private:
	sigverse::SIGService sigService;


public:
	MyoDevice();
	MyoDevice(int argc, char **argv);
	~MyoDevice();

	///@brief Execute a device.
	int run();
};

