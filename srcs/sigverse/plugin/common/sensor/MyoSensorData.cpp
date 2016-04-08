#include <map>
#include <sigverse/plugin/common/sensor/MyoSensorData.h>

///@brief Constructor.
MyoSensorData::MyoSensorData()
{
	// Make a map for translating Enum to string.
	this->mapMsgItemEnum2Str.insert(std::map<MsgItemEnum, std::string>::value_type(Pose,        "POSE"));
	this->mapMsgItemEnum2Str.insert(std::map<MsgItemEnum, std::string>::value_type(Orientation, "ORIENTATION"));
	this->mapMsgItemEnum2Str.insert(std::map<MsgItemEnum, std::string>::value_type(EmgData,     "EMG_DATA"));

	// Make a map for translating string to Enum.
	std::map<MsgItemEnum, std::string>::const_iterator it;

	for (it = this->mapMsgItemEnum2Str.begin(); it != this->mapMsgItemEnum2Str.end(); it++)
	{
		this->mapMsgItemStr2Enum.insert(std::map<std::string, MsgItemEnum>::value_type((*it).second, (*it).first));
	}
}


///@brief Generate a message string by posture.
std::string MyoSensorData::encodeSensorData(const std::string &itemsDelim, const std::string &keyValueDelim, const std::string &valuesDelim) const
{
	std::stringstream ss;

	ss << this->mapMsgItemEnum2Str.at(Pose) << keyValueDelim << this->poseStr << itemsDelim
		<< this->mapMsgItemEnum2Str.at(Orientation) << keyValueDelim << this->roll << valuesDelim << this->pitch << valuesDelim << this->yaw << itemsDelim
		<< this->mapMsgItemEnum2Str.at(EmgData) << keyValueDelim << this->getEmgDataString(this->emgData);

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

//
/////@brief Set to sensor data.
/////@param Map of sensor data;
//bool MyoSensorData::setSensorData(const std::map<std::string, std::vector<std::string> > &sensorDataMap)
//{
//	if (sensorDataMap.find(this->mapMsgItemEnum2Str.at(DataType)) != sensorDataMap.end())
//	{
//		std::string dataType = sensorDataMap.at(this->mapMsgItemEnum2Str.at(DataType))[0];
//
//		return this->setSensorData4Bvh(sensorDataMap);
//	}
//	else
//	{
//		std::cout << "DataType does not exits.(in setSensorData) " << std::endl;
//		exit(-1);
//	}
//}
//
//
/////@brief Set to sensor data (for BVH).
/////@param Map of sensor data;
//bool MyoSensorData::setSensorData4Bvh(const std::map<std::string, std::vector<std::string> > &sensorDataMap)
//{
//	std::map<std::string, std::vector<std::string> >::const_iterator it;
//
//	// For Header.
//	for (it = sensorDataMap.begin(); it != sensorDataMap.end(); it++)
//	{
//		if((*it).first==MSG_KEY_DEV_TYPE)     { continue; }
//		if((*it).first==MSG_KEY_DEV_UNIQUE_ID){ continue; }
//
//		MsgItemEnum itemEnum = this->mapMsgItemStr2Enum.at((*it).first);
//
//		switch (itemEnum)
//		{
//			case AvatarIndex:
//			{
//				this->bvhData.avatarIndex = std::stoi((*it).second[0]);
//				break;
//			}
//			case AvatarName:
//			{
//				this->bvhData.avatarName = (*it).second[0];
//				break;
//			}
//			case WithDisp:
//			{
//				this->bvhData.withDisp = (std::stoi((*it).second[0]) == 1);
//				break; 
//			}
//			case WithReference:
//			{
//				this->bvhData.withReference = (std::stoi((*it).second[0]) == 1);
//				break; 
//			}
//			case FrameIndex:
//			{
//				this->bvhData.frameIndex = std::stoi((*it).second[0]);
//				break;
//			}
//			case DataCount:
//			{
//				this->bvhData.dataCount = std::stoi((*it).second[0]);
//				break;
//			}
//			default :
//			{
//				break;
//			}
//		}
//	}
//
//	// For Data
//	std::string keyStrData = this->mapMsgItemEnum2Str.at(MsgItemEnum::Data);
//
//	if (sensorDataMap.find(keyStrData) != sensorDataMap.end())
//	{
//		std::vector<std::string> dataList = sensorDataMap.at(keyStrData);
//
//	}
//
//	return true;
//}


