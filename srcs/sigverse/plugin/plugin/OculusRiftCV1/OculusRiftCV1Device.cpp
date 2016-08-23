#include <sigverse/plugin/plugin/OculusRiftCV1/OculusRiftCV1Device.h>
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
	params.Flags = 0;
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

void OculusRiftCV1Device::Process(OculusRiftCV1SensorData* sensorData)
{
    static ovrPosef eyeRenderPose[2];
    static ovrTrackingState HmdState;

    // update eye states
    EyeRenderDesc[0] = ovr_GetRenderDesc(Session, ovrEye_Left, HmdDesc.DefaultEyeFov[0]);
    EyeRenderDesc[1] = ovr_GetRenderDesc(Session, ovrEye_Right,HmdDesc.DefaultEyeFov[1]);
    //ovr_GetInputState(Session, ovrControllerType::ovrControllerType_Remote,)
    HmdState = ovr_GetTrackingState(Session, 0, ovrTrue);
    ovrVector3f viewOffset[2] = { EyeRenderDesc[0].HmdToEyeOffset, EyeRenderDesc[1].HmdToEyeOffset };
    ovr_CalcEyePoses(HmdState.HeadPose.ThePose, viewOffset, EyeRenderPose);

    sensorData->setQuaternion(HmdState.HeadPose.ThePose.Orientation.w,
                              HmdState.HeadPose.ThePose.Orientation.x,
                              HmdState.HeadPose.ThePose.Orientation.y,
                              HmdState.HeadPose.ThePose.Orientation.z);
    
    sensorData->setPosition(  HmdState.HeadPose.ThePose.Position.x,
                              HmdState.HeadPose.ThePose.Position.y,
                              HmdState.HeadPose.ThePose.Position.z);

    ovrInputState buttonState;
    ovrResult res = ovr_GetInputState(Session, ovrControllerType::ovrControllerType_Active, &buttonState);
    if (OVR_SUCCESS(res))
    {
        std::vector<std::string> pressed_buttons;
        if (buttonState.Buttons & ovrButton::ovrButton_A)
            pressed_buttons.push_back("A");
        if (buttonState.Buttons & ovrButton::ovrButton_B)
            pressed_buttons.push_back("B");
        if (buttonState.Buttons & ovrButton::ovrButton_Back)
            pressed_buttons.push_back("Back");
        if (buttonState.Buttons & ovrButton::ovrButton_Down)
            pressed_buttons.push_back("Down");
        if (buttonState.Buttons & ovrButton::ovrButton_Enter)
            pressed_buttons.push_back("Enter");
        if (buttonState.Buttons & ovrButton::ovrButton_Home)
            pressed_buttons.push_back("Home");
        if (buttonState.Buttons & ovrButton::ovrButton_Left)
            pressed_buttons.push_back("Left");
        if (buttonState.Buttons & ovrButton::ovrButton_LMask)
            pressed_buttons.push_back("LMask");
        if (buttonState.Buttons & ovrButton::ovrButton_LShoulder)
            pressed_buttons.push_back("LShoulder");
        if (buttonState.Buttons & ovrButton::ovrButton_LThumb)
            pressed_buttons.push_back("LThumb");
        if (buttonState.Buttons & ovrButton::ovrButton_Right)
            pressed_buttons.push_back("Right");
        if (buttonState.Buttons & ovrButton::ovrButton_RMask)
            pressed_buttons.push_back("RMask");
        if (buttonState.Buttons & ovrButton::ovrButton_RShoulder)
            pressed_buttons.push_back("RShoulder");
        if (buttonState.Buttons & ovrButton::ovrButton_RThumb)
            pressed_buttons.push_back("RThumb");
        if (buttonState.Buttons & ovrButton::ovrButton_Up)
            pressed_buttons.push_back("Up");
        if (buttonState.Buttons & ovrButton::ovrButton_VolDown)
            pressed_buttons.push_back("VolDown");
        if (buttonState.Buttons & ovrButton::ovrButton_VolUp)
            pressed_buttons.push_back("VolUp");
        if (buttonState.Buttons & ovrButton::ovrButton_X)
            pressed_buttons.push_back("X");
        if (buttonState.Buttons & ovrButton::ovrButton_Y)
            pressed_buttons.push_back("Y");

        sensorData->setButtonStates(pressed_buttons);
    }
    else
    {
        std::cout << "Could not retrieve input state for any connected controllers! Error code: " << res << std::endl;
    }
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
        if (!OVR_SUCCESS(res))
        {
            std::cerr << "ERROR: Could not retrieve OVR Session Status!" << std::endl;
        }

        if (_kbhit()) {
            int key = _getch();
            if (key == 'r') {
                resetView();
            }
        }

        if (sessionStatus.ShouldRecenter)
        {
            resetView();
        }
        if (sessionStatus.ShouldQuit)
        {
            std::cout << "OVRSession requested shutdown -- quitting..." << std::endl;
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

        OculusRiftCV1SensorData sensorData;
        Process(&sensorData);

        if (this->sendMessageFlag) {
            const std::string sensorDataMessage = sensorData.encodeSensorData();
            const std::string messageHeader = this->generateMessageHeader();
            const std::string message = messageHeader + sensorDataMessage;

            this->sendMessage(sigService, message);
        }
        Sleep(10);
    }

    sigService.disconnect();

    std::cout << "end program" << std::endl;
    Sleep(2000);
}

