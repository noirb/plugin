/*
 * ManNiiPosture.h
 *
 *  Created on: 2015/03/12
 *      Author: Nozaki
 */

#ifndef SIGVERSE_MAN_NII_POSTURE_H
#define SIGVERSE_MAN_NII_POSTURE_H

#include <sigverse/plugin/controller/common/Posture.h>
#include <sigverse/plugin/common/Quaternion.h>
#include <string>
#include <sstream>


class ManNiiPosture : public Posture
{
public:
	enum ManNiiJointType
	{
		HEAD_JOINT0 = 0,
		HEAD_JOINT1,
		LARM_JOINT0,
		LARM_JOINT1,
		LARM_JOINT2,
		LARM_JOINT3,
		LARM_JOINT4,
		LARM_JOINT5,
		LARM_JOINT6,
		LARM_JOINT7,
		RARM_JOINT0,
		RARM_JOINT1,
		RARM_JOINT2,
		RARM_JOINT3,
		RARM_JOINT4,
		RARM_JOINT5,
		RARM_JOINT6,
		RARM_JOINT7,
		WAIST_JOINT0,
		WAIST_JOINT1,
		WAIST_JOINT2,
		ROOT_JOINT0,
		ROOT_JOINT1,
		ROOT_JOINT2,
		LLEG_JOINT2,
		LLEG_JOINT4,
		LLEG_JOINT6,
		RLEG_JOINT2,
		RLEG_JOINT4,
		RLEG_JOINT6,
		LEYE_JOINT1,
		LEYE_JOINT0,
		REYE_JOINT1,
		REYE_JOINT0,
		ManNiiJointType_Count = (REYE_JOINT0 + 1)
	};

	//Avatar joints
	enum SigVec
	{
		HIP = 0,
		HTOTOR,
		WAIST,
		RSHOULDER,
		LSHOULDER,
		RELBOW,
		LELBOW,
		LEG,
		FOOT
	};

	typedef struct _ManNiiJoint
	{
		ManNiiJointType jointType;
		Quaternion quaternion;
		bool isValid;
	} ManNiiJoint;


	static std::string manNiiJointTypeStr(ManNiiJointType e);

	static SigCmn::Vector3 getSigVec(const SigVec &sigvec);

	ManNiiPosture();
	void clearJointValidFlag();

	ManNiiJoint joint[ManNiiJointType_Count];
};

#endif /* SIGVERSE_MAN_NII_POSTURE_H */
