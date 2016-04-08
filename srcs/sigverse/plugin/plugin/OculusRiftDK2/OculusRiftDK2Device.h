#pragma once
#include <SIGService.h>
#include <sigverse/plugin/plugin/common/Device.h>
#include "OVR.h"
#include <d3d11.h>
#include <conio.h>
#include <iostream>

#undef new

using namespace OVR;

class OculusRiftDK2Device :
	public Device
{
private:
	///@brief Parameter file name.
	std::string parameterFileName;

	//Structures for the application
	ovrHmd             HMD;
	ovrEyeRenderDesc   EyeRenderDesc[2];
	ovrRecti           EyeRenderViewport[2];

public:
	OculusRiftDK2Device(){};
	OculusRiftDK2Device(int argc, char **argv);
	~OculusRiftDK2Device();

	void Process(Quatf* Q);

	void Release();

	///@brief Set service name.
	//void setSigServiceName();

	///@brief 
	void init();

	///@brief Run oculus dk2 device.
	void run();
};