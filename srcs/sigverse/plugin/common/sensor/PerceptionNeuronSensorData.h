#ifndef SIGVERSE_PERCEPTION_NEURON_SENSOR_DATA_H
#define SIGVERSE_PERCEPTION_NEURON_SENSOR_DATA_H

#include <sigverse/plugin/common/sensor/SensorData.h>
#include <sigverse/plugin/common/Quaternion.h>
#include <sigverse/plugin/common/SigCmn.h>
#include <string>
#include <sstream>
#include <iomanip>

///@brief Sensor data class for using Perception Neuron device.
class PerceptionNeuronSensorData : public SensorData
{
public:
	enum BVHBonesType
	{
		Hips          = 0,
		RightUpLeg,
		RightLeg,
		RightFoot,
		LeftUpLeg,
		LeftLeg,
		LeftFoot,
		Spine,
		Spine1,
		Spine2,
		Spine3,
		Neck,
		Head,

		RightShoulder,
		RightArm,
		RightForeArm,
		RightHand,
		RightHandThumb1,
		RightHandThumb2,
		RightHandThumb3,
		RightInHandIndex,
		RightHandIndex1,
		RightHandIndex2,
		RightHandIndex3,
		RightInHandMiddle,
		RightHandMiddle1,
		RightHandMiddle2,
		RightHandMiddle3,
		RightInHandRing,
		RightHandRing1,
		RightHandRing2,
		RightHandRing3,
		RightInHandPinky,
		RightHandPinky1,
		RightHandPinky2,
		RightHandPinky3,

		LeftShoulder,
		LeftArm,
		LeftForeArm,
		LeftHand,
		LeftHandThumb1,
		LeftHandThumb2,
		LeftHandThumb3,
		LeftInHandIndex,
		LeftHandIndex1,
		LeftHandIndex2,
		LeftHandIndex3,
		LeftInHandMiddle,
		LeftHandMiddle1,
		LeftHandMiddle2,
		LeftHandMiddle3,
		LeftInHandRing,
		LeftHandRing1,
		LeftHandRing2,
		LeftHandRing3,
		LeftInHandPinky,
		LeftHandPinky1,
		LeftHandPinky2,
		LeftHandPinky3,

		BVHBonesTypeCount = LeftHandPinky3 + 1
	};
	
	typedef struct _PerceptionNeuronJointRotation
	{
		BVHBonesType     jointType;
		SigCmn::Vector3  rotation;
	} PerceptionNeuronJointRotation;

	enum MsgItemEnum
	{
		AvatarIndex = 0,
		AvatarName,
		WithDisp,
		WithReference,
		FrameIndex,
		DataCount,
		Data,
	};
	
	typedef struct _BvhData
	{
		unsigned int   avatarIndex;
		std::string    avatarName;
		bool           withDisp;
		bool           withReference;
		unsigned int   frameIndex;
		unsigned short dataCount;
		float          *data;
	} BvhData;

private:
	std::map<MsgItemEnum, std::string> mapMsgItemEnum2Str;
	std::map<std::string, MsgItemEnum> mapMsgItemStr2Enum;

public:

	BvhData bvhData;

	///@brief Constructor.
	PerceptionNeuronSensorData();

	///@brief Convert sensor information to string.
	///@param itemsDelim Symbol to split items.
	///@param keyValueDelim Symbol to split KEY and VALUE.
	///@param valuesDelim Symbol to split each component of VALUE. When VALUE is multidimensional data, you use this symbol.
	std::string encodeSensorData(const std::string &itemsDelim = ITEMS_DELIMITER, const std::string &keyValueDelim = KEY_AND_VALUE_DELIMITER, const std::string &valuesDelim = VALUES_DELIMITER) const;

	///@brief Get data string for sending message.
	std::string getDataString(const std::string &valuesDelim = VALUES_DELIMITER) const;

	///@brief Set to sensor data.
	///@param Map of sensor data;
	bool setSensorData(const std::map<std::string, std::vector<std::string> > &sensorDataMap);

	///@brief Set to sensor data (Joint rotations).
	///@param Map of sensor data (Joint rotations);
	bool setSensorDataJointRotation(const std::map<std::string, std::vector<std::string> > &sensorDataMap);

	///@brief Root body position.
	SigCmn::Vector3 rootPosition;

	///@brief Whole body joint rotation.
	PerceptionNeuronJointRotation jointRotations[BVHBonesTypeCount];
};

#endif // SIGVERSE_PERCEPTION_NEURON_SENSOR_DATA_H


