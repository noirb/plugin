/*
 * ManBvhPosture.h
 *
 *  Created on: 2016/03/08
 *      Author: Yamada
 */

#ifndef SIGVERSE_MAN_BVH_POSTURE_H
#define SIGVERSE_MAN_BVH_POSTURE_H

#include <sigverse/plugin/common/SigCmn.h>
#include <sigverse/plugin/controller/common/Posture.h>
#include <string>
#include <sstream>
#include <iostream>

class ManBvhPosture : public Posture
{
public:
	enum ManBvhJointType
	{
		HIP_JOINT = 0,
		WAIST_JOINT,
		NECK_JOINT,

		LCHEST_JOINT,   //It's for Perception Neuron.
		LSHOULDER_JOINT,
		LELBOW_JOINT,
		LWRIST_JOINT,
		RCHEST_JOINT,   //It's for Perception Neuron.
		RSHOULDER_JOINT,
		RELBOW_JOINT,
		RWRIST_JOINT,

		LHIP_JOINT,
		LKNEE_JOINT,
		LANKLE_JOINT,
		RHIP_JOINT,
		RKNEE_JOINT,
		RANKLE_JOINT,

		// left Hand
		LeftH_T1,
		LeftH_T2,
		LeftH_T3,
		LeftH_I1,
		LeftH_I2,
		LeftH_I3,
		LeftH_M1,
		LeftH_M2,
		LeftH_M3,
		LeftH_R1,
		LeftH_R2,
		LeftH_R3,
		LeftH_P1,
		LeftH_P2,
		LeftH_P3,

		// right Hand
		RightH_T1,
		RightH_T2,
		RightH_T3,
		RightH_I1,
		RightH_I2,
		RightH_I3,
		RightH_M1,
		RightH_M2,
		RightH_M3,
		RightH_R1,
		RightH_R2,
		RightH_R3,
		RightH_P1,
		RightH_P2,
		RightH_P3,

		ManBvhJointType_Count = (RightH_P3 + 1)
	};

	typedef struct _ManBvhJoint
	{
		_ManBvhJoint() { isValid   = true; };

		ManBvhJointType jointType;
		bool            isValid;
		SigCmn::Vector3 angle;
	} ManBvhJoint;


	ManBvhPosture();

	static std::string manBvhJointTypeStr(ManBvhJointType e);

	SigCmn::Vector3 rootPosision;
	ManBvhJoint joint[ManBvhJointType_Count];
};

#endif /* SIGVERSE_MAN_BVH_POSTURE_H */
