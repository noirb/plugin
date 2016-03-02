#include <map>
#include <sigverse/plugin/common/sensor/PerceptionNeuronSensorData.h>

//PerceptionNeuronSensorData::SensorDataMode PerceptionNeuronSensorData::sensorDataMode;

///@brief Constructor.
PerceptionNeuronSensorData::PerceptionNeuronSensorData()
{
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
	//// Generate map<string, vector<string>, which has only JointType.
	//std::map<std::string, std::vector<std::string> > jointMap;
	//std::vector<std::string> emptyStrings(4, "");

	//jointMap.insert(std::map<std::string, std::vector<std::string> >::value_type(MSG_KEY_ROOT_P,                          emptyStrings));
	//jointMap.insert(std::map<std::string, std::vector<std::string> >::value_type(jointType2ShortJointName(SpineBase),     emptyStrings));
	//jointMap.insert(std::map<std::string, std::vector<std::string> >::value_type(jointType2ShortJointName(SpineMid),      emptyStrings));
	//jointMap.insert(std::map<std::string, std::vector<std::string> >::value_type(jointType2ShortJointName(Neck),          emptyStrings));
	//jointMap.insert(std::map<std::string, std::vector<std::string> >::value_type(jointType2ShortJointName(Head),          emptyStrings));
	//jointMap.insert(std::map<std::string, std::vector<std::string> >::value_type(jointType2ShortJointName(ShoulderLeft) , emptyStrings));
	//jointMap.insert(std::map<std::string, std::vector<std::string> >::value_type(jointType2ShortJointName(ElbowLeft) ,    emptyStrings));
	//jointMap.insert(std::map<std::string, std::vector<std::string> >::value_type(jointType2ShortJointName(WristLeft) ,    emptyStrings));
	//jointMap.insert(std::map<std::string, std::vector<std::string> >::value_type(jointType2ShortJointName(HandLeft),      emptyStrings));
	//jointMap.insert(std::map<std::string, std::vector<std::string> >::value_type(jointType2ShortJointName(ShoulderRight), emptyStrings));
	//jointMap.insert(std::map<std::string, std::vector<std::string> >::value_type(jointType2ShortJointName(ElbowRight) ,   emptyStrings));
	//jointMap.insert(std::map<std::string, std::vector<std::string> >::value_type(jointType2ShortJointName(WristRight) ,   emptyStrings));
	//jointMap.insert(std::map<std::string, std::vector<std::string> >::value_type(jointType2ShortJointName(HandRight),     emptyStrings));
	//jointMap.insert(std::map<std::string, std::vector<std::string> >::value_type(jointType2ShortJointName(HipLeft) ,      emptyStrings));
	//jointMap.insert(std::map<std::string, std::vector<std::string> >::value_type(jointType2ShortJointName(KneeLeft),      emptyStrings));
	//jointMap.insert(std::map<std::string, std::vector<std::string> >::value_type(jointType2ShortJointName(AnkleLeft),     emptyStrings));
	//jointMap.insert(std::map<std::string, std::vector<std::string> >::value_type(jointType2ShortJointName(FootLeft) ,     emptyStrings));
	//jointMap.insert(std::map<std::string, std::vector<std::string> >::value_type(jointType2ShortJointName(HipRight) ,     emptyStrings));
	//jointMap.insert(std::map<std::string, std::vector<std::string> >::value_type(jointType2ShortJointName(KneeRight),     emptyStrings));
	//jointMap.insert(std::map<std::string, std::vector<std::string> >::value_type(jointType2ShortJointName(AnkleRight),    emptyStrings));
	//jointMap.insert(std::map<std::string, std::vector<std::string> >::value_type(jointType2ShortJointName(FootRight) ,    emptyStrings));
	//jointMap.insert(std::map<std::string, std::vector<std::string> >::value_type(jointType2ShortJointName(SpineShoulder), emptyStrings));
	//jointMap.insert(std::map<std::string, std::vector<std::string> >::value_type(jointType2ShortJointName(HandTipLeft),   emptyStrings));
	//jointMap.insert(std::map<std::string, std::vector<std::string> >::value_type(jointType2ShortJointName(ThumbLeft),     emptyStrings));
	//jointMap.insert(std::map<std::string, std::vector<std::string> >::value_type(jointType2ShortJointName(HandTipRight) , emptyStrings));
	//jointMap.insert(std::map<std::string, std::vector<std::string> >::value_type(jointType2ShortJointName(ThumbRight),    emptyStrings));

	std::map<std::string, std::vector<std::string> >::const_iterator it;

	for (it = sensorDataMap.begin(); it != sensorDataMap.end(); it++)
	{
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
//				bool a = (strcmp((*it).second[0].c_str(),"true"))? false:true;
//				bool b = (strcmp((*it).second[0].c_str(),"false"))?true :false;
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
			case Data:
			{
				std::vector<std::string> dataList = (*it).second;

				for(int i = 0; i < (int)dataList.size(); ++i)
				{
					this->bvhData.data[i] = std::stof(dataList.at(i));
				}
				
				break;
			}
		}
	}

	return true;
}

