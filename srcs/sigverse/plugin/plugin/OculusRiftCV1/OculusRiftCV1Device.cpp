#include <sigverse/plugin/plugin/OculusRiftCV1/OculusRiftCV1Device.h>
#include <sigverse/plugin/common/sensor/OculusRiftCV1SensorData.h>
#include <sigverse/plugin/plugin/common/CheckRecvSIGServiceData.h>
#include <Windows.h>
#include <boost/thread.hpp>
#include <boost/thread/thread.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/exception/diagnostic_information.hpp>

///@brief Parameter file name.
const std::string Device::parameterFileName = "OculusRiftCV1.ini";


OculusRiftCV1Device::OculusRiftCV1Device(int argc, char **argv)
{
	if (argc == 3) {
		std::cout << "SIGServer IP address: " << argv[1] << std::endl;
		std::cout << "Port number: " << argv[2] << std::endl;
		this->sendMessageFlag = true;
		this->serverAddress = argv[1];
		this->portNumber = atoi(argv[2]);
	}
	else {
		std::cout << "Please execute with SIGServer address and port number." << std::endl;
		exit(1);
	}
}

OculusRiftCV1Device::~OculusRiftCV1Device()
{
	// finalize Oculus Rift connection
	Release();
}

void OculusRiftCV1Device::init()
{
	// Initializes LibOVR, and the Rift
	ovrInitParams params;
	params.Flags = ovrInit_Debug;
	params.ConnectionTimeoutMS = 0;
	params.RequestedMinorVersion = 6;
	params.UserData = 0;
	params.LogCallback = 0;
	ovrResult res = ovr_Initialize(&params);
	if (!OVR_SUCCESS(res))
	{
		MessageBoxA(NULL, "Could not initialize LibOVR!", "", MB_OK);
	}

	res = ovr_Create(&Session, &Luid);
	if (!OVR_SUCCESS(res))
	{
        MessageBoxA(NULL, "Could not create OVR Session!", "", MB_OK);
	}

	res = ovr_RecenterTrackingOrigin(Session);
	if (!OVR_SUCCESS(res))
	{
		std::cerr << "ERROR: Could not recenter HMD Pose!" << std::endl;
	}

    HmdDesc = ovr_GetHmdDesc(Session);

	std::cout << "-------------- Function --------------\n" << std::endl;
	std::cout << "Press \"r\" key : Reset center position." << std::endl;
	std::cout << "\n--------------------------------------\n" << std::endl;
}

void OculusRiftCV1Device::Process(ovrQuatf* Q)
{
	static ovrPosef eyeRenderPose[2];
	static ovrTrackingState HmdState;

    // update eye states
    EyeRenderDesc[0] = ovr_GetRenderDesc(Session, ovrEye_Left, HmdDesc.DefaultEyeFov[0]);
    EyeRenderDesc[1] = ovr_GetRenderDesc(Session, ovrEye_Right,HmdDesc.DefaultEyeFov[1]);

    HmdState = ovr_GetTrackingState(Session, 0, ovrTrue);
    ovrVector3f viewOffset[2] = { EyeRenderDesc[0].HmdToEyeOffset, EyeRenderDesc[1].HmdToEyeOffset };
    ovr_CalcEyePoses(HmdState.HeadPose.ThePose, viewOffset, EyeRenderPose);

	*Q = HmdState.HeadPose.ThePose.Orientation;
}

void OculusRiftCV1Device::Release(void)
{
    ovr_Destroy(Session);

	// No OVR functions involving memory are allowed after this.
	ovr_Shutdown();
}

void OculusRiftCV1Device::resetView()
{
    ovrResult res = ovr_RecenterTrackingOrigin(Session);
    if (!OVR_SUCCESS(res))
    {
        std::cerr << "ERROR: Could not recenter HMD Pose!" << std::endl;
    }
    else
    {
        std::cout << "Reset center position" << std::endl;
    }
}

void OculusRiftCV1Device::run()
{
	init();

	this->readIniFile();

	sigverse::SIGService sigService(this->serviceName);
	this->initializeSigService(sigService);

	// check receive SIGService data by another thread
	CheckRecvSIGServiceData checkRecvSIGServiceData;
	boost::thread thCheckRecvData(&CheckRecvSIGServiceData::run, &checkRecvSIGServiceData, &sigService);

	bool sendSuccessPrev = false;

    ovrSessionStatus sessionStatus;
	while (1) {
        ovrResult res = ovr_GetSessionStatus(Session, &sessionStatus);

		if (_kbhit()){
			int key = _getch();
			if (key == 'r'){
                resetView();
			}
		}

        
        if (!OVR_SUCCESS(res))
        {
            std::cerr << "ERROR: Could not retrieve OVR Session Status!" << std::endl;
            return;
        }

        if (sessionStatus.ShouldRecenter)
        {
            resetView();
        }
        if (sessionStatus.ShouldQuit)
        {
            break;
        }
        if (sessionStatus.DisplayLost)
        {
            std::cerr << "ERROR: HMD Display lost!" << std::endl;
        }
        if (!sessionStatus.HmdPresent)
        {
            std::cerr << "ERROR: HMD Not connected!" << std::endl;
        }

		ovrQuatf q;
		Process(&q);

		OculusRiftCV1SensorData sensorData;
		sensorData.setQuaternion(q.w, q.x, q.y, q.z);

		if (this->sendMessageFlag) {
			const std::string sensorDataMessage = sensorData.encodeSensorData();
			const std::string messageHeader = this->generateMessageHeader();
			const std::string message = messageHeader + sensorDataMessage;

			this->sendMessage(sigService, message);

			OculusRiftCV1SensorData tmp;
			std::map<std::string, std::vector<std::string> > sensorDataMap = tmp.decodeSensorData(message);
			tmp.setSensorData(sensorDataMap);
		}
		Sleep(10);
	}

	sigService.disconnect();

	std::cout << "end program" << std::endl;
	Sleep(2000);
}

