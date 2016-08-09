#pragma once
#include <SIGService.h>
#include <sigverse/plugin/plugin/common/Device.h>
#include <OVR_CAPI.h>
#include <d3d11.h>
#include <conio.h>
#include <iostream>

#undef new


class OculusRiftCV1Device :
	public Device
{
private:
	///@brief Parameter file name.
	std::string parameterFileName;

	//Structures for the application
	ovrSession		   Session;
    ovrGraphicsLuid    Luid;
    ovrHmdDesc         HmdDesc;
    ovrPosef           EyeRenderPose[2];
	ovrEyeRenderDesc   EyeRenderDesc[2];
	ovrRecti           EyeRenderViewport[2];

public:
	OculusRiftCV1Device(){};
	OculusRiftCV1Device(int argc, char **argv);
	~OculusRiftCV1Device();

	void Process(ovrQuatf* Q);

	void Release();

	///@brief Set service name.
	//void setSigServiceName();

	///@brief 
	void init();

    ///@breif Resets HMD pose
    void resetView();

	///@brief Run oculus CV1 device.
	void run();
};