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
		HEAD_JOINT1 = 1,
		LARM_JOINT0 = 2,
		LARM_JOINT1 = 3,
		LARM_JOINT2 = 4,
		LARM_JOINT3 = 5,
		LARM_JOINT4 = 6,
		LARM_JOINT5 = 7,
		LARM_JOINT6 = 8,
		LARM_JOINT7 = 9,
		RARM_JOINT0 = 10,
		RARM_JOINT1 = 11,
		RARM_JOINT2 = 12,
		RARM_JOINT3 = 13,
		RARM_JOINT4 = 14,
		RARM_JOINT5 = 15,
		RARM_JOINT6 = 16,
		RARM_JOINT7 = 17,
		WAIST_JOINT0 = 18,
		WAIST_JOINT1 = 19,
		WAIST_JOINT2 = 20,
		ROOT_JOINT0 = 21,
		ROOT_JOINT1 = 22,
		ROOT_JOINT2 = 23,
		LLEG_JOINT2 = 24,
		LLEG_JOINT4 = 25,
		LLEG_JOINT6 = 26,
		RLEG_JOINT2 = 27,
		RLEG_JOINT4 = 28,
		RLEG_JOINT6 = 29,
		LEYE_JOINT1 = 30,
		LEYE_JOINT0 = 31,
		REYE_JOINT1 = 32,
		REYE_JOINT0 = 33,
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
