#ifndef SIGVERSE_OPENVR_SENSOR_DATA_H
#define SIGVERSE_OPENVR_SENSOR_DATA_H

#include <sigverse/plugin/common/sensor/SensorData.h>
#include <sigverse/plugin/common/SigCmn.h>

#define MSG_KEY_HMD_ROT          "HMD_QUATERNION"
#define MSG_KEY_HMD_POS          "HMD_POSITION"
#define MSG_KEY_CONTROLLER_L_ROT "CONTROLLER_L_QUATERNION"
#define MSG_KEY_CONTROLLER_L_POS "CONTROLLER_L_POSITION"
#define MSG_KEY_CONTROLLER_L_BTN "CONTROLLER_L_BUTTONS"
#define MSG_KEY_CONTROLLER_R_ROT "CONTROLLER_R_QUATERNION"
#define MSG_KEY_CONTROLLER_R_POS "CONTROLLER_R_POSITION"
#define MSG_KEY_CONTROLLER_R_BTN "CONTROLLER_R_BUTTONS"

class OpenVRSensorData :
    public SensorData
{
private:
    SigCmn::Vector4 hmd_quaternion     = { 1, 0, 0, 0 }; // orientation of headset
    SigCmn::Vector3 hmd_position       = { 0, 0, 0, };   // position of headset
    SigCmn::Vector4 ctrl_L_quaternion  = { 1, 0, 0, 0 }; // orientation of left controller
    SigCmn::Vector3 ctrl_L_position    = { 0, 0, 0 };    // position of left controller
    SigCmn::Vector4 ctrl_R_quaternion  = { 1, 0, 0, 0 }; // orientation of right controller
    SigCmn::Vector3 ctrl_R_position    = { 0, 0, 0 };    // position of right controller
    
    std::vector<std::string> ctrl_L_buttons; // list of currently-pressed buttons on left controller
    std::vector<std::string> ctrl_R_buttons; // list of currently-pressed buttons on right controller

public:

    enum DeviceType {
        HMD,
        Controller_L,
        Controller_R
    };

    ///@brief Convert sensor information to string.
    ///@param itemsDelim Symbol to split items.
    ///@param keyValueDelim Symbol to split KEY and VALUE.
    ///@param valuesDelim Symbol to split each componet of VALUE. When VALUE is multidimensional data, you use this symbol.
    std::string encodeSensorData(const std::string &itemsDelim = ITEMS_DELIMITER, const std::string &keyValueDelim = KEY_AND_VALUE_DELIMITER, const std::string &valuesDelim = VALUES_DELIMITER) const;

    ///@brief Set to sensor data.
    ///@param Map of sensor data;
    bool setSensorData(const std::map<std::string, std::vector<std::string> > &sensorDataMap);

    ///@brief Set sensor orientation
    void setQuaternion(DeviceType device, SigCmn::Vector4 quat);

    ///@brief Set sensor orientation
    void setQuaternion(DeviceType device, const float &w, const float &x, const float &y, const float &z);

    ///@brief Set sensor position
    void setPosition(DeviceType device, const float &x, const float &y, const float &z);

    ///@brief Set current set of pressed buttons
    void setButtonStates(DeviceType device, std::vector<std::string> buttons);

    ///@brief Get sensor orientation
    SigCmn::Vector4 getQuaternion(DeviceType device) const;

    ///@brief Get sensor position
    SigCmn::Vector3 getPosition(DeviceType device) const;

    ///@brief Get list of currently-pressed buttons
    std::vector<std::string> getButtonStates(DeviceType device) const;
};
#endif // SIGVERSE_OPENVR_SENSOR_DATA_H
