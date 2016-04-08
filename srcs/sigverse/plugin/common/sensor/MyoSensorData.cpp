#include <map>
#include <sigverse/plugin/common/sensor/MyoSensorData.h>

std::map<MyoSensorData::MsgItemEnum, std::string> MyoSensorData::mapMsgItemEnum2Str;
std::map<std::string, MyoSensorData::MsgItemEnum> MyoSensorData::mapMsgItemStr2Enum;
std::map<MyoSensorData::PoseType, std::string> MyoSensorData::mapPoseType2Str;

///@brief Constructor.
MyoSensorData::MyoSensorData()
{
	if(MyoSensorData::mapMsgItemEnum2Str.size()==0)
	{
		// Make a map for translating Enum to string.
		MyoSensorData::mapMsgItemEnum2Str.insert(std::map<MsgItemEnum, std::string>::value_type(Pose,        "POSE"));
		MyoSensorData::mapMsgItemEnum2Str.insert(std::map<MsgItemEnum, std::string>::value_type(Orientation, "ORIENTATION"));
		MyoSensorData::mapMsgItemEnum2Str.insert(std::map<MsgItemEnum, std::string>::value_type(EmgData,     "EMG_DATA"));

		// Make a map for translating string to Enum.
		std::map<MsgItemEnum, std::string>::const_iterator it;

		for (it = MyoSensorData::mapMsgItemEnum2Str.begin(); it != MyoSensorData::mapMsgItemEnum2Str.end(); it++)
		{
			MyoSensorData::mapMsgItemStr2Enum.insert(std::map<std::string, MsgItemEnum>::value_type((*it).second, (*it).first));
		}

		MyoSensorData::mapPoseType2Str.insert(std::map<PoseType, std::string>::value_type(Rest,          "rest"));
		MyoSensorData::mapPoseType2Str.insert(std::map<PoseType, std::string>::value_type(Fist,          "fist"));
		MyoSensorData::mapPoseType2Str.insert(std::map<PoseType, std::string>::value_type(WaveIn,        "waveIn"));
		MyoSensorData::mapPoseType2Str.insert(std::map<PoseType, std::string>::value_type(WaveOut,       "waveOut"));
		MyoSensorData::mapPoseType2Str.insert(std::map<PoseType, std::string>::value_type(FingersSpread, "fingersSpread"));
		MyoSensorData::mapPoseType2Str.insert(std::map<PoseType, std::string>::value_type(DoubleTap,     "doubleTap"));
		MyoSensorData::mapPoseType2Str.insert(std::map<PoseType, std::string>::value_type(Unknown,       "unknown"));
	}

	this->roll  = 0.0;
	this->pitch = 0.0;
	this->yaw   = 0.0;
	this->emgData.resize(EMG_SENSOR_NUM);
}


///@brief Generate a message string by posture.
std::string MyoSensorData::encodeSensorData(const std::string &itemsDelim, const std::string &keyValueDelim, const std::string &valuesDelim) const
{
	std::stringstream ss;

	ss  << MyoSensorData::mapMsgItemEnum2Str.at(Orientation) << keyValueDelim << this->roll << valuesDelim << this->pitch << valuesDelim << this->yaw << itemsDelim
		<< MyoSensorData::mapMsgItemEnum2Str.at(Pose) << keyValueDelim << this->poseStr << itemsDelim
		<< MyoSensorData::mapMsgItemEnum2Str.at(EmgData) << keyValueDelim << this->getEmgDataString(this->emgData);

	return(ss.str());
}


///@brief Get a Emg data string for sending message.
std::string MyoSensorData::getEmgDataString(const std::vector<int> &data, const std::string &valuesDelim) const
{
	std::stringstream ss;

	for (size_t i = 0; i < data.size(); i++)
	{
		if (i != 0) { ss << valuesDelim; }

		ss << data[i];
	}

	return ss.str();
}


///@brief Set to sensor data.
///@param Map of sensor data;
bool MyoSensorData::setSensorData(const std::map<std::string, std::vector<std::string> > &sensorDataMap)
{
	std::map<std::string, std::vector<std::string> >::const_iterator it;

	for (it = sensorDataMap.begin(); it != sensorDataMap.end(); it++)
	{
		if((*it).first==MSG_KEY_DEV_TYPE)     { continue; }
		if((*it).first==MSG_KEY_DEV_UNIQUE_ID){ continue; }

		MsgItemEnum itemEnum = this->mapMsgItemStr2Enum.at((*it).first);

		switch (itemEnum)
		{
			case Orientation:
			{
				this->roll  = std::stof((*it).second[0]);
				this->pitch = std::stof((*it).second[1]);
				this->yaw   = std::stof((*it).second[2]);
				break;
			}
			case Pose:
			{
				this->poseStr = (*it).second[0];
				break;
			}
			case EmgData:
			{
				for(int i=0; i<EMG_SENSOR_NUM; i++)
				{
					this->emgData[i] = std::stoi((*it).second[i]);
				}
				break;
			}
			default :
			{
				return false;
			}
		}
	}

	return true;
}

