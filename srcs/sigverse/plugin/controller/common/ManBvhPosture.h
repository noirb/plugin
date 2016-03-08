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


class ManBvhPosture : public Posture
{
public:
	enum ManBvhJointType
	{
		HIP_JOINT = 0,
		WAIST_JOINT,
		NECK_JOINT,

		LSHOULDER_JOINT,
		LELBOW_JOINT,
		LWRIST_JOINT,
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

	static std::string manBvhJointTypeStr(ManBvhJointType e)
	{
		switch(e)
		{
			case HIP_JOINT       : { return("HIP_JOINT" ); }
			case WAIST_JOINT     : { return("WAIST_JOINT" ); }
			case NECK_JOINT      : { return("NECK_JOINT" ); }
			case LSHOULDER_JOINT : { return("LSHOULDER_JOINT" ); }
			case LELBOW_JOINT    : { return("LELBOW_JOINT" ); }
			case LWRIST_JOINT    : { return("LWRIST_JOINT" ); }
			case RSHOULDER_JOINT : { return("RSHOULDER_JOINT" ); }
			case RELBOW_JOINT    : { return("RELBOW_JOINT" ); }
			case RWRIST_JOINT    : { return("RWRIST_JOINT" ); }
			case LHIP_JOINT      : { return("LHIP_JOINT" ); }
			case LKNEE_JOINT     : { return("LKNEE_JOINT" ); }
			case LANKLE_JOINT    : { return("LANKLE_JOINT" ); }
			case RHIP_JOINT      : { return("RHIP_JOINT" ); }
			case RKNEE_JOINT     : { return("RKNEE_JOINT" ); }
			case RANKLE_JOINT    : { return("RANKLE_JOINT" ); }
			//default: { return("illegal"); }
		}
	}

	typedef struct _ManBvhJoint
	{
		ManBvhJointType jointType;
		SigCmn::Vector3 angle;
	} ManBvhJoint;


	ManBvhPosture();

	SigCmn::Vector3 rootPosision;
	ManBvhJoint joint[ManBvhJointType_Count];
};

#endif /* SIGVERSE_MAN_BVH_POSTURE_H */
