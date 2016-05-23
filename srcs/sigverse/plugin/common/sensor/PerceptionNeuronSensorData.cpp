#include <map>
#include <sigverse/plugin/common/sensor/PerceptionNeuronSensorData.h>

std::map<PerceptionNeuronSensorData::MsgItemEnum, std::string> PerceptionNeuronSensorData::mapMsgItemEnum2Str;
std::map<std::string, PerceptionNeuronSensorData::MsgItemEnum> PerceptionNeuronSensorData::mapMsgItemStr2Enum;

///@brief Constructor.
PerceptionNeuronSensorData::PerceptionNeuronSensorData()
{
	if(PerceptionNeuronSensorData::mapMsgItemEnum2Str.size()==0)
	{
		// Make a map for translating Enum to string.
		PerceptionNeuronSensorData::mapMsgItemEnum2Str.insert(std::map<MsgItemEnum, std::string>::value_type(DataType,      "DATA_TYPE"));
		PerceptionNeuronSensorData::mapMsgItemEnum2Str.insert(std::map<MsgItemEnum, std::string>::value_type(AvatarIndex,   "AVATAR_INDEX"));
		PerceptionNeuronSensorData::mapMsgItemEnum2Str.insert(std::map<MsgItemEnum, std::string>::value_type(AvatarName,    "AVATAR_NAME"));
		PerceptionNeuronSensorData::mapMsgItemEnum2Str.insert(std::map<MsgItemEnum, std::string>::value_type(WithDisp,      "WITH_DISP"));
		PerceptionNeuronSensorData::mapMsgItemEnum2Str.insert(std::map<MsgItemEnum, std::string>::value_type(WithReference, "WITH_REFERENCE"));
		PerceptionNeuronSensorData::mapMsgItemEnum2Str.insert(std::map<MsgItemEnum, std::string>::value_type(FrameIndex,    "FRAME_INDEX"));
		PerceptionNeuronSensorData::mapMsgItemEnum2Str.insert(std::map<MsgItemEnum, std::string>::value_type(DataCount,     "DATA_COUNT"));
		PerceptionNeuronSensorData::mapMsgItemEnum2Str.insert(std::map<MsgItemEnum, std::string>::value_type(Data,          "DATA"));

		// Make a map for translating string to Enum.
		std::map<MsgItemEnum, std::string>::const_iterator it;

		for (it = PerceptionNeuronSensorData::mapMsgItemEnum2Str.begin(); it != PerceptionNeuronSensorData::mapMsgItemEnum2Str.end(); it++)
		{
			PerceptionNeuronSensorData::mapMsgItemStr2Enum.insert(std::map<std::string, MsgItemEnum>::value_type((*it).second, (*it).first));
		}
	}

	this->dataType = DataTypeEnum::BVH;
}


///@brief Generate a message string by posture.
std::string PerceptionNeuronSensorData::encodeSensorData(const std::string &itemsDelim, const std::string &keyValueDelim, const std::string &valuesDelim) const
{
	std::stringstream ss;

	if (this->dataType == DataTypeEnum::BVH)
	{
		ss  << PerceptionNeuronSensorData::mapMsgItemEnum2Str.at(DataType)      << keyValueDelim << STR(BVH)                    << itemsDelim
			<< PerceptionNeuronSensorData::mapMsgItemEnum2Str.at(AvatarIndex)   << keyValueDelim << this->bvhData.avatarIndex   << itemsDelim
			<< PerceptionNeuronSensorData::mapMsgItemEnum2Str.at(AvatarName)    << keyValueDelim << this->bvhData.avatarName    << itemsDelim
			<< PerceptionNeuronSensorData::mapMsgItemEnum2Str.at(WithDisp)      << keyValueDelim << this->bvhData.withDisp      << itemsDelim
			<< PerceptionNeuronSensorData::mapMsgItemEnum2Str.at(WithReference) << keyValueDelim << this->bvhData.withReference << itemsDelim
			<< PerceptionNeuronSensorData::mapMsgItemEnum2Str.at(FrameIndex)    << keyValueDelim << this->bvhData.frameIndex    << itemsDelim
			<< PerceptionNeuronSensorData::mapMsgItemEnum2Str.at(DataCount)     << keyValueDelim << this->bvhData.dataCount     << itemsDelim
			<< PerceptionNeuronSensorData::mapMsgItemEnum2Str.at(Data)          << keyValueDelim << this->getDataString(this->bvhData.dataCount, this->bvhData.data);
	}
	else if(this->dataType == DataTypeEnum::CALC)
	{
		ss  << PerceptionNeuronSensorData::mapMsgItemEnum2Str.at(DataType)      << keyValueDelim << STR(CALC)                    << itemsDelim
			<< PerceptionNeuronSensorData::mapMsgItemEnum2Str.at(AvatarIndex)   << keyValueDelim << this->calcData.avatarIndex   << itemsDelim
			<< PerceptionNeuronSensorData::mapMsgItemEnum2Str.at(AvatarName)    << keyValueDelim << this->calcData.avatarName    << itemsDelim
			<< PerceptionNeuronSensorData::mapMsgItemEnum2Str.at(FrameIndex)    << keyValueDelim << this->calcData.frameIndex    << itemsDelim
			<< PerceptionNeuronSensorData::mapMsgItemEnum2Str.at(DataCount)     << keyValueDelim << this->calcData.dataCount     << itemsDelim
			<< PerceptionNeuronSensorData::mapMsgItemEnum2Str.at(Data)          << keyValueDelim << this->getDataString(this->calcData.dataCount, this->calcData.data);
	}

	return(ss.str());
}