/////@brief Set root position.
//void PerceptionNeuronSensorData::setRootPosition(const SigCmn::Vector3 &position)
//{
//	this->rootPosition = position;
//}
//
//
//void PerceptionNeuronSensorData::setPerceptionNeuronJointPosition(PerceptionNeuronJointPosition *PerceptionNeuronJointPositions)
//{
//	for (int i = 0; i < PerceptionNeuronJointType_Count; i++)
//	{
//		this->jointPositions[i].jointType     = PerceptionNeuronJointPositions[i].jointType;
//		this->jointPositions[i].position      = PerceptionNeuronJointPositions[i].position;
//		this->jointPositions[i].trackingState = PerceptionNeuronJointPositions[i].trackingState;
//	}
//}

//void PerceptionNeuronSensorData::getPerceptionNeuronJointPosition(PerceptionNeuronJointPosition* destination) const
//{
//	for (int i = 0; i < PerceptionNeuronJointType_Count; i++)
//	{
//		destination[i].jointType     = this->jointPositions[i].jointType;
//		destination[i].position      = this->jointPositions[i].position;
//		destination[i].trackingState = this->jointPositions[i].trackingState;
//	}
//}
//
//void PerceptionNeuronSensorData::setPerceptionNeuronJointOrientation(PerceptionNeuronJointOrientation *PerceptionNeuronJointOrientations)
//{
//	for (int i = 0; i < PerceptionNeuronJointType_Count; i++)
//	{
//		this->jointOrientations[i].jointType   = PerceptionNeuronJointOrientations[i].jointType;
//		this->jointOrientations[i].orientation = PerceptionNeuronJointOrientations[i].orientation;
//	}
//}
//
//void PerceptionNeuronSensorData::getPerceptionNeuronJointOrientation(PerceptionNeuronJointOrientation* destination) const
//{
//	for (int i = 0; i < PerceptionNeuronJointType_Count; i++)
//	{
//		destination[i].jointType   = this->jointOrientations[i].jointType;
//		destination[i].orientation = this->jointOrientations[i].orientation;
//	}
//}

/////@brief Get string enum of JointType.
//std::string PerceptionNeuronSensorData::jointType2String(PerceptionNeuronSensorData::PerceptionNeuronJointType e) const
//{
//	switch (e)
//	{
//		case SpineBase:      { return("SpineBase");     break; }
//		case SpineMid:       { return("SpineMid");      break; }
//		case Neck:           { return("Neck");          break; }
//		case Head:           { return("Head");          break; }
//		case ShoulderLeft:   { return("ShoulderLeft");  break; }
//		case ElbowLeft:      { return("ElbowLeft");     break; }
//		case WristLeft:      { return("WristLeft");     break; }
//		case HandLeft:       { return("HandLeft");      break; }
//		case ShoulderRight:  { return("ShoulderRight"); break; }
//		case ElbowRight:     { return("ElbowRight");    break; }
//		case WristRight:     { return("WristRight");    break; }
//		case HandRight:      { return("HandRight");     break; }
//		case HipLeft:        { return("HipLeft");       break; }
//		case KneeLeft:       { return("KneeLeft");      break; }
//		case AnkleLeft:      { return("AnkleLeft");     break; }
//		case FootLeft:       { return("FootLeft");      break; }
//		case HipRight:       { return("HipRight");      break; }
//		case KneeRight:      { return("KneeRight");     break; }
//		case AnkleRight:     { return("AnkleRight");    break; }
//		case FootRight:      { return("FootRight");     break; }
//		case SpineShoulder:  { return("SpineShoulder"); break; }
//		case HandTipLeft:    { return("HandTipLeft");   break; }
//		case ThumbLeft:      { return("ThumbLeft");     break; }
//		case HandTipRight:   { return("HandTipRight");  break; }
//		case ThumbRight:     { return("ThumbRight");    break; }
//		default:             { return("illegal");       break; }
//	}
//}

