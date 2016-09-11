#include <sigverse/plugin/common/sensor/OpenVRSensorData.h>
#include <sstream>

///@brief generate message by posture.
std::string OpenVRSensorData::encodeSensorData(const std::string &itemsDelim, const std::string &keyValueDelim, const std::string &valuesDelim) const
{
    std::map<std::string, std::vector<std::string> > messageMap;
    std::vector<std::string> hmd_quat_vec;
    std::vector<std::string> hmd_pos_vec;
    std::vector<std::string> ctrl_L_quat_vec;
    std::vector<std::string> ctrl_R_quat_vec;
    std::vector<std::string> ctrl_L_pos_vec;
    std::vector<std::string> ctrl_R_pos_vec;

    hmd_quat_vec.push_back(std::to_string(hmd_quaternion.w));
    hmd_quat_vec.push_back(std::to_string(hmd_quaternion.x));
    hmd_quat_vec.push_back(std::to_string(hmd_quaternion.y));
    hmd_quat_vec.push_back(std::to_string(hmd_quaternion.z));

    hmd_pos_vec.push_back(std::to_string(hmd_position.x));
    hmd_pos_vec.push_back(std::to_string(hmd_position.y));
    hmd_pos_vec.push_back(std::to_string(hmd_position.z));

    ctrl_L_quat_vec.push_back(std::to_string(ctrl_L_quaternion.w));
    ctrl_L_quat_vec.push_back(std::to_string(ctrl_L_quaternion.x));
    ctrl_L_quat_vec.push_back(std::to_string(ctrl_L_quaternion.y));
    ctrl_L_quat_vec.push_back(std::to_string(ctrl_L_quaternion.z));
   
    ctrl_L_pos_vec.push_back(std::to_string(ctrl_L_position.x));
    ctrl_L_pos_vec.push_back(std::to_string(ctrl_L_position.y));
    ctrl_L_pos_vec.push_back(std::to_string(ctrl_L_position.z));

    ctrl_R_quat_vec.push_back(std::to_string(ctrl_R_quaternion.w));
    ctrl_R_quat_vec.push_back(std::to_string(ctrl_R_quaternion.x));
    ctrl_R_quat_vec.push_back(std::to_string(ctrl_R_quaternion.y));
    ctrl_R_quat_vec.push_back(std::to_string(ctrl_R_quaternion.z));

    ctrl_R_pos_vec.push_back(std::to_string(ctrl_R_position.x));
    ctrl_R_pos_vec.push_back(std::to_string(ctrl_R_position.y));
    ctrl_R_pos_vec.push_back(std::to_string(ctrl_R_position.z));

    messageMap.insert(make_pair(MSG_KEY_HMD_ROT,             hmd_quat_vec));
    messageMap.insert(make_pair(MSG_KEY_HMD_POS,             hmd_pos_vec));
    messageMap.insert(make_pair(MSG_KEY_CONTROLLER_L_ROT,    ctrl_L_quat_vec));
    messageMap.insert(make_pair(MSG_KEY_CONTROLLER_L_POS,    ctrl_L_pos_vec));
    messageMap.insert(make_pair(MSG_KEY_CONTROLLER_R_ROT,    ctrl_R_quat_vec));
    messageMap.insert(make_pair(MSG_KEY_CONTROLLER_R_POS,    ctrl_R_pos_vec));
    messageMap.insert(make_pair(MSG_KEY_CONTROLLER_L_BTN,    ctrl_L_buttons));
    messageMap.insert(make_pair(MSG_KEY_CONTROLLER_R_BTN,    ctrl_R_buttons));

    return this->convertMap2Message(messageMap, itemsDelim, keyValueDelim, valuesDelim);
}


