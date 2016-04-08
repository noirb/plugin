#include <sigverse/plugin/plugin/OculusRiftDK2/OculusRiftDK2Device.h>
#include <sigverse/plugin/common/sensor/OculusRiftDK2SensorData.h>
#include <sigverse/plugin/plugin/common/CheckRecvSIGServiceData.h>
#include <Windows.h>
#include <boost/thread.hpp>
#include <boost/thread/thread.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/exception/diagnostic_information.hpp>

///@brief Parameter file name.
const std::string Device::parameterFileName = "OculusRiftDK2.ini";


OculusRiftDK2Device::OculusRiftDK2Device(int argc, char **argv)
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

OculusRiftDK2Device::~OculusRiftDK2Device()
{
	// finalize Oculus Rift connection
	Release();
	OVR_ASSERT(!_CrtDumpMemoryLeaks());
}

void OculusRiftDK2Device::init()
{
	HMD = 0;
	// Initializes LibOVR, and the Rift
	ovr_Initialize();
	if (!HMD)
	{
		HMD = ovrHmd_Create(0);
		if (!HMD)
		{
			MessageBoxA(NULL, "Oculus Rift not detected.", "", MB_OK);
			exit(-1);
		}
		if (HMD->ProductName[0] == '\0')
			MessageBoxA(NULL, "Rift detected, display not enabled.", "", MB_OK);
	}

	ovrHmd_SetEnabledCaps(HMD, ovrHmdCap_LowPersistence | ovrHmdCap_DynamicPrediction);

	// Start the sensor which informs of the Rift's pose and motion
	ovrHmd_ConfigureTracking(HMD, ovrTrackingCap_Orientation |
		ovrTrackingCap_MagYawCorrection |
		ovrTrackingCap_Position, 0);

	ovrHmd_RecenterPose(HMD);

	std::cout << "-------------- Function --------------\n" << std::endl;
	std::cout << "Press \"r\" key : Reset center position." << std::endl;
	std::cout << "\n--------------------------------------\n" << std::endl;
}

void OculusRiftDK2Device::Process(Quatf* Q)
{
	static ovrPosef eyeRenderPose[2];

	static ovrTrackingState HmdState;

	ovrVector3f hmdToEyeViewOffset[2] = { EyeRenderDesc[0].HmdToEyeViewOffset, EyeRenderDesc[1].HmdToEyeViewOffset };
	ovrHmd_GetEyePoses(HMD, 0, hmdToEyeViewOffset, eyeRenderPose, &HmdState);

	*Q = HmdState.HeadPose.ThePose.Orientation;
}

void OculusRiftDK2Device::Release(void)
{
	ovrHmd_Destroy(HMD);
	HMD = 0;

	// No OVR functions involving memory are allowed after this.
	ovr_Shutdown();
}

void OculusRiftDK2Device::run()
{
	init();

	this->readIniFile();

	sigverse::SIGService sigService(this->serviceName);
	this->initializeSigService(sigService);

	// check receive SIGService data by another thread
	CheckRecvSIGServiceData checkRecvSIGServiceData;
	boost::thread thCheckRecvData(&CheckRecvSIGServiceData::run, &checkRecvSIGServiceData, &sigService);

	bool sendSuccessPrev = false;

	while (1) {

		if (_kbhit()){
			int key = _getch();
			if (key == 'r'){
				ovrHmd_RecenterPose(HMD);
				
				std::cout << "Reset center position" << std::endl;
			}
		}
		Quatf q;
		Process(&q);

		OculusRiftDK2SensorData sensorData;
		sensorData.setQuaternion(q.w, q.x, q.y, q.z);

		if (this->sendMessageFlag) {
			const std::string sensorDataMessage = sensorData.encodeSensorData();
			const std::string messageHeader = this->generateMessageHeader();
			const std::string message = messageHeader + sensorDataMessage;

			this->sendMessage(sigService, message);

			OculusRiftDK2SensorData tmp;
			std::map<std::string, std::vector<std::string> > sensorDataMap = tmp.decodeSensorData(message);
			tmp.setSensorData(sensorDataMap);
		}
		Sleep(10);
	}

	sigService.disconnect();

	std::cout << "end program" << std::endl;
	Sleep(2000);
}

