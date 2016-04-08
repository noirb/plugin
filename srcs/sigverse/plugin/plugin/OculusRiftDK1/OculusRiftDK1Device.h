#pragma once
#include <SIGService.h>
#include <sigverse/plugin/plugin/common/Device.h>

#include <OVR.h>
#include <conio.h>
#include <iostream>

#undef new

class OculusRiftDK1Device : public Device
{
private:
	OVR::Ptr<OVR::DeviceManager>  pManager;
	OVR::Ptr<OVR::HMDDevice>      pHMD;
	OVR::Ptr<OVR::SensorDevice>   pSensor;
	OVR::SensorFusion*            pFusionResult;
	OVR::HMDInfo                  Info;
	bool                          InfoLoaded;

public:
	OculusRiftDK1Device(){};
	OculusRiftDK1Device(int argc, char **argv);
	~OculusRiftDK1Device();
	
	/////@brief Set service name.
	//void setSigServiceName();

	///@brief 
	void init();

	///@brief Run oculus dk1 device.
	void run();
};

