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

    vr::VRCompositor()->SetTrackingSpace(vr::ETrackingUniverseOrigin::TrackingUniverseSeated);

    std::cout << "-------------- Function --------------\n" << std::endl;
    std::cout << "Press \"r\" key : Reset center position." << std::endl;
    std::cout << "\n--------------------------------------\n" << std::endl;
}

std::vector<std::string> OpenVRDevice::getButtonStates(vr::VRControllerState_t controller_state, uint32_t controller_index)
{
    std::vector<std::string> output;

    if (controller_state.ulButtonPressed & vr::ButtonMaskFromId(vr::EVRButtonId::k_EButton_A))
    {
        output.push_back(m_pHMD->GetButtonIdNameFromEnum(vr::EVRButtonId::k_EButton_A));
    }

    if (controller_state.ulButtonPressed & vr::ButtonMaskFromId(vr::EVRButtonId::k_EButton_ApplicationMenu))
    {
        output.push_back(m_pHMD->GetButtonIdNameFromEnum(vr::EVRButtonId::k_EButton_ApplicationMenu));
    }

    if (controller_state.ulButtonPressed & vr::ButtonMaskFromId(vr::EVRButtonId::k_EButton_Dashboard_Back))
    {
        output.push_back(m_pHMD->GetButtonIdNameFromEnum(vr::EVRButtonId::k_EButton_Dashboard_Back));
    }

    if (controller_state.ulButtonPressed & vr::ButtonMaskFromId(vr::EVRButtonId::k_EButton_DPad_Down))
    {
        output.push_back(m_pHMD->GetButtonIdNameFromEnum(vr::EVRButtonId::k_EButton_DPad_Down));
    }

    if (controller_state.ulButtonPressed & vr::ButtonMaskFromId(vr::EVRButtonId::k_EButton_DPad_Left))
    {
        output.push_back(m_pHMD->GetButtonIdNameFromEnum(vr::EVRButtonId::k_EButton_DPad_Left));
    }

    if (controller_state.ulButtonPressed & vr::ButtonMaskFromId(vr::EVRButtonId::k_EButton_DPad_Right))
    {
        output.push_back(m_pHMD->GetButtonIdNameFromEnum(vr::EVRButtonId::k_EButton_DPad_Right));
    }

    if (controller_state.ulButtonPressed & vr::ButtonMaskFromId(vr::EVRButtonId::k_EButton_DPad_Up))
    {
        output.push_back(m_pHMD->GetButtonIdNameFromEnum(vr::EVRButtonId::k_EButton_DPad_Up));
    }

    if (controller_state.ulButtonPressed & vr::ButtonMaskFromId(vr::EVRButtonId::k_EButton_Grip))
    {
        output.push_back(m_pHMD->GetButtonIdNameFromEnum(vr::EVRButtonId::k_EButton_Grip));
    }

    if (controller_state.ulButtonPressed & vr::ButtonMaskFromId(vr::EVRButtonId::k_EButton_Max))
    {
        output.push_back(m_pHMD->GetButtonIdNameFromEnum(vr::EVRButtonId::k_EButton_Max));
    }

    if (controller_state.ulButtonPressed & vr::ButtonMaskFromId(vr::EVRButtonId::k_EButton_SteamVR_Touchpad))
    {
        output.push_back(m_pHMD->GetButtonIdNameFromEnum(vr::EVRButtonId::k_EButton_SteamVR_Touchpad));
    }

    if (controller_state.ulButtonPressed & vr::ButtonMaskFromId(vr::EVRButtonId::k_EButton_SteamVR_Trigger))
    {
        output.push_back(m_pHMD->GetButtonIdNameFromEnum(vr::EVRButtonId::k_EButton_SteamVR_Trigger));
    }

    if (controller_state.ulButtonPressed & vr::ButtonMaskFromId(vr::EVRButtonId::k_EButton_System))
    {
        output.push_back(m_pHMD->GetButtonIdNameFromEnum(vr::EVRButtonId::k_EButton_System));
    }

    return output;
}

