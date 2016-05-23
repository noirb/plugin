/*
 * PerceptionNeuronDeviceManager.cpp
 *
 *  Created on: 2016/03/02
 *      Author: Yamada
 */

#include <sigverse/plugin/controller/common/sensor/PerceptionNeuronDeviceManager.h>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <cmath>

PerceptionNeuronDeviceManager::PerceptionNeuronDeviceManager()
{
	this->service        = NULL;
	this->serviceName    = "";
	this->deviceType     = "";
	this->deviceUniqueID = "";

	this->started = false;
}

PerceptionNeuronDeviceManager::PerceptionNeuronDeviceManager(std::string &name, std::string &deviceType, std::string &deviceUniqueID)
{
	this->service        = NULL;
	this->serviceName    = name;
	this->deviceType     = deviceType;
	this->deviceUniqueID = deviceUniqueID;

	this->started = false;
}


///@brief Get initial positions and rotations.
void PerceptionNeuronDeviceManager::getInitialPositionAndRotation(SimObj *myself)
{
	// Get initial positions.
	Vector3d tmpPos;
	myself->getPosition(tmpPos);
	this->iniPos.x = tmpPos.x();
	this->iniPos.y = tmpPos.y();
	this->iniPos.z = tmpPos.z();
}


void PerceptionNeuronDeviceManager::setPosition2ManBvh(SimObj *obj, const ManBvhPosture &manBvhPosture)
{
	//Calculate moving distance from start position.
	SigCmn::Vector3 movingDistance;
	double dx = +(manBvhPosture.rootPosision.x - this->sensorStartPos.x);
	double dy = +(manBvhPosture.rootPosision.y - this->sensorStartPos.y);
	double dz = +(manBvhPosture.rootPosision.z - this->sensorStartPos.z);

	float yrot = SigCmn::deg2rad<float>(this->sensorStartRot.x);

	double gx = std::cos(yrot)*dx - std::sin(yrot)*dz;
	double gz = std::sin(yrot)*dx + std::cos(yrot)*dz;

	obj->setPosition(this->iniPos.x + gx, this->iniPos.y + dy, this->iniPos.z + gz);
}


void PerceptionNeuronDeviceManager::setJointAngle2ManBvhYXZ(SimObj *obj, const ManBvhPosture &manBvhPosture, const ManBvhPosture::ManBvhJointType &manBvhJointType)
{
	obj->setJointAngle((ManBvhPosture::manBvhJointTypeStr(manBvhJointType)+"_Y").c_str(), SigCmn::deg2rad<float>(manBvhPosture.joint[manBvhJointType].angle.x));
	obj->setJointAngle((ManBvhPosture::manBvhJointTypeStr(manBvhJointType)+"_X").c_str(), SigCmn::deg2rad<float>(manBvhPosture.joint[manBvhJointType].angle.y));
	obj->setJointAngle((ManBvhPosture::manBvhJointTypeStr(manBvhJointType)+"_Z").c_str(), SigCmn::deg2rad<float>(manBvhPosture.joint[manBvhJointType].angle.z));
}


