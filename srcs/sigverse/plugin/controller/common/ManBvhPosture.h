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

		LCHEST_JOINT,
		LSHOULDER_JOINT,
		LELBOW_JOINT,
		LWRIST_JOINT,
		RCHEST_JOINT,
		RSHOULDER_JOINT,
		RELBOW_JOINT,
		RWRIST_JOINT,

		LHIP_JOINT,
		LKNEE_JOINT,
		LANKLE_JOINT,
		RHIP_JOINT,
		RKNEE_JOINT,
		RANKLE_JOINT,

		ManBvhJointType_Count = (RANKLE_JOINT + 1)
	};

	typedef struct _ManBvhJoint
	{
		ManBvhJointType jointType;
		SigCmn::Vector3 angle;
	} ManBvhJoint;


	ManBvhPosture();

	static std::string manBvhJointTypeStr(ManBvhJointType e);

	SigCmn::Vector3 rootPosision;
	ManBvhJoint joint[ManBvhJointType_Count];
};

#endif /* SIGVERSE_MAN_BVH_POSTURE_H */