bool OpenVRSensorData::setSensorData(const std::map<std::string, std::vector<std::string> > &sensorDataMap)
{
    std::map<std::string, std::vector<std::string> >::const_iterator it;

    for (it = sensorDataMap.begin(); it != sensorDataMap.end(); it++)
    {
        if ((*it).first == MSG_KEY_HMD_ROT)
        {
            if ((*it).second.size() != 4)
            {
                std::cout << MSG_KEY_HMD_ROT << ": Expected to be length 4. Actual length: " << (*it).second.size() << std::endl;
                return false;
            }

            // Extract quaternion.
            const float qw = (float)atof((*it).second[0].c_str());
            const float qx = (float)atof((*it).second[1].c_str());
            const float qy = (float)atof((*it).second[2].c_str());
            const float qz = (float)atof((*it).second[3].c_str());

            // Set extracted angle to sensor data.
            this->setQuaternion(HMD, qw, qx, qy, qz);

            continue;
        }
        else if ((*it).first == MSG_KEY_HMD_POS)
        {
            if ((*it).second.size() != 3)
            {
                std::cout << MSG_KEY_HMD_POS << ": Expected to be length 3. Actual length: " << (*it).second.size() << std::endl;
                return false;
            }

            // Extract position
            const float x = (float)atof((*it).second[0].c_str());
            const float y = (float)atof((*it).second[1].c_str());
            const float z = (float)atof((*it).second[2].c_str());

            // Set extracted position
            this->setPosition(HMD, x, y, z);

            continue;
        }
        else if ((*it).first == MSG_KEY_CONTROLLER_L_ROT)
        {
            if ((*it).second.size() != 4)
            {
                std::cout << MSG_KEY_CONTROLLER_L_ROT << ": Expected to be length 4. Actual length: " << (*it).second.size() << std::endl;
                return false;
            }

            // Extract quaternion.
            const float qw = (float)atof((*it).second[0].c_str());
            const float qx = (float)atof((*it).second[1].c_str());
            const float qy = (float)atof((*it).second[2].c_str());
            const float qz = (float)atof((*it).second[3].c_str());

            // Set extracted angle to sensor data.
            this->setQuaternion(Controller_L, qw, qx, qy, qz);

            continue;
        }
        else if ((*it).first == MSG_KEY_CONTROLLER_R_ROT)
        {
            if ((*it).second.size() != 4)
            {
                std::cout << MSG_KEY_CONTROLLER_R_ROT << ": Expected to be length 4. Actual length: " << (*it).second.size() << std::endl;
                return false;
            }

            // Extract quaternion.
            const float qw = (float)atof((*it).second[0].c_str());
            const float qx = (float)atof((*it).second[1].c_str());
            const float qy = (float)atof((*it).second[2].c_str());
            const float qz = (float)atof((*it).second[3].c_str());

            // Set extracted angle to sensor data.
            this->setQuaternion(Controller_R, qw, qx, qy, qz);

            continue;
        }
        else if ((*it).first == MSG_KEY_CONTROLLER_L_POS)
        {
            if ((*it).second.size() != 3)
            {
                std::cout << MSG_KEY_CONTROLLER_L_POS << ": Expected to be length 3. Actual length: " << (*it).second.size() << std::endl;
                return false;
            }

            // Extract position
            const float x = (float)atof((*it).second[0].c_str());
            const float y = (float)atof((*it).second[1].c_str());
            const float z = (float)atof((*it).second[2].c_str());

            // Set extracted position
            this->setPosition(Controller_L, x, y, z);

            continue;
        }
        else if ((*it).first == MSG_KEY_CONTROLLER_R_POS)
        {
            if ((*it).second.size() != 3)
            {
                std::cout << MSG_KEY_CONTROLLER_R_POS << ": Expected to be length 3. Actual length: " << (*it).second.size() << std::endl;
                return false;
            }

            // Extract position
            const float x = (float)atof((*it).second[0].c_str());
            const float y = (float)atof((*it).second[1].c_str());
            const float z = (float)atof((*it).second[2].c_str());

            // Set extracted position
            this->setPosition(Controller_R, x, y, z);

            continue;
        }
        else if ((*it).first == MSG_KEY_CONTROLLER_L_BTN)
        {
            this->ctrl_L_buttons.clear();
            for (size_t i = 0; i < (*it).second.size(); ++i)
            {
                this->ctrl_L_buttons.push_back((*it).second[i]);
            }
        }
        else if ((*it).first == MSG_KEY_CONTROLLER_R_BTN)
        {
            this->ctrl_R_buttons.clear();
            for (size_t i = 0; i < (*it).second.size(); ++i)
            {
                this->ctrl_R_buttons.push_back((*it).second[i]);
            }
        }
    }

    return true;
}


