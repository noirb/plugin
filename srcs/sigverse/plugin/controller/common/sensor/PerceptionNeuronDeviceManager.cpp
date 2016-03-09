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

#define DEG2RAD(DEG) ( (M_PI) * (DEG) / 180.0 )

PerceptionNeuronDeviceManager::PerceptionNeuronDeviceManager()
{
	this->service        = NULL;
	this->serviceName    = "";
	this->deviceType     = "";
	this->deviceUniqueID = "";

	this->yrot = 0.0;
	this->started = false;
}

PerceptionNeuronDeviceManager::PerceptionNeuronDeviceManager(std::string &name, std::string &deviceType, std::string &deviceUniqueID)
{
	this->service        = NULL;
	this->serviceName    = name;
	this->deviceType     = deviceType;
	this->deviceUniqueID = deviceUniqueID;

	this->yrot = 0.0;
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

	// Get initial rotations.
	Rotation rot;
	myself->getRotation(rot);
	double qw = rot.qw();
	double qy = rot.qy();
	this->yrot = acos(fabs(qw))*2.0;
	if (qw*qy > 0){ this->yrot = -1.0*this->yrot; }
}


void PerceptionNeuronDeviceManager::setPosition2ManBvh(SimObj *obj, const ManBvhPosture &manBvhPosture)
{
	//Calculate moving distance from start position.
	SigCmn::Vector3 movingDistance;
	movingDistance.x = +(manBvhPosture.rootPosision.x - this->startpos.x);
	movingDistance.y = +(manBvhPosture.rootPosision.y - this->startpos.y);
	movingDistance.z = +(manBvhPosture.rootPosision.z - this->startpos.z);

	double dx = movingDistance.x;
	double dy = movingDistance.y;
	double dz = movingDistance.z;

	double gx = std::cos(this->yrot)*dx - std::sin(this->yrot)*dz;
	double gz = std::sin(this->yrot)*dx + std::cos(this->yrot)*dz;

//	obj->setPosition(this->iniPos.x + gx,this->iniPos.y + dy,this->iniPos.z + gz);
	obj->setPosition(this->iniPos.x + dx,this->iniPos.y + dy,this->iniPos.z + dz);
}


void PerceptionNeuronDeviceManager::setJointAngle2ManBvh(SimObj *obj, const ManBvhPosture &manBvhPosture, const ManBvhPosture::ManBvhJointType &manBvhJointType)
{
//	if(manBvhJointType==ManBvhPosture::HIP_JOINT)
//	{
////		printf("Start rotation   X : %f ------ Y : %f   ---- Z : %f   ---- end \n", this->startRot.x, this->startRot.y, this->startRot.z);
////		obj->setJointAngle((ManBvhPosture::manBvhJointTypeStr(manBvhJointType)+"_Y").c_str(), DEG2RAD(-this->startRot.z));
//		obj->setJointAngle((ManBvhPosture::manBvhJointTypeStr(manBvhJointType)+"_X").c_str(), DEG2RAD(manBvhPosture.joint[manBvhJointType].angle.y-this->startRot.y));
//		obj->setJointAngle((ManBvhPosture::manBvhJointTypeStr(manBvhJointType)+"_Y").c_str(), DEG2RAD(manBvhPosture.joint[manBvhJointType].angle.z-this->startRot.z));
//		obj->setJointAngle((ManBvhPosture::manBvhJointTypeStr(manBvhJointType)+"_Z").c_str(), DEG2RAD(manBvhPosture.joint[manBvhJointType].angle.x-this->startRot.x));
//	}
//	else
//	{
		obj->setJointAngle((ManBvhPosture::manBvhJointTypeStr(manBvhJointType)+"_X").c_str(), DEG2RAD(manBvhPosture.joint[manBvhJointType].angle.y));
		obj->setJointAngle((ManBvhPosture::manBvhJointTypeStr(manBvhJointType)+"_Y").c_str(), DEG2RAD(manBvhPosture.joint[manBvhJointType].angle.z));
		obj->setJointAngle((ManBvhPosture::manBvhJointTypeStr(manBvhJointType)+"_Z").c_str(), DEG2RAD(manBvhPosture.joint[manBvhJointType].angle.x));
//	}
}


