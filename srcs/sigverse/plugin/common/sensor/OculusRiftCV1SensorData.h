#ifndef SIGVERSE_OCULUS_RIFT_CV1_SENSOR_DATA_H
#define SIGVERSE_OCULUS_RIFT_CV1_SENSOR_DATA_H

#include <sigverse/plugin/common/sensor/SensorData.h>
#include <sigverse/plugin/common/SigCmn.h>

#define MSG_KEY_QUATERNION "QUATERNION"
#define MSG_KEY_POSITION   "POSITION"
#define MSG_KEY_BUTTONS    "BUTTONS"

class OculusRiftCV1SensorData :
    public SensorData
{
private:
    SigCmn::Vector4 quaternion; // orientation of headset
    SigCmn::Vector3 position;   // position of headset
    std::vector<std::string> buttonStates; // list of currently-pressed buttons on controller devices

public:

    ///@brief Convert sensor information to string.
    ///@param itemsDelim Symbol to split items.
    ///@param keyValueDelim Symbol to split KEY and VALUE.
    ///@param valuesDelim Symbol to split each componet of VALUE. When VALUE is multidimensional data, you use this symbol.
    std::string encodeSensorData(const std::string &itemsDelim = ITEMS_DELIMITER, const std::string &keyValueDelim = KEY_AND_VALUE_DELIMITER, const std::string &valuesDelim = VALUES_DELIMITER) const;

    ///@brief Set to sensor data.
    ///@param Map of sensor data;
    bool setSensorData(const std::map<std::string, std::vector<std::string> > &sensorDataMap);

    ///@brief Set sensor orientation
    void setQuaternion(const float &w, const float &x, const float &y, const float &z);

    ///@brief Set sensor position
    void setPosition(const float &x, const float &y, const float &z);

    ///@brief Set current set of pressed buttons
    void setButtonStates(std::vector<std::string> buttons);

    ///@brief Get sensor orientation
    SigCmn::Vector4 getQuaternion() const;

    ///@brief Get sensor position
    SigCmn::Vector3 getPosition() const;

    ///@brief Get list of currently-pressed buttons
    std::vector<std::string> getButtonStates() const;
};
#endif // SIGVERSE_OCULUS_RIFT_CV1_SENSOR_DATA_H