std::vector<std::string> OpenVRDevice::getAxisStates(vr::VRControllerState_t controller_state, uint32_t controller_index)
{
    std::vector<std::string> output;

    for (uint32_t i = 0; i < 5; i++)
    {
        auto axis_type = m_pHMD->GetInt32TrackedDeviceProperty(controller_index, (vr::ETrackedDeviceProperty)((uint32_t)vr::ETrackedDeviceProperty::Prop_Axis0Type_Int32 + i));
        if (axis_type == vr::EVRControllerAxisType::k_eControllerAxis_Trigger)
        {
            
            output.push_back(m_pHMD->GetControllerAxisTypeNameFromEnum((vr::EVRControllerAxisType)axis_type) + std::string(":")
                                                                        + std::to_string(controller_state.rAxis[i].x)); // trigger only has data in x
        }
        else
        {
            output.push_back(m_pHMD->GetControllerAxisTypeNameFromEnum((vr::EVRControllerAxisType)axis_type) + std::string(":")
                + std::to_string(controller_state.rAxis[i].x) + "," // all other axis types have both x, y values
                + std::to_string(controller_state.rAxis[i].y));
        }
    }

    return output;
}

void OpenVRDevice::Process(OpenVRSensorData* sensorData)
{
    vr::VRCompositor()->WaitGetPoses(m_rTrackedDevicePose, vr::k_unMaxTrackedDeviceCount, NULL, 0);
    m_pHMD->GetDeviceToAbsoluteTrackingPose(vr::ETrackingUniverseOrigin::TrackingUniverseSeated, 0, m_rTrackedDevicePose, vr::k_unMaxTrackedDeviceCount);

    auto idx_l = m_pHMD->GetTrackedDeviceIndexForControllerRole(vr::ETrackedControllerRole::TrackedControllerRole_LeftHand);
    auto idx_r = m_pHMD->GetTrackedDeviceIndexForControllerRole(vr::ETrackedControllerRole::TrackedControllerRole_RightHand);
    std::cout << "Left idx: " << idx_l << ", right: " << idx_r << std::endl;

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
                    vr::VRControllerState_t controller_state;
                    if (!m_pHMD->GetControllerState(i, &controller_state))
                    {
                        std::cout << "Could not get controller state for device " << i << std::endl;
                    }

                    if (i == idx_r)
                    {
                        std::cout << "Right controller" << std::endl;
                        sensorData->setQuaternion(OpenVRSensorData::DeviceType::Controller_R, ctrlr_quat);
                        sensorData->setPosition(OpenVRSensorData::DeviceType::Controller_R,
                            m_rTrackedDevicePose[i].mDeviceToAbsoluteTracking.m[0][3],
                            m_rTrackedDevicePose[i].mDeviceToAbsoluteTracking.m[1][3],
                            m_rTrackedDevicePose[i].mDeviceToAbsoluteTracking.m[2][3]);
                        sensorData->setButtonStates(OpenVRSensorData::DeviceType::Controller_R, getButtonStates(controller_state, i));
                    }
                    else if (i == idx_l)
                    {
                        std::cout << "Left controller" << std::endl;
                        sensorData->setQuaternion(OpenVRSensorData::DeviceType::Controller_L, ctrlr_quat);
                        sensorData->setPosition(OpenVRSensorData::DeviceType::Controller_L,
                            m_rTrackedDevicePose[i].mDeviceToAbsoluteTracking.m[0][3],
                            m_rTrackedDevicePose[i].mDeviceToAbsoluteTracking.m[1][3],
                            m_rTrackedDevicePose[i].mDeviceToAbsoluteTracking.m[2][3]);
                        sensorData->setButtonStates(OpenVRSensorData::DeviceType::Controller_L, getButtonStates(controller_state, i));
                    }
                    else
                    {
                        std::cout << "Invalid controller " << i << std::endl;
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