void PerceptionNeuronDeviceManager::setPosture2ManBvh(SimObj *obj, const ManBvhPosture &manBvhPosture)
{
	//Save start position
	if (this->started == false)
	{
		this->startpos.x = manBvhPosture.rootPosision.x;
		this->startpos.y = manBvhPosture.rootPosision.y;
		this->startpos.z = manBvhPosture.rootPosision.z;
		printf("Start position   X : %f ------ Y : %f   ---- Z : %f   ---- end \n", this->startpos.x, this->startpos.y, this->startpos.z);

		this->startRot.x = manBvhPosture.joint[ManBvhPosture::HIP_JOINT].angle.x;
		this->startRot.y = manBvhPosture.joint[ManBvhPosture::HIP_JOINT].angle.y;
		this->startRot.z = manBvhPosture.joint[ManBvhPosture::HIP_JOINT].angle.z;

		this->started = true;
	}

	// Set position.
	this->setPosition2ManBvh(obj, manBvhPosture);

	// Set angles.
	this->setJointAngle2ManBvh(obj, manBvhPosture, ManBvhPosture::HIP_JOINT);
	this->setJointAngle2ManBvh(obj, manBvhPosture, ManBvhPosture::WAIST_JOINT);
	this->setJointAngle2ManBvh(obj, manBvhPosture, ManBvhPosture::NECK_JOINT);

	this->setJointAngle2ManBvh(obj, manBvhPosture, ManBvhPosture::LSHOULDER_JOINT);
	this->setJointAngle2ManBvh(obj, manBvhPosture, ManBvhPosture::LELBOW_JOINT);
	this->setJointAngle2ManBvh(obj, manBvhPosture, ManBvhPosture::LWRIST_JOINT);
	this->setJointAngle2ManBvh(obj, manBvhPosture, ManBvhPosture::RSHOULDER_JOINT);
	this->setJointAngle2ManBvh(obj, manBvhPosture, ManBvhPosture::RELBOW_JOINT);
	this->setJointAngle2ManBvh(obj, manBvhPosture, ManBvhPosture::RWRIST_JOINT);

	this->setJointAngle2ManBvh(obj, manBvhPosture, ManBvhPosture::LHIP_JOINT);
	this->setJointAngle2ManBvh(obj, manBvhPosture, ManBvhPosture::LKNEE_JOINT);
	this->setJointAngle2ManBvh(obj, manBvhPosture, ManBvhPosture::LANKLE_JOINT);
	this->setJointAngle2ManBvh(obj, manBvhPosture, ManBvhPosture::RHIP_JOINT);
	this->setJointAngle2ManBvh(obj, manBvhPosture, ManBvhPosture::RKNEE_JOINT);
	this->setJointAngle2ManBvh(obj, manBvhPosture, ManBvhPosture::RANKLE_JOINT);
}


ManBvhPosture PerceptionNeuronDeviceManager::convertSensorData2ManBvhPosture(const PerceptionNeuronSensorData &sensorData)
{
	ManBvhPosture manBvhPosture;

	manBvhPosture.rootPosision = sensorData.rootPosition;

	manBvhPosture.joint[ManBvhPosture::HIP_JOINT  ].angle = sensorData.jointRotations[PerceptionNeuronSensorData::Hips  ].rotation;
	manBvhPosture.joint[ManBvhPosture::WAIST_JOINT].angle = sensorData.jointRotations[PerceptionNeuronSensorData::Spine1].rotation;
	manBvhPosture.joint[ManBvhPosture::NECK_JOINT ].angle = sensorData.jointRotations[PerceptionNeuronSensorData::Neck  ].rotation;

	manBvhPosture.joint[ManBvhPosture::LSHOULDER_JOINT].angle = sensorData.jointRotations[PerceptionNeuronSensorData::LeftArm     ].rotation;
	manBvhPosture.joint[ManBvhPosture::LELBOW_JOINT   ].angle = sensorData.jointRotations[PerceptionNeuronSensorData::LeftForeArm ].rotation;
	manBvhPosture.joint[ManBvhPosture::LWRIST_JOINT   ].angle = sensorData.jointRotations[PerceptionNeuronSensorData::LeftHand    ].rotation;
	manBvhPosture.joint[ManBvhPosture::RSHOULDER_JOINT].angle = sensorData.jointRotations[PerceptionNeuronSensorData::RightArm    ].rotation;
	manBvhPosture.joint[ManBvhPosture::RELBOW_JOINT   ].angle = sensorData.jointRotations[PerceptionNeuronSensorData::RightForeArm].rotation;
	manBvhPosture.joint[ManBvhPosture::RWRIST_JOINT   ].angle = sensorData.jointRotations[PerceptionNeuronSensorData::RightHand   ].rotation;

	manBvhPosture.joint[ManBvhPosture::LHIP_JOINT  ].angle = sensorData.jointRotations[PerceptionNeuronSensorData::LeftUpLeg ].rotation;
	manBvhPosture.joint[ManBvhPosture::LKNEE_JOINT ].angle = sensorData.jointRotations[PerceptionNeuronSensorData::LeftLeg   ].rotation;
	manBvhPosture.joint[ManBvhPosture::LANKLE_JOINT].angle = sensorData.jointRotations[PerceptionNeuronSensorData::LeftFoot  ].rotation;
	manBvhPosture.joint[ManBvhPosture::RHIP_JOINT  ].angle = sensorData.jointRotations[PerceptionNeuronSensorData::RightUpLeg].rotation;
	manBvhPosture.joint[ManBvhPosture::RKNEE_JOINT ].angle = sensorData.jointRotations[PerceptionNeuronSensorData::RightLeg  ].rotation;
	manBvhPosture.joint[ManBvhPosture::RANKLE_JOINT].angle = sensorData.jointRotations[PerceptionNeuronSensorData::RightFoot ].rotation;

	return manBvhPosture;
}
