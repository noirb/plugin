#include <sigverse/plugin/common/sensor/OculusRiftCV1SensorData.h>
#include <sstream>

///@brief generate message by posture.
std::string OculusRiftCV1SensorData::encodeSensorData(const std::string &itemsDelim, const std::string &keyValueDelim, const std::string &valuesDelim) const
{
    std::vector<std::string> quat_vec;
    std::vector<std::string> pos_vec;
    std::stringstream ssValue;

    ssValue << this->quaternion.w; quat_vec.push_back(ssValue.str()); ssValue.clear(); ssValue.str("");
    ssValue << this->quaternion.x; quat_vec.push_back(ssValue.str()); ssValue.clear(); ssValue.str("");
    ssValue << this->quaternion.y; quat_vec.push_back(ssValue.str()); ssValue.clear(); ssValue.str("");
    ssValue << this->quaternion.z; quat_vec.push_back(ssValue.str());

    pos_vec.push_back(std::to_string(position.x));
    pos_vec.push_back(std::to_string(position.y));
    pos_vec.push_back(std::to_string(position.z));

    std::map<std::string, std::vector<std::string> > messageMap;

    messageMap.insert(make_pair(MSG_KEY_QUATERNION, quat_vec));
    messageMap.insert(make_pair(MSG_KEY_POSITION,   pos_vec));
    messageMap.insert(make_pair(MSG_KEY_BUTTONS,    buttonStates));
    return this->convertMap2Message(messageMap, itemsDelim, keyValueDelim, valuesDelim);
}


bool OculusRiftCV1SensorData::setSensorData(const std::map<std::string, std::vector<std::string> > &sensorDataMap)
{
    std::map<std::string, std::vector<std::string> >::const_iterator it;

    for (it = sensorDataMap.begin(); it != sensorDataMap.end(); it++)
    {
        if ((*it).first == MSG_KEY_QUATERNION)
        {
            if ((*it).second.size() != 4)
            {
                std::cout << "Not quaternion value. Number of value items is " << (*it).second.size() << std::endl;
                return false;
            }

            // Extract quaternion.
            const float qw = (float)atof((*it).second[0].c_str());
            const float qx = (float)atof((*it).second[1].c_str());
            const float qy = (float)atof((*it).second[2].c_str());
            const float qz = (float)atof((*it).second[3].c_str());

            // Set extracted angle to sensor data.
            this->setQuaternion(qw, qx, qy, qz);

            continue;
        }
        else if ((*it).first == MSG_KEY_POSITION)
        {
            if ((*it).second.size() != 3)
            {
                std::cout << MSG_KEY_POSITION << ": Expected to be length 3. Actual length: " << (*it).second.size() << std::endl;
                return false;
            }

            // Extract position
            const float x = (float)atof((*it).second[0].c_str());
            const float y = (float)atof((*it).second[1].c_str());
            const float z = (float)atof((*it).second[2].c_str());

            // Set extracted position
            this->setPosition(x, y, z);

            continue;
        }
        else if ((*it).first == MSG_KEY_BUTTONS)
        {
            this->buttonStates.clear();
            for (size_t i = 0; i < (*it).second.size(); ++i)
            {
                this->buttonStates.push_back((*it).second[i]);
            }
        }
    }

    return true;
}


///@brief Set sensor orientation.
void OculusRiftCV1SensorData::setQuaternion(const float &w, const float &x, const float &y, const float &z)
{
    this->quaternion.w = w;
    this->quaternion.x = x;
    this->quaternion.y = y;
    this->quaternion.z = z;
}

///@brief Set sensor position
void OculusRiftCV1SensorData::setPosition(const float &x, const float &y, const float &z)
{
    this->position.x = x;
    this->position.y = y;
    this->position.z = z;
}

///@brief Set currently-pressed buttons
void OculusRiftCV1SensorData::setButtonStates(std::vector<std::string> buttons)
{
    this->buttonStates.clear();
    for (auto b : buttons)
    {
        this->buttonStates.push_back(b);
    }
}

///@brief Get sensor orientation
SigCmn::Vector4 OculusRiftCV1SensorData::getQuaternion() const
{
    return this->quaternion;
}

///@brief Get sensor position
SigCmn::Vector3 OculusRiftCV1SensorData::getPosition() const
{
    return this->position;
}

///@brief Get list of currently-pressed buttons
std::vector<std::string> OculusRiftCV1SensorData::getButtonStates() const
{
    return this->buttonStates;
}