void PerceptionNeuronDeviceManager::setPosture2ManBvhYXZ(SimObj *obj, const ManBvhPosture &manBvhPosture)
{
	// Set position.
	this->setPosition2ManBvh(obj, manBvhPosture);

	// Set angles.
	this->setJointAngle2ManBvhYXZ(obj, manBvhPosture, ManBvhPosture::HIP_JOINT);
	this->setJointAngle2ManBvhYXZ(obj, manBvhPosture, ManBvhPosture::WAIST_JOINT);
	this->setJointAngle2ManBvhYXZ(obj, manBvhPosture, ManBvhPosture::NECK_JOINT);

	this->setJointAngle2ManBvhYXZ(obj, manBvhPosture, ManBvhPosture::LCHEST_JOINT);  //It's for Perception Neuron.
	this->setJointAngle2ManBvhYXZ(obj, manBvhPosture, ManBvhPosture::LSHOULDER_JOINT);
	this->setJointAngle2ManBvhYXZ(obj, manBvhPosture, ManBvhPosture::LELBOW_JOINT);
	this->setJointAngle2ManBvhYXZ(obj, manBvhPosture, ManBvhPosture::LWRIST_JOINT);
	this->setJointAngle2ManBvhYXZ(obj, manBvhPosture, ManBvhPosture::RCHEST_JOINT);  //It's for Perception Neuron.
	this->setJointAngle2ManBvhYXZ(obj, manBvhPosture, ManBvhPosture::RSHOULDER_JOINT);
	this->setJointAngle2ManBvhYXZ(obj, manBvhPosture, ManBvhPosture::RELBOW_JOINT);
	this->setJointAngle2ManBvhYXZ(obj, manBvhPosture, ManBvhPosture::RWRIST_JOINT);

	this->setJointAngle2ManBvhYXZ(obj, manBvhPosture, ManBvhPosture::LHIP_JOINT);
	this->setJointAngle2ManBvhYXZ(obj, manBvhPosture, ManBvhPosture::LKNEE_JOINT);
	this->setJointAngle2ManBvhYXZ(obj, manBvhPosture, ManBvhPosture::LANKLE_JOINT);
	this->setJointAngle2ManBvhYXZ(obj, manBvhPosture, ManBvhPosture::RHIP_JOINT);
	this->setJointAngle2ManBvhYXZ(obj, manBvhPosture, ManBvhPosture::RKNEE_JOINT);
	this->setJointAngle2ManBvhYXZ(obj, manBvhPosture, ManBvhPosture::RANKLE_JOINT);

	// left hand
	this->setJointAngle2ManBvhYXZ(obj, manBvhPosture, ManBvhPosture::LeftH_T1);
	this->setJointAngle2ManBvhYXZ(obj, manBvhPosture, ManBvhPosture::LeftH_T2);
	this->setJointAngle2ManBvhYXZ(obj, manBvhPosture, ManBvhPosture::LeftH_T3);
	this->setJointAngle2ManBvhYXZ(obj, manBvhPosture, ManBvhPosture::LeftH_I1);
	this->setJointAngle2ManBvhYXZ(obj, manBvhPosture, ManBvhPosture::LeftH_I2);
	this->setJointAngle2ManBvhYXZ(obj, manBvhPosture, ManBvhPosture::LeftH_I3);
	this->setJointAngle2ManBvhYXZ(obj, manBvhPosture, ManBvhPosture::LeftH_M1);
	this->setJointAngle2ManBvhYXZ(obj, manBvhPosture, ManBvhPosture::LeftH_M2);
	this->setJointAngle2ManBvhYXZ(obj, manBvhPosture, ManBvhPosture::LeftH_M3);
	this->setJointAngle2ManBvhYXZ(obj, manBvhPosture, ManBvhPosture::LeftH_R1);
	this->setJointAngle2ManBvhYXZ(obj, manBvhPosture, ManBvhPosture::LeftH_R2);
	this->setJointAngle2ManBvhYXZ(obj, manBvhPosture, ManBvhPosture::LeftH_R3);
	this->setJointAngle2ManBvhYXZ(obj, manBvhPosture, ManBvhPosture::LeftH_P1);
	this->setJointAngle2ManBvhYXZ(obj, manBvhPosture, ManBvhPosture::LeftH_P2);
	this->setJointAngle2ManBvhYXZ(obj, manBvhPosture, ManBvhPosture::LeftH_P3);

	// Right hand
	this->setJointAngle2ManBvhYXZ(obj, manBvhPosture, ManBvhPosture::RightH_T1);
	this->setJointAngle2ManBvhYXZ(obj, manBvhPosture, ManBvhPosture::RightH_T2);
	this->setJointAngle2ManBvhYXZ(obj, manBvhPosture, ManBvhPosture::RightH_T3);
	this->setJointAngle2ManBvhYXZ(obj, manBvhPosture, ManBvhPosture::RightH_I1);
	this->setJointAngle2ManBvhYXZ(obj, manBvhPosture, ManBvhPosture::RightH_I2);
	this->setJointAngle2ManBvhYXZ(obj, manBvhPosture, ManBvhPosture::RightH_I3);
	this->setJointAngle2ManBvhYXZ(obj, manBvhPosture, ManBvhPosture::RightH_M1);
	this->setJointAngle2ManBvhYXZ(obj, manBvhPosture, ManBvhPosture::RightH_M2);
	this->setJointAngle2ManBvhYXZ(obj, manBvhPosture, ManBvhPosture::RightH_M3);
	this->setJointAngle2ManBvhYXZ(obj, manBvhPosture, ManBvhPosture::RightH_R1);
	this->setJointAngle2ManBvhYXZ(obj, manBvhPosture, ManBvhPosture::RightH_R2);
	this->setJointAngle2ManBvhYXZ(obj, manBvhPosture, ManBvhPosture::RightH_R3);
	this->setJointAngle2ManBvhYXZ(obj, manBvhPosture, ManBvhPosture::RightH_P1);
	this->setJointAngle2ManBvhYXZ(obj, manBvhPosture, ManBvhPosture::RightH_P2);
	this->setJointAngle2ManBvhYXZ(obj, manBvhPosture, ManBvhPosture::RightH_P3);
}