///@brief Set sensor orientation.
void OpenVRSensorData::setQuaternion(DeviceType device, SigCmn::Vector4 quat)
{
    switch (device)
    {
        case HMD:
        {
            this->hmd_quaternion = quat;
            break;
        }
        case Controller_L:
        {
            this->ctrl_L_quaternion = quat;
            break;
        }
        case Controller_R:
        {
            this->ctrl_R_quaternion = quat;
            break;
        }
    }
}

///@brief Set sensor orientation.
void OpenVRSensorData::setQuaternion(DeviceType device, const float &w, const float &x, const float &y, const float &z)
{
    switch (device)
    {
        case HMD:
        {
            this->hmd_quaternion.w = w;
            this->hmd_quaternion.x = x;
            this->hmd_quaternion.y = y;
            this->hmd_quaternion.z = z;
            break;
        }
        case Controller_L:
        {
            this->ctrl_L_quaternion.w = w;
            this->ctrl_L_quaternion.x = x;
            this->ctrl_L_quaternion.y = y;
            this->ctrl_L_quaternion.z = z;
            break;
        }
        case Controller_R:
        {
            this->ctrl_R_quaternion.w = w;
            this->ctrl_R_quaternion.x = x;
            this->ctrl_R_quaternion.y = y;
            this->ctrl_R_quaternion.z = z;
            break;
        }
    }
}

///@brief Set sensor position
void OpenVRSensorData::setPosition(DeviceType device, const float &x, const float &y, const float &z)
{
    switch (device)
    {
        case HMD:
        {
            this->hmd_position.x = x;
            this->hmd_position.y = y;
            this->hmd_position.z = z;
            break;
        }
        case Controller_L:
        {
            this->ctrl_L_position.x = x;
            this->ctrl_L_position.y = y;
            this->ctrl_L_position.z = z;
            break;
        }
        case Controller_R:
        {
            this->ctrl_R_position.x = x;
            this->ctrl_R_position.y = y;
            this->ctrl_R_position.z = z;
            break;
        }
    }
}

///@brief Set currently-pressed buttons
void OpenVRSensorData::setButtonStates(DeviceType device, std::vector<std::string> buttons)
{
    switch (device)
    {
        case HMD:
            break;
        case Controller_L:
        {
            this->ctrl_L_buttons.clear();
            for (auto b : buttons)
            {
                this->ctrl_L_buttons.push_back(b);
            }
            break;
        }
        case Controller_R:
        {
            this->ctrl_R_buttons.clear();
            for (auto b : buttons)
            {
                this->ctrl_R_buttons.push_back(b);
            }
            break;
        }
    }
}

///@brief Get sensor orientation
SigCmn::Vector4 OpenVRSensorData::getQuaternion(DeviceType device) const
{
    switch (device)
    {
        case HMD:
            return this->hmd_quaternion;
        case Controller_L:
            return this->ctrl_L_quaternion;
        case Controller_R:
            return this->ctrl_R_quaternion;
    }
}

///@brief Get sensor position
SigCmn::Vector3 OpenVRSensorData::getPosition(DeviceType device) const
{
    switch (device)
    {
        case HMD:
            return this->hmd_position;
        case Controller_L:
            return this->ctrl_L_position;
        case Controller_R:
            return this->ctrl_L_position;
    }
}

///@brief Get list of currently-pressed buttons
std::vector<std::string> OpenVRSensorData::getButtonStates(DeviceType device) const
{
    switch (device)
    {
        case HMD:
            return std::vector<std::string>();
        case Controller_L:
            return this->ctrl_L_buttons;
        case Controller_R:
            return this->ctrl_R_buttons;
    }
}