///@brief Get data string for sending message.
std::string PerceptionNeuronSensorData::getDataString(const unsigned short dataCount, const float *data, const std::string &valuesDelim) const
{
	std::stringstream ss;

	for (int i = 0; i < dataCount; i++)
	{
		if (i != 0) { ss << valuesDelim; }

		ss << data[i];
	}

	return ss.str();
}


///@brief Set data type(BVH or Calculation).
void PerceptionNeuronSensorData::setDataType(DataTypeEnum dataTypeEnum)
{
	this->dataType = dataTypeEnum;
}


///@brief Set to sensor data.
///@param Map of sensor data;
bool PerceptionNeuronSensorData::setSensorData(const std::map<std::string, std::vector<std::string> > &sensorDataMap)
{
	if (sensorDataMap.find(PerceptionNeuronSensorData::mapMsgItemEnum2Str.at(DataType)) != sensorDataMap.end())
	{
		std::string dataType = sensorDataMap.at(PerceptionNeuronSensorData::mapMsgItemEnum2Str.at(DataType))[0];

		if(dataType==STR(BVH))
		{
			this->dataType = DataTypeEnum::BVH;
			return this->setSensorData4Bvh(sensorDataMap);
		}
		else if (dataType==STR(CALC))
		{
			this->dataType = DataTypeEnum::CALC;
			return this->setSensorData4Calc(sensorDataMap);
		}
		else
		{
			std::cout << "Illegal Data type. dataType=" << dataType << std::endl;
			exit(-1);
		}
	}
	else
	{
		std::cout << "DataType does not exits.(in setSensorData) " << std::endl;
		exit(-1);
	}

}