ManBvhPosture PerceptionNeuronDeviceManager::convertSensorData2ManBvhPosture(const PerceptionNeuronSensorData &sensorData)
{
	//Save start position
	if (this->started == false)
	{
		this->sensorStartPos.x = sensorData.rootPosition.x;
		this->sensorStartPos.y = sensorData.rootPosition.y;
		this->sensorStartPos.z = sensorData.rootPosition.z;
		printf("Start position   X : %f ------ Y : %f   ---- Z : %f   ---- end \n", this->sensorStartPos.x, this->sensorStartPos.y, this->sensorStartPos.z);

		this->sensorStartRot.x = sensorData.bvhJoints[NeuronBVH::Hips].rotation.x;
		this->sensorStartRot.y = sensorData.bvhJoints[NeuronBVH::Hips].rotation.y;
		this->sensorStartRot.z = sensorData.bvhJoints[NeuronBVH::Hips].rotation.z;

		this->started = true;
	}

	ManBvhPosture manBvhPosture;

	manBvhPosture.rootPosision = sensorData.rootPosition;

	//TODO Have to adjust Hip,Waist,Neck.
	SigCmn::Vector3 hipAngle = sensorData.bvhJoints[NeuronBVH::Hips].rotation;
	hipAngle.x -= this->sensorStartRot.x;

	SigCmn::Vector3 waistAngle = sensorData.bvhJoints[NeuronBVH::Spine1].rotation;
	SigCmn::Vector3 neckAngle = sensorData.bvhJoints[NeuronBVH::Head].rotation;

	manBvhPosture.joint[ManBvhPosture::HIP_JOINT  ].angle = hipAngle;
	manBvhPosture.joint[ManBvhPosture::WAIST_JOINT].angle = waistAngle;
	manBvhPosture.joint[ManBvhPosture::NECK_JOINT ].angle = neckAngle;

	manBvhPosture.joint[ManBvhPosture::LCHEST_JOINT   ].angle = sensorData.bvhJoints[NeuronBVH::LeftShoulder].rotation; //It's for Perception Neuron.
	manBvhPosture.joint[ManBvhPosture::LSHOULDER_JOINT].angle = sensorData.bvhJoints[NeuronBVH::LeftArm     ].rotation;
	manBvhPosture.joint[ManBvhPosture::LELBOW_JOINT   ].angle = sensorData.bvhJoints[NeuronBVH::LeftForeArm ].rotation;
	manBvhPosture.joint[ManBvhPosture::LWRIST_JOINT   ].angle = sensorData.bvhJoints[NeuronBVH::LeftHand    ].rotation;

	manBvhPosture.joint[ManBvhPosture::RCHEST_JOINT   ].angle = sensorData.bvhJoints[NeuronBVH::RightShoulder].rotation; //It's for Perception Neuron.
	manBvhPosture.joint[ManBvhPosture::RSHOULDER_JOINT].angle = sensorData.bvhJoints[NeuronBVH::RightArm     ].rotation;
	manBvhPosture.joint[ManBvhPosture::RELBOW_JOINT   ].angle = sensorData.bvhJoints[NeuronBVH::RightForeArm ].rotation;
	manBvhPosture.joint[ManBvhPosture::RWRIST_JOINT   ].angle = sensorData.bvhJoints[NeuronBVH::RightHand    ].rotation;

	manBvhPosture.joint[ManBvhPosture::LHIP_JOINT  ].angle = sensorData.bvhJoints[NeuronBVH::LeftUpLeg ].rotation;
	manBvhPosture.joint[ManBvhPosture::LKNEE_JOINT ].angle = sensorData.bvhJoints[NeuronBVH::LeftLeg   ].rotation;
	manBvhPosture.joint[ManBvhPosture::LANKLE_JOINT].angle = sensorData.bvhJoints[NeuronBVH::LeftFoot  ].rotation;
	manBvhPosture.joint[ManBvhPosture::RHIP_JOINT  ].angle = sensorData.bvhJoints[NeuronBVH::RightUpLeg].rotation;
	manBvhPosture.joint[ManBvhPosture::RKNEE_JOINT ].angle = sensorData.bvhJoints[NeuronBVH::RightLeg  ].rotation;
	manBvhPosture.joint[ManBvhPosture::RANKLE_JOINT].angle = sensorData.bvhJoints[NeuronBVH::RightFoot ].rotation;

	manBvhPosture.joint[ManBvhPosture::LeftH_T1].angle = sensorData.bvhJoints[NeuronBVH::LeftHandThumb1].rotation;
	manBvhPosture.joint[ManBvhPosture::LeftH_T2].angle = sensorData.bvhJoints[NeuronBVH::LeftHandThumb2].rotation;
	manBvhPosture.joint[ManBvhPosture::LeftH_T3].angle = sensorData.bvhJoints[NeuronBVH::LeftHandThumb3].rotation;
	manBvhPosture.joint[ManBvhPosture::LeftH_I1].angle = sensorData.bvhJoints[NeuronBVH::LeftHandIndex1].rotation;
	manBvhPosture.joint[ManBvhPosture::LeftH_I2].angle = sensorData.bvhJoints[NeuronBVH::LeftHandIndex2].rotation;
	manBvhPosture.joint[ManBvhPosture::LeftH_I3].angle = sensorData.bvhJoints[NeuronBVH::LeftHandIndex3].rotation;
	manBvhPosture.joint[ManBvhPosture::LeftH_M1].angle = sensorData.bvhJoints[NeuronBVH::LeftHandMiddle1].rotation;
	manBvhPosture.joint[ManBvhPosture::LeftH_M2].angle = sensorData.bvhJoints[NeuronBVH::LeftHandMiddle2].rotation;
	manBvhPosture.joint[ManBvhPosture::LeftH_M3].angle = sensorData.bvhJoints[NeuronBVH::LeftHandMiddle3].rotation;
	manBvhPosture.joint[ManBvhPosture::LeftH_R1].angle = sensorData.bvhJoints[NeuronBVH::LeftHandRing1].rotation;
	manBvhPosture.joint[ManBvhPosture::LeftH_R2].angle = sensorData.bvhJoints[NeuronBVH::LeftHandRing2].rotation;
	manBvhPosture.joint[ManBvhPosture::LeftH_R3].angle = sensorData.bvhJoints[NeuronBVH::LeftHandRing3].rotation;
	manBvhPosture.joint[ManBvhPosture::LeftH_P1].angle = sensorData.bvhJoints[NeuronBVH::LeftHandPinky1].rotation;
	manBvhPosture.joint[ManBvhPosture::LeftH_P2].angle = sensorData.bvhJoints[NeuronBVH::LeftHandPinky2].rotation;
	manBvhPosture.joint[ManBvhPosture::LeftH_P3].angle = sensorData.bvhJoints[NeuronBVH::LeftHandPinky3].rotation;

	manBvhPosture.joint[ManBvhPosture::RightH_T1].angle = sensorData.bvhJoints[NeuronBVH::RightHandThumb1].rotation;
	manBvhPosture.joint[ManBvhPosture::RightH_T2].angle = sensorData.bvhJoints[NeuronBVH::RightHandThumb2].rotation;
	manBvhPosture.joint[ManBvhPosture::RightH_T3].angle = sensorData.bvhJoints[NeuronBVH::RightHandThumb3].rotation;
	manBvhPosture.joint[ManBvhPosture::RightH_I1].angle = sensorData.bvhJoints[NeuronBVH::RightHandIndex1].rotation;
	manBvhPosture.joint[ManBvhPosture::RightH_I2].angle = sensorData.bvhJoints[NeuronBVH::RightHandIndex2].rotation;
	manBvhPosture.joint[ManBvhPosture::RightH_I3].angle = sensorData.bvhJoints[NeuronBVH::RightHandIndex3].rotation;
	manBvhPosture.joint[ManBvhPosture::RightH_M1].angle = sensorData.bvhJoints[NeuronBVH::RightHandMiddle1].rotation;
	manBvhPosture.joint[ManBvhPosture::RightH_M2].angle = sensorData.bvhJoints[NeuronBVH::RightHandMiddle2].rotation;
	manBvhPosture.joint[ManBvhPosture::RightH_M3].angle = sensorData.bvhJoints[NeuronBVH::RightHandMiddle3].rotation;
	manBvhPosture.joint[ManBvhPosture::RightH_R1].angle = sensorData.bvhJoints[NeuronBVH::RightHandRing1].rotation;
	manBvhPosture.joint[ManBvhPosture::RightH_R2].angle = sensorData.bvhJoints[NeuronBVH::RightHandRing2].rotation;
	manBvhPosture.joint[ManBvhPosture::RightH_R3].angle = sensorData.bvhJoints[NeuronBVH::RightHandRing3].rotation;
	manBvhPosture.joint[ManBvhPosture::RightH_P1].angle = sensorData.bvhJoints[NeuronBVH::RightHandPinky1].rotation;
	manBvhPosture.joint[ManBvhPosture::RightH_P2].angle = sensorData.bvhJoints[NeuronBVH::RightHandPinky2].rotation;
	manBvhPosture.joint[ManBvhPosture::RightH_P3].angle = sensorData.bvhJoints[NeuronBVH::RightHandPinky3].rotation;

	return manBvhPosture;
}

