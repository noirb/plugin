#ifndef SIGVERSE_PERCEPTION_NEURON_SENSOR_DATA_H
#define SIGVERSE_PERCEPTION_NEURON_SENSOR_DATA_H

#include <sigverse/plugin/common/sensor/SensorData.h>
#include <sigverse/plugin/common/Quaternion.h>
#include <sigverse/plugin/common/SigCmn.h>
#include <string>
#include <sstream>
#include <iomanip>

// Set precision of quaternion and position. Only for send information.
//#define ORIENTATION_PRECISION 5
//#define POSITION_PRECISION 5

//#define MSG_KEY_ROOT_P "Root_P"
//#define MSG_KEY_HAND_STATE  "HandState"


///@brief Posture class for using kinect v2 device.
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

		BVHBoneCount = LeftHandPinky3 + 1
	};
	
	//enum PerceptionNeuronTrackingState
	//{
	//	NotTracked = 0,
	//	Inferred,
	//	Tracked,
	//};

	//enum HandState
	//{
	//	HandState_Unknown    = 0,
	//	HandState_NotTracked = 1,
	//	HandState_Open       = 2,
	//	HandState_Closed     = 3,
	//	HandState_Lasso      = 4,
	//	HandState_NotUsed    = 9
	//};

	//typedef struct _PerceptionNeuronJointPosition
	//{
	//	PerceptionNeuronJointType     jointType;
	//	SigCmn::Vector3       position;
	//	PerceptionNeuronTrackingState trackingState;
	//} PerceptionNeuronJointPosition;

	//typedef struct _PerceptionNeuronJointOrientation
	//{
	//	PerceptionNeuronJointType jointType;
	//	Quaternion        orientation;
	//} PerceptionNeuronJointOrientation;

	enum BvhItemEnum
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
	/////@brief root position (Root_P, x, y, z) to string.
	//std::string rootPosition2Message(const SigCmn::Vector3 &position, const std::string &keyValueDelim, const std::string &valuesDelim) const;

	/////@brief Three dimensional position (x, y, z) to string.
	//std::string position2Message(const SigCmn::Vector3 &position, const std::string &valuesDelim) const;

	/////@brief JointPosition (jointname, x, y, z) to string.
	//std::string jointPosition2Message(const PerceptionNeuronJointPosition &jp, const std::string &keyValueDelim, const std::string &valuesDelim) const;

	//std::string orientation2Message(const Quaternion &orientation, const std::string &valuesDelim) const;

	/////@brief JointOrientation (jointname, w, x, y, z) to string.
	//std::string jointOrientation2Message(const PerceptionNeuronJointOrientation &jo, const std::string &keyValueDelim, const std::string &valuesDelim) const;

	//std::string jointType2String(PerceptionNeuronSensorData::PerceptionNeuronJointType e) const;

	//std::string jointType2ShortJointName(PerceptionNeuronJointType e) const;

	/////@brief Hand state (left hand and right hand) to string.
	//std::string handState2Message(const HandState &leftHandState, const HandState &rightHandState, const std::string &keyValueDelim, const std::string &valuesDelim) const;

	/////@brief Left hand state. (Open, Closed, Lasso, and so on.)
	//HandState leftHandState;
	/////@brief Right hand state. (Open, Closed, Lasso, and so on.)
	//HandState rightHandState;

	std::map<BvhItemEnum, std::string> mapItemEnum2Str;
	std::map<std::string, BvhItemEnum> mapItemStr2Enum;

public:

	BvhData bvhData;

	///@brief Constructor.
	PerceptionNeuronSensorData();

	////sensor data mode.("Quaternion" or "Position")
	//static SensorDataMode sensorDataMode;

	/////@brief Constructor
	//static void setSensorDataMode(std::string sensorDataModeStr);

	std::string getBvhItemName(BvhItemEnum bvhItemEnum) const;
	BvhItemEnum getBvhItemEnum(std::string itemName) const;

	///@brief Convert sensor information to string.
	///@param itemsDelim Symbol to split items.
	///@param keyValueDelim Symbol to split KEY and VALUE.
	///@param valuesDelim Symbol to split each componet of VALUE. When VALUE is multidimensional data, you use this symbol.
	std::string encodeSensorData(const std::string &itemsDelim = ITEMS_DELIMITER, const std::string &keyValueDelim = KEY_AND_VALUE_DELIMITER, const std::string &valuesDelim = VALUES_DELIMITER) const;

	std::string getDataString(const std::string &valuesDelim = VALUES_DELIMITER) const;

	///@brief Set to sensor data.
	///@param Map of sensor data;
	bool setSensorData(const std::map<std::string, std::vector<std::string> > &sensorDataMap);

	/////@brief Set root position.
	//void setRootPosition(const SigCmn::Vector3 &position);

	/////@brief Set posture from kinect joint positions.
	//void setPerceptionNeuronJointPosition(PerceptionNeuronJointPosition *PerceptionNeuronJointPositions);

	/////@brief Get orientations from kinect joint positions.
	//void getPerceptionNeuronJointPosition(PerceptionNeuronJointPosition *destination) const;

	/////@brief Set posture from kinect joint orientations.
	//void setPerceptionNeuronJointOrientation(PerceptionNeuronJointOrientation *PerceptionNeuronJointOrientation);

	/////@brief Get orientations from kinect joint orientations.
	//void getPerceptionNeuronJointOrientation(PerceptionNeuronJointOrientation *destination) const;

	/////@brief Set hand state of left hand.
	//void setLeftHandState(const HandState &handState);
	/////@brief Set hand state of right hand.
	//void setRightHandState(const HandState &handState);
	/////@brief Get hand state of left hand.
	//HandState getLeftHandState() const;
	/////@brief Get hand state of right hand.
	//HandState getRightHandState() const;

	/////@brief Set joint quaternions for man-nii avatar.
	//PerceptionNeuronJointType shortJointName2PerceptionNeuronJointType(const std::string &shortJointName) const;

	/////@brief Root body position.
	//SigCmn::Vector3 rootPosition;

	/////@brief Whole body joint positions.
	//PerceptionNeuronJointPosition jointPositions[PerceptionNeuronJointType_Count];

	/////@brief Whole body joint orientations.
	//PerceptionNeuronJointOrientation jointOrientations[PerceptionNeuronJointType_Count];
};

#endif // SIGVERSE_PERCEPTION_NEURON_SENSOR_DATA_H


