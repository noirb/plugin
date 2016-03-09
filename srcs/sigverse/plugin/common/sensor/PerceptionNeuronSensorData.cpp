#include <map>
#include <sigverse/plugin/common/sensor/PerceptionNeuronSensorData.h>

///@brief Constructor.
PerceptionNeuronSensorData::PerceptionNeuronSensorData()
{
	// Make a map for translating Enum to string.
	this->mapMsgItemEnum2Str.insert(std::map<MsgItemEnum, std::string>::value_type(AvatarIndex,   "AVATAR_INDEX"));
	this->mapMsgItemEnum2Str.insert(std::map<MsgItemEnum, std::string>::value_type(AvatarName,    "AVATAR_NAME"));
	this->mapMsgItemEnum2Str.insert(std::map<MsgItemEnum, std::string>::value_type(WithDisp,      "WITH_DISP"));
	this->mapMsgItemEnum2Str.insert(std::map<MsgItemEnum, std::string>::value_type(WithReference, "WITH_REFERENCE"));
	this->mapMsgItemEnum2Str.insert(std::map<MsgItemEnum, std::string>::value_type(FrameIndex,    "FRAME_INDEX"));
	this->mapMsgItemEnum2Str.insert(std::map<MsgItemEnum, std::string>::value_type(DataCount,     "DATA_COUNT"));
	this->mapMsgItemEnum2Str.insert(std::map<MsgItemEnum, std::string>::value_type(Data,          "DATA"));

	// Make a map for translating string to Enum.
	std::map<MsgItemEnum, std::string>::const_iterator it;

	for (it = this->mapMsgItemEnum2Str.begin(); it != this->mapMsgItemEnum2Str.end(); it++)
	{
		this->mapMsgItemStr2Enum.insert(std::map<std::string, MsgItemEnum>::value_type((*it).second, (*it).first));
	}
}


///@brief Generate a message string by posture.
std::string PerceptionNeuronSensorData::encodeSensorData(const std::string &itemsDelim, const std::string &keyValueDelim, const std::string &valuesDelim) const
{
	std::stringstream ss;

	ss  << this->mapMsgItemEnum2Str.at(AvatarIndex)   << keyValueDelim << this->bvhData.avatarIndex   << itemsDelim
		<< this->mapMsgItemEnum2Str.at(AvatarName)    << keyValueDelim << this->bvhData.avatarName    << itemsDelim
		<< this->mapMsgItemEnum2Str.at(WithDisp)      << keyValueDelim << this->bvhData.withDisp      << itemsDelim
		<< this->mapMsgItemEnum2Str.at(WithReference) << keyValueDelim << this->bvhData.withReference << itemsDelim
		<< this->mapMsgItemEnum2Str.at(FrameIndex)    << keyValueDelim << this->bvhData.frameIndex    << itemsDelim
		<< this->mapMsgItemEnum2Str.at(DataCount)     << keyValueDelim << this->bvhData.dataCount     << itemsDelim
		<< this->mapMsgItemEnum2Str.at(Data)          << keyValueDelim << this->getDataString();

	return(ss.str());
}


///@brief Get data string for sending message.
std::string PerceptionNeuronSensorData::getDataString(const std::string &valuesDelim) const
{
	std::stringstream ss;

	for (int i = 0; i < this->bvhData.dataCount; i++)
	{
		if (i != 0) { ss << valuesDelim; }

		ss << this->bvhData.data[i];
	}

	return ss.str();
}


