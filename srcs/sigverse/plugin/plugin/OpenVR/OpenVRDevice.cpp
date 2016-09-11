#include <sigverse/plugin/plugin/OpenVR/OpenVRDevice.h>
#include <sigverse/plugin/plugin/common/CheckRecvSIGServiceData.h>
#include <Windows.h>
#include <boost/thread.hpp>
#include <boost/thread/thread.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/exception/diagnostic_information.hpp>


///@brief Parameter file name.
const std::string Device::parameterFileName = "OpenVR.ini";


OpenVRDevice::OpenVRDevice(int argc, char **argv)
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

OpenVRDevice::~OpenVRDevice()
{
    Release();
}

void OpenVRDevice::init()
{
    // Initializes OpenVR and any tracked objects
    vr::EVRInitError eError = vr::VRInitError_None;
    m_pHMD = vr::VR_Init(&eError, vr::VRApplication_Background);
    if (eError != vr::VRInitError_None)
    {
        MessageBoxA(NULL, VR_GetVRInitErrorAsSymbol(eError), "Failed to initialize OpenVR!", MB_OK);
        m_pHMD = NULL;
        return;
    }

    if (!vr::VRCompositor())
    {
        MessageBoxA(NULL, "OpenVR Compositor failed to start!", "Failed to initialize Compositor!", MB_OK);
        return;
    }

    std::cout << "-------------- Function --------------\n" << std::endl;
    std::cout << "Press \"r\" key : Reset center position." << std::endl;
    std::cout << "\n--------------------------------------\n" << std::endl;
}

void OpenVRDevice::Process(OpenVRSensorData* sensorData)
{
    vr::VRCompositor()->WaitGetPoses(m_rTrackedDevicePose, vr::k_unMaxTrackedDeviceCount, NULL, 0);
    m_pHMD->GetDeviceToAbsoluteTrackingPose(vr::ETrackingUniverseOrigin::TrackingUniverseStanding, 0, m_rTrackedDevicePose, vr::k_unMaxTrackedDeviceCount);

    for (size_t i = 0; i < vr::k_unMaxTrackedDeviceCount; ++i)
    {
        if (m_rTrackedDevicePose[i].bPoseIsValid)
        {
            switch (m_pHMD->GetTrackedDeviceClass(i))
            {
                case vr::TrackedDeviceClass_HMD:
                {
                    SigCmn::Vector4 hmd_quat = extractQuaternion(m_rTrackedDevicePose[i].mDeviceToAbsoluteTracking);
                    sensorData->setQuaternion(OpenVRSensorData::DeviceType::HMD, hmd_quat);
                    sensorData->setPosition(OpenVRSensorData::DeviceType::HMD,
                        m_rTrackedDevicePose[i].mDeviceToAbsoluteTracking.m[0][3],
                        m_rTrackedDevicePose[i].mDeviceToAbsoluteTracking.m[1][3],
                        m_rTrackedDevicePose[i].mDeviceToAbsoluteTracking.m[2][3]);
                    break;
                }
                case vr::TrackedDeviceClass_Controller:
                {
                    SigCmn::Vector4 ctrlr_quat = extractQuaternion(m_rTrackedDevicePose[i].mDeviceToAbsoluteTracking);

                    auto controllerRole = m_pHMD->GetInt32TrackedDeviceProperty(i, vr::ETrackedDeviceProperty::Prop_ControllerRoleHint_Int32);
                    if (controllerRole == vr::ETrackedControllerRole::TrackedControllerRole_RightHand)
                    {
                        sensorData->setQuaternion(OpenVRSensorData::DeviceType::Controller_R, ctrlr_quat);
                        sensorData->setPosition(OpenVRSensorData::DeviceType::Controller_R,
                            m_rTrackedDevicePose[i].mDeviceToAbsoluteTracking.m[0][3],
                            m_rTrackedDevicePose[i].mDeviceToAbsoluteTracking.m[1][3],
                            m_rTrackedDevicePose[i].mDeviceToAbsoluteTracking.m[2][3]);
                    }
                    else if (controllerRole == vr::ETrackedControllerRole::TrackedControllerRole_LeftHand)
                    {
                        sensorData->setQuaternion(OpenVRSensorData::DeviceType::Controller_L, ctrlr_quat);
                        sensorData->setPosition(OpenVRSensorData::DeviceType::Controller_L,
                            m_rTrackedDevicePose[i].mDeviceToAbsoluteTracking.m[0][3],
                            m_rTrackedDevicePose[i].mDeviceToAbsoluteTracking.m[1][3],
                            m_rTrackedDevicePose[i].mDeviceToAbsoluteTracking.m[2][3]);
                    }
                    break;
                }
                case vr::TrackedDeviceClass_Other:
                {
                    break;
                }
                case vr::TrackedDeviceClass_Invalid:
                {
                    break;
                }
                default:
                {
//                    std::cerr << "Device " << i << " has unknown type!" << std::endl;
                    break;
                }
            }
        }
    }

}

void OpenVRDevice::Release()
{

}

void OpenVRDevice::resetView()
{
    m_pHMD->ResetSeatedZeroPose();

    std::cout << "Trying to reset position/orientation" << std::endl;
}

void OpenVRDevice::run()
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
        if (_kbhit()) {
            int key = _getch();
            if (key == 'r') {
                resetView();
            }
        }

        OpenVRSensorData sensorData;
        Process(&sensorData);

//        std::cout << sensorData.encodeSensorData() << std::endl << std::endl << "----" << std::endl << std::endl;
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