///@brief Set to sensor data (for BVH).
///@param Map of sensor data;
bool PerceptionNeuronSensorData::setSensorData4Bvh(const std::map<std::string, std::vector<std::string> > &sensorDataMap)
{
	std::map<std::string, std::vector<std::string> >::const_iterator it;

	// For Header.
	for (it = sensorDataMap.begin(); it != sensorDataMap.end(); it++)
	{
		if((*it).first==MSG_KEY_DEV_TYPE)     { continue; }
		if((*it).first==MSG_KEY_DEV_UNIQUE_ID){ continue; }

		MsgItemEnum itemEnum = PerceptionNeuronSensorData::mapMsgItemStr2Enum.at((*it).first);

		switch (itemEnum)
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

	// For Data
	std::string keyStrData = PerceptionNeuronSensorData::mapMsgItemEnum2Str.at(MsgItemEnum::Data);

	if (sensorDataMap.find(keyStrData) != sensorDataMap.end())
	{
		std::vector<std::string> dataList = sensorDataMap.at(keyStrData);

		for(int i = 0; i < NeuronBVH::BonesTypeCount; ++i)
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
				if (i == NeuronBVH::BonesType::Hips)
				{
					this->rootPosition.x = std::stof(dataList[dataIndex + 0]);
					this->rootPosition.y = std::stof(dataList[dataIndex + 1]);
					this->rootPosition.z = std::stof(dataList[dataIndex + 2]);
				}

				this->bvhJoints[i].jointType  = (NeuronBVH::BonesType)i;
				this->bvhJoints[i].rotation.y = std::stof(dataList[dataIndex + 3]);
				this->bvhJoints[i].rotation.x = std::stof(dataList[dataIndex + 4]);
				this->bvhJoints[i].rotation.z = std::stof(dataList[dataIndex + 5]);
			}
			else
			{
				if (i == NeuronBVH::BonesType::Hips)
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

					this->bvhJoints[i].jointType  = (NeuronBVH::BonesType)i;
					this->bvhJoints[i].rotation.y = std::stof(dataList[dataIndex + 3]);
					this->bvhJoints[i].rotation.x = std::stof(dataList[dataIndex + 4]);
					this->bvhJoints[i].rotation.z = std::stof(dataList[dataIndex + 5]);
				}
				else
				{
					dataIndex = 3 + i * 3;

					if (this->bvhData.withReference)
					{
						dataIndex += 6;
					}

					this->bvhJoints[i].jointType  = (NeuronBVH::BonesType)i;
					this->bvhJoints[i].rotation.y = std::stof(dataList[dataIndex + 0]);
					this->bvhJoints[i].rotation.x = std::stof(dataList[dataIndex + 1]);
					this->bvhJoints[i].rotation.z = std::stof(dataList[dataIndex + 2]);
				}
			}
		}
	}

	return true;
}




///@brief Set to sensor data (for Calculation).
///@param Map of sensor data;
bool PerceptionNeuronSensorData::setSensorData4Calc(const std::map<std::string, std::vector<std::string> > &sensorDataMap)
{
	std::map<std::string, std::vector<std::string> >::const_iterator it;

	// For Header.
	for (it = sensorDataMap.begin(); it != sensorDataMap.end(); it++)
	{
		if((*it).first==MSG_KEY_DEV_TYPE)     { continue; }
		if((*it).first==MSG_KEY_DEV_UNIQUE_ID){ continue; }

		MsgItemEnum itemEnum = PerceptionNeuronSensorData::mapMsgItemStr2Enum.at((*it).first);

		switch (itemEnum)
		{
			case AvatarIndex:
			{
				this->calcData.avatarIndex = std::stoi((*it).second[0]);
				break;
			}
			case AvatarName:
			{
				this->calcData.avatarName = (*it).second[0];
				break;
			}
			case FrameIndex:
			{
				this->calcData.frameIndex = std::stoi((*it).second[0]);
				break;
			}
			case DataCount:
			{
				this->calcData.dataCount = std::stoi((*it).second[0]);
				break;
			}
			default :
			{
				break;
			}
		}
	}

	// For Data
	std::string keyStrData = PerceptionNeuronSensorData::mapMsgItemEnum2Str.at(MsgItemEnum::Data);

	if (sensorDataMap.find(keyStrData) != sensorDataMap.end())
	{
		std::vector<std::string> dataList = sensorDataMap.at(keyStrData);

		for(int i = 0; i < NeuronCalc::BonesTypeCount; ++i)
		{
			if (i == NeuronCalc::BonesType::Hips)
			{
				this->rootPosition.x = std::stof(dataList[i*16 + 0]);
				this->rootPosition.y = std::stof(dataList[i*16 + 1]);
				this->rootPosition.z = std::stof(dataList[i*16 + 2]);
			}

			// Velocity 3-5

			this->calcJoints[i].jointType  = (NeuronCalc::BonesType)i;
			this->calcJoints[i].quaternion.w = std::stof(dataList[i*16 + 6]);
			this->calcJoints[i].quaternion.x = std::stof(dataList[i*16 + 7]);
			this->calcJoints[i].quaternion.y = std::stof(dataList[i*16 + 8]);
			this->calcJoints[i].quaternion.z = std::stof(dataList[i*16 + 9]);

			// Accelerated velocity 10-12

			// Gyro 13-15
		}
	}

	return true;
}
