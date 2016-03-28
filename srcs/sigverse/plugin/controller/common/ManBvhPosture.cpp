/*
 * ManBvhPosture.cpp
 *
 *  Created on: 2016/03/08
 *      Author: Yamada
 */

#include <sigverse/plugin/controller/common/ManBvhPosture.h>

ManBvhPosture::ManBvhPosture()
{
	for (int i = 0; i < ManBvhJointType_Count; i++)
	{
		this->joint[i].jointType = (ManBvhJointType)i;
		this->joint[i].angle.x = 0.0;
		this->joint[i].angle.y = 0.0;
		this->joint[i].angle.z = 0.0;
	}
}


std::string ManBvhPosture::manBvhJointTypeStr(ManBvhJointType e)
{
	switch(e)
	{
		case HIP_JOINT       : { return("HIP_JOINT" ); }
		case WAIST_JOINT     : { return("WAIST_JOINT" ); }
		case NECK_JOINT      : { return("NECK_JOINT" ); }
		case LCHEST_JOINT    : { return("LCHEST_JOINT" ); }
		case LSHOULDER_JOINT : { return("LSHOULDER_JOINT" ); }
		case LELBOW_JOINT    : { return("LELBOW_JOINT" ); }
		case LWRIST_JOINT    : { return("LWRIST_JOINT" ); }
		case RCHEST_JOINT    : { return("RCHEST_JOINT" ); }
		case RSHOULDER_JOINT : { return("RSHOULDER_JOINT" ); }
		case RELBOW_JOINT    : { return("RELBOW_JOINT" ); }
		case RWRIST_JOINT    : { return("RWRIST_JOINT" ); }
		case LHIP_JOINT      : { return("LHIP_JOINT" ); }
		case LKNEE_JOINT     : { return("LKNEE_JOINT" ); }
		case LANKLE_JOINT    : { return("LANKLE_JOINT" ); }
		case RHIP_JOINT      : { return("RHIP_JOINT" ); }
		case RKNEE_JOINT     : { return("RKNEE_JOINT" ); }
		case RANKLE_JOINT    : { return("RANKLE_JOINT" ); }
		default:
		{
			std::cout << "illegal JointType error in manBvhJointTypeStr(ManBvhJointType e)." << std::endl;
			exit(EXIT_FAILURE);
		}
	}
}