///@brief Set to sensor data.
///@param Map of sensor data;
bool PerceptionNeuronSensorData::setSensorData(const std::map<std::string, std::vector<std::string> > &sensorDataMap)
{
	std::map<std::string, std::vector<std::string> >::const_iterator it;

	// For Header.
	for (it = sensorDataMap.begin(); it != sensorDataMap.end(); it++)
	{
		if((*it).first==MSG_KEY_DEV_TYPE)     { continue; }
		if((*it).first==MSG_KEY_DEV_UNIQUE_ID){ continue; }

		MsgItemEnum bvhItemEnum = this->mapMsgItemStr2Enum.at((*it).first);

		switch (bvhItemEnum)
		{
			case AvatarIndex:
			{
				this->bvhData.avatarIndex = std::stoi((*it).second[0]);
				break;
			}
			case AvatarName:
			{
				this->bvhData.avatarName = (*it).second[0];
				break;
			}
			case WithDisp:
			{
				this->bvhData.withDisp = (std::stoi((*it).second[0]) == 1);
				break; 
			}
			case WithReference:
			{
				this->bvhData.withReference = (std::stoi((*it).second[0]) == 1);
				break; 
			}
			case FrameIndex:
			{
				this->bvhData.frameIndex = std::stoi((*it).second[0]);
				break;
			}
			case DataCount:
			{
				this->bvhData.dataCount = std::stoi((*it).second[0]);
				break;
			}
			default :
			{
				break;
			}
		}
	}

	// For Data (Joint rotations).
	return this->setSensorDataJointRotation(sensorDataMap);
}


///@brief Set to sensor data (Joint rotations).
///@param Map of sensor data (Joint rotations);
bool PerceptionNeuronSensorData::setSensorDataJointRotation(const std::map<std::string, std::vector<std::string> > &sensorDataMap)
{
	std::string keyStrData = this->mapMsgItemEnum2Str.at(MsgItemEnum::Data);

	if (sensorDataMap.find(keyStrData) != sensorDataMap.end())
	{
		std::vector<std::string> dataList = sensorDataMap.at(keyStrData);

		for(int i = 0; i < BVHBonesTypeCount; ++i)
		{
			int dataIndex = 0;

			if (this->bvhData.withDisp)
			{
				dataIndex = i * 6;

				if (this->bvhData.withReference)
				{
					dataIndex += 6;
				}

				// Hips has a position.
				if (i == BVHBonesType::Hips)
				{
					this->rootPosition.x = std::stof(dataList[dataIndex + 0]);
					this->rootPosition.y = std::stof(dataList[dataIndex + 1]);
					this->rootPosition.z = std::stof(dataList[dataIndex + 2]);
				}

				this->jointRotations[i].jointType  = (BVHBonesType)i;
				this->jointRotations[i].rotation.x = std::stof(dataList[dataIndex + 3]);
				this->jointRotations[i].rotation.y = std::stof(dataList[dataIndex + 4]);
				this->jointRotations[i].rotation.z = std::stof(dataList[dataIndex + 5]);
			}
			else
			{
				if (i == BVHBonesType::Hips)
				{
					dataIndex = 0;

					if (this->bvhData.withReference)
					{
						dataIndex += 6;
					}

					// Hips has a position.
					this->rootPosition.x = std::stof(dataList[dataIndex + 0]);
					this->rootPosition.y = std::stof(dataList[dataIndex + 1]);
					this->rootPosition.z = std::stof(dataList[dataIndex + 2]);

					this->jointRotations[i].jointType  = (BVHBonesType)i;
					this->jointRotations[i].rotation.x = std::stof(dataList[dataIndex + 3]);
					this->jointRotations[i].rotation.y = std::stof(dataList[dataIndex + 4]);
					this->jointRotations[i].rotation.z = std::stof(dataList[dataIndex + 5]);
				}
				else
				{
					dataIndex = 3 + i * 3;

					if (this->bvhData.withReference)
					{
						dataIndex += 6;
					}

					this->jointRotations[i].jointType  = (BVHBonesType)i;
					this->jointRotations[i].rotation.x = std::stof(dataList[dataIndex + 0]);
					this->jointRotations[i].rotation.y = std::stof(dataList[dataIndex + 1]);
					this->jointRotations[i].rotation.z = std::stof(dataList[dataIndex + 2]);
				}
			}
		}
	}

	return true;
}