/////@brief Get string enum of JointType to genrate message.
//std::string PerceptionNeuronSensorData::jointType2ShortJointName(PerceptionNeuronJointType e) const
//{
//	std::string sensorDataModeType;
//
//	switch (sensorDataMode)
//	{
//		case QUATERNION:{ sensorDataModeType = "Q"; break; }
//		case POSITION:  { sensorDataModeType = "P"; break; }
//	}
//
//	switch (e)
//	{
//		case SpineBase:      { return "SpBs_" + sensorDataModeType; break; }
//		case SpineMid:       { return "SpMd_" + sensorDataModeType; break; }
//		case Neck:           { return "Neck_" + sensorDataModeType; break; }
//		case Head:           { return "Head_" + sensorDataModeType; break; }
//		case ShoulderLeft:   { return "ShL_"  + sensorDataModeType; break; }
//		case ElbowLeft:      { return "LbL_"  + sensorDataModeType; break; }
//		case WristLeft:      { return "WrL_"  + sensorDataModeType; break; }
//		case HandLeft:       { return "HndL_" + sensorDataModeType; break; }
//		case ShoulderRight:  { return "ShR_"  + sensorDataModeType; break; }
//		case ElbowRight:     { return "LbR_"  + sensorDataModeType; break; }
//		case WristRight:     { return "WrR_"  + sensorDataModeType; break; }
//		case HandRight:      { return "HndR_" + sensorDataModeType; break; }
//		case HipLeft:        { return "HpL_"  + sensorDataModeType; break; }
//		case KneeLeft:       { return "NeeL_" + sensorDataModeType; break; }
//		case AnkleLeft:      { return "AnkL_" + sensorDataModeType; break; }
//		case FootLeft:       { return "FtL_"  + sensorDataModeType; break; }
//		case HipRight:       { return "HpR_"  + sensorDataModeType; break; }
//		case KneeRight:      { return "NeeR_" + sensorDataModeType; break; }
//		case AnkleRight:     { return "AnkR_" + sensorDataModeType; break; }
//		case FootRight:      { return "FtR_"  + sensorDataModeType; break; }
//		case SpineShoulder:  { return "SpSh_" + sensorDataModeType; break; }
//		case HandTipLeft:    { return "HTL_"  + sensorDataModeType; break; }
//		case ThumbLeft:      { return "ThmL_" + sensorDataModeType; break; }
//		case HandTipRight:   { return "HTR_"  + sensorDataModeType; break; }
//		case ThumbRight:     { return "ThmR_" + sensorDataModeType; break; }
//		default:             { return "illegal";                    break; }
//	}
//}


//PerceptionNeuronSensorData::PerceptionNeuronJointType PerceptionNeuronSensorData::shortJointName2PerceptionNeuronJointType(const std::string &shortJointName) const
//{
//	if (shortJointName.length()==0)
//	{
//		throw std::string("Short joint name is blank.");
//	}
//
//	std::string  shortJointNameForJudge = shortJointName;
//	shortJointNameForJudge.erase(shortJointNameForJudge.length()-1);
//
//	if      (shortJointNameForJudge == "SpBs_"){ return SpineBase; }
//	else if (shortJointNameForJudge == "SpMd_"){ return SpineMid; }
//	else if (shortJointNameForJudge == "Neck_"){ return Neck; }
//	else if (shortJointNameForJudge == "Head_"){ return Head; }
//	else if (shortJointNameForJudge == "ShL_") { return ShoulderLeft; }
//	else if (shortJointNameForJudge == "LbL_") { return ElbowLeft; }
//	else if (shortJointNameForJudge == "WrL_") { return WristLeft; }
//	else if (shortJointNameForJudge == "HndL_"){ return HandLeft; }
//	else if (shortJointNameForJudge == "ShR_") { return ShoulderRight; }
//	else if (shortJointNameForJudge == "LbR_") { return ElbowRight; }
//	else if (shortJointNameForJudge == "WrR_") { return WristRight; }
//	else if (shortJointNameForJudge == "HndR_"){ return HandRight; }
//	else if (shortJointNameForJudge == "HpL_") { return HipLeft; }
//	else if (shortJointNameForJudge == "NeeL_"){ return KneeLeft; }
//	else if (shortJointNameForJudge == "AnkL_"){ return AnkleLeft; }
//	else if (shortJointNameForJudge == "FtL_") { return FootLeft; }
//	else if (shortJointNameForJudge == "HpR_") { return HipRight; }
//	else if (shortJointNameForJudge == "NeeR_"){ return KneeRight; }
//	else if (shortJointNameForJudge == "AnkR_"){ return AnkleRight; }
//	else if (shortJointNameForJudge == "FtR_") { return FootRight;}
//	else if (shortJointNameForJudge == "SpSh_"){ return SpineShoulder; }
//	else if (shortJointNameForJudge == "HTL_") { return HandTipLeft; }
//	else if (shortJointNameForJudge == "ThmL_"){ return ThumbLeft; }
//	else if (shortJointNameForJudge == "HTR_") { return HandTipRight; }
//	else if (shortJointNameForJudge == "ThmR_"){ return ThumbRight; }
//	else { throw std::string(("Short joint name is invalid. : "+shortJointName).c_str()); }
//}


