#include <map>
#include <sigverse/plugin/common/sensor/PerceptionNeuronSensorData.h>

//PerceptionNeuronSensorData::SensorDataMode PerceptionNeuronSensorData::sensorDataMode;

///@brief Constructor.
PerceptionNeuronSensorData::PerceptionNeuronSensorData()
{
	// Make Enum list.
	this->mapItemEnum2Str.insert(std::map<BvhItemEnum, std::string>::value_type(AvatarIndex,   "AVATAR_INDEX"));
	this->mapItemEnum2Str.insert(std::map<BvhItemEnum, std::string>::value_type(AvatarName,    "AVATAR_NAME"));
	this->mapItemEnum2Str.insert(std::map<BvhItemEnum, std::string>::value_type(WithDisp,      "WITH_DISP"));
	this->mapItemEnum2Str.insert(std::map<BvhItemEnum, std::string>::value_type(WithReference, "WITH_REFERENCE"));
	this->mapItemEnum2Str.insert(std::map<BvhItemEnum, std::string>::value_type(FrameIndex,    "FRAME_INDEX"));
	this->mapItemEnum2Str.insert(std::map<BvhItemEnum, std::string>::value_type(DataCount,     "DATA_COUNT"));
	this->mapItemEnum2Str.insert(std::map<BvhItemEnum, std::string>::value_type(Data,          "DATA"));

	std::map<BvhItemEnum, std::string>::const_iterator it;

	for (it = this->mapItemEnum2Str.begin(); it != this->mapItemEnum2Str.end(); it++)
	{
		this->mapItemStr2Enum.insert(std::map<std::string, BvhItemEnum>::value_type((*it).second, (*it).first));
	}
}


/// @brief Return the name of BVH item.
std::string PerceptionNeuronSensorData::getBvhItemName(BvhItemEnum bvhItemEnum) const
{
	return this->mapItemEnum2Str.at(bvhItemEnum);
}

/// @brief 
PerceptionNeuronSensorData::BvhItemEnum PerceptionNeuronSensorData::getBvhItemEnum(std::string itemName) const
{
	return this->mapItemStr2Enum.at(itemName);
}



///@brief Generate message by posture.
std::string PerceptionNeuronSensorData::encodeSensorData(const std::string &itemsDelim, const std::string &keyValueDelim, const std::string &valuesDelim) const
{
	std::stringstream ss;

	ss  << this->mapItemEnum2Str.at(AvatarIndex)   << keyValueDelim << this->bvhData.avatarIndex   << itemsDelim
		<< this->mapItemEnum2Str.at(AvatarName)    << keyValueDelim << this->bvhData.avatarName    << itemsDelim
		<< this->mapItemEnum2Str.at(WithDisp)      << keyValueDelim << this->bvhData.withDisp      << itemsDelim
		<< this->mapItemEnum2Str.at(WithReference) << keyValueDelim << this->bvhData.withReference << itemsDelim
		<< this->mapItemEnum2Str.at(FrameIndex)    << keyValueDelim << this->bvhData.frameIndex    << itemsDelim
		<< this->mapItemEnum2Str.at(DataCount)     << keyValueDelim << this->bvhData.dataCount     << itemsDelim
		<< this->mapItemEnum2Str.at(Data)          << keyValueDelim << this->getDataString();

	return(ss.str());
}


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
	// For Header.
	std::map<std::string, std::vector<std::string> >::const_iterator it;

	for (it = sensorDataMap.begin(); it != sensorDataMap.end(); it++)
	{
		if((*it).first==MSG_KEY_DEV_TYPE)     { continue; }
		if((*it).first==MSG_KEY_DEV_UNIQUE_ID){ continue; }

		BvhItemEnum bvhItemEnum = this->mapItemStr2Enum.at((*it).first);

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

	this->setSensorDataJointRotation(sensorDataMap);

	return true;
}


///@brief Set to sensor data.
///@param Map of sensor data;
bool PerceptionNeuronSensorData::setSensorDataJointRotation(const std::map<std::string, std::vector<std::string> > &sensorDataMap)
{
	// For Data.
	std::string keyStrData = this->mapItemEnum2Str.at(BvhItemEnum::Data);

	if (sensorDataMap.find(keyStrData) != sensorDataMap.end())
	{
		std::vector<std::string> dataList = sensorDataMap.at(keyStrData);

		for(int i = 0; i < BVHBonesTypeCount; ++i)
		{
			// calculate data index for selected bone
			int dataIndex = 0;

			if (this->bvhData.withDisp)
			{
				dataIndex = i * 6;

				if (this->bvhData.withReference)
				{
					dataIndex += 6;
				}

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
}

