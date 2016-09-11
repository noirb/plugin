#pragma once
#include <SIGService.h>
#include <sigverse/plugin/plugin/common/Device.h>
#include <sigverse/plugin/common/sensor/OpenVRSensorData.h>
#include <openvr.h>
#include <conio.h>
#include <iostream>

#undef new


class OpenVRDevice :
    public Device
{
private:
    ///@brief Parameter file name.
    std::string parameterFileName;

    //Structures for the application
    vr::IVRSystem*              m_pHMD;
    vr::IVRRenderModels*        m_pRenderModels;
    std::string                 m_strDriver;
    std::string                 m_strDisplay;
    vr::TrackedDevicePose_t     m_rTrackedDevicePose[vr::k_unMaxTrackedDeviceCount];
    int                         m_iValidPoseCount;
    int                         m_iValidPoseCount_Last;
    std::string                 m_strPoseClasses;
    char                        m_rDevClassChar[vr::k_unMaxTrackedDeviceCount];

    // Adapted from: http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/
    inline SigCmn::Vector4 extractQuaternion(const vr::HmdMatrix34_t &m) const {
        SigCmn::Vector4 q;
        float trace = m.m[0][0] + m.m[1][1] + m.m[2][2]; // I removed + 1.0f; see discussion with Ethan
        if (trace > 0) {
            float s = 0.5f / sqrtf(trace + 1.0f);
            q.w = 0.25f / s;
            q.x = (m.m[2][1] - m.m[1][2]) * s;
            q.y = (m.m[0][2] - m.m[2][0]) * s;
            q.z = (m.m[1][0] - m.m[0][1]) * s;
        }
        else {
            if (m.m[0][0] > m.m[1][1] && m.m[0][0] > m.m[2][2]) {
                float s = 2.0f * sqrtf(1.0f + m.m[0][0] - m.m[1][1] - m.m[2][2]);
                q.w = (m.m[2][1] - m.m[1][2]) / s;
                q.x = 0.25f * s;
                q.y = (m.m[0][1] + m.m[1][0]) / s;
                q.z = (m.m[0][2] + m.m[2][0]) / s;
            }
            else if (m.m[1][1] > m.m[2][2]) {
                float s = 2.0f * sqrtf(1.0f + m.m[1][1] - m.m[0][0] - m.m[2][2]);
                q.w = (m.m[0][2] - m.m[2][0]) / s;
                q.x = (m.m[0][1] + m.m[1][0]) / s;
                q.y = 0.25f * s;
                q.z = (m.m[1][2] + m.m[2][1]) / s;
            }
            else {
                float s = 2.0f * sqrtf(1.0f + m.m[2][2] - m.m[0][0] - m.m[1][1]);
                q.w = (m.m[1][0] - m.m[0][1]) / s;
                q.x = (m.m[0][2] + m.m[2][0]) / s;
                q.y = (m.m[1][2] + m.m[2][1]) / s;
                q.z = 0.25f * s;
            }
        }
        return q;
    }

public:
    OpenVRDevice(){};
    OpenVRDevice(int argc, char **argv);
    ~OpenVRDevice();

    void Process(OpenVRSensorData* sensorData);

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