/////@brief root position (Root_P, x, y, z) to string.
//std::string PerceptionNeuronSensorData::rootPosition2Message(const SigCmn::Vector3 &position, const std::string &keyValueDelim, const std::string &valuesDelim) const
//{
//	std::stringstream ss;
//	ss << MSG_KEY_ROOT_P << keyValueDelim << position2Message(position, valuesDelim);
//	return ss.str();
//}


/////@brief Three dimensional position (x, y, z) to string.
//std::string PerceptionNeuronSensorData::position2Message(const SigCmn::Vector3 &position, const std::string &valuesDelim) const
//{
//	std::stringstream ssPosition;
//	ssPosition << std::setprecision(PerceptionNeuronSensorData::rootPositionPrecision);
//	ssPosition << position.x << valuesDelim << position.y << valuesDelim << position.z;
//	return ssPosition.str();
//}
//
/////@brief JointPosition (jointname, x, y, z) to string.
//std::string PerceptionNeuronSensorData::jointPosition2Message(const PerceptionNeuronJointPosition &jp, const std::string &keyValueDelim, const std::string &valuesDelim) const
//{
//	std::stringstream ss;
//	ss << jointType2ShortJointName(jp.jointType) << keyValueDelim << position2Message(jp.position, valuesDelim) << valuesDelim << jp.trackingState;
//	return ss.str();
//}


/////@brief Orientation (w, x, y, z) to string.
//std::string PerceptionNeuronSensorData::orientation2Message(const Quaternion &orientation, const std::string &valuesDelim) const
//{
//	std::stringstream ssOrientation;
//	ssOrientation << std::setprecision(PerceptionNeuronSensorData::orientationPrecision);
//	ssOrientation << orientation.w << valuesDelim << orientation.x << valuesDelim << orientation.y << valuesDelim << orientation.z;
//	return ssOrientation.str();
//}
//
/////@brief JointOrientation (jointname, w, x, y, z) to string.
//std::string PerceptionNeuronSensorData::jointOrientation2Message(const PerceptionNeuronJointOrientation &jo, const std::string &keyValueDelim, const std::string &valuesDelim) const
//{
//	std::stringstream ss;
//	ss << jointType2ShortJointName(jo.jointType) << keyValueDelim << orientation2Message(jo.orientation, valuesDelim);
//	return ss.str();
//}
//
/////@brief Hand state (left hand and right hand) to string.
//std::string PerceptionNeuronSensorData::handState2Message(const HandState &leftHandState, const HandState &rightHandState, const std::string &keyValueDelim, const std::string &valuesDelim) const
//{
//	std::stringstream ss;
//	ss << MSG_KEY_HAND_STATE << keyValueDelim << leftHandState << valuesDelim << rightHandState;
//	return ss.str();
//}
//


/////@brief Set hand state of left hand.
//void PerceptionNeuronSensorData::setLeftHandState(const HandState &handState)
//{
//	this->leftHandState = handState;
//}
//
/////@brief Set hand state of right hand.
//void PerceptionNeuronSensorData::setRightHandState(const HandState &handState)
//{
//	this->rightHandState = handState;
//}
/////@brief Get hand state of left hand.
//PerceptionNeuronSensorData::HandState PerceptionNeuronSensorData::getLeftHandState() const
//{
//	return this->leftHandState;
//}
/////@brief Get hand state of right hand.
//PerceptionNeuronSensorData::HandState PerceptionNeuronSensorData::getRightHandState() const
//{
//	return this->rightHandState;
//}
