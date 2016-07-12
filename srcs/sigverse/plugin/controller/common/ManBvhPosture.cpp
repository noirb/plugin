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


std::string ManBvhPosture::manBvhJointTypeStr(ManBvhJointType type)
{
	switch(type)
	{
		case HIP_JOINT       : { return("HIP_JOINT" ); }
		case WAIST_JOINT     : { return("WAIST_JOINT" ); }
		case NECK_JOINT      : { return("NECK_JOINT" ); }

		case LCHEST_JOINT    : { return("LCHEST_JOINT" ); }    //It's for Perception Neuron.
		case LSHOULDER_JOINT : { return("LSHOULDER_JOINT" ); }
		case LELBOW_JOINT    : { return("LELBOW_JOINT" ); }
		case LWRIST_JOINT    : { return("LWRIST_JOINT" ); }

		case RCHEST_JOINT    : { return("RCHEST_JOINT" ); }    //It's for Perception Neuron.
		case RSHOULDER_JOINT : { return("RSHOULDER_JOINT" ); }
		case RELBOW_JOINT    : { return("RELBOW_JOINT" ); }
		case RWRIST_JOINT    : { return("RWRIST_JOINT" ); }

		case LHIP_JOINT      : { return("LHIP_JOINT" ); }
		case LKNEE_JOINT     : { return("LKNEE_JOINT" ); }
		case LANKLE_JOINT    : { return("LANKLE_JOINT" ); }

		case RHIP_JOINT      : { return("RHIP_JOINT" ); }
		case RKNEE_JOINT     : { return("RKNEE_JOINT" ); }
		case RANKLE_JOINT    : { return("RANKLE_JOINT" ); }

		// Left Hand
		case LeftH_T1  : { return("LeftH_T1" ); }
		case LeftH_T2  : { return("LeftH_T2" ); }
		case LeftH_T3  : { return("LeftH_T3" ); }
		case LeftH_I1  : { return("LeftH_I1" ); }
		case LeftH_I2  : { return("LeftH_I2" ); }
		case LeftH_I3  : { return("LeftH_I3" ); }
		case LeftH_M1  : { return("LeftH_M1" ); }
		case LeftH_M2  : { return("LeftH_M2" ); }
		case LeftH_M3  : { return("LeftH_M3" ); }
		case LeftH_R1  : { return("LeftH_R1" ); }
		case LeftH_R2  : { return("LeftH_R2" ); }
		case LeftH_R3  : { return("LeftH_R3" ); }
		case LeftH_P1  : { return("LeftH_P1" ); }
		case LeftH_P2  : { return("LeftH_P2" ); }
		case LeftH_P3  : { return("LeftH_P3" ); }

		// Right Hand
		case RightH_T1  : { return("RightH_T1" ); }
		case RightH_T2  : { return("RightH_T2" ); }
		case RightH_T3  : { return("RightH_T3" ); }
		case RightH_I1  : { return("RightH_I1" ); }
		case RightH_I2  : { return("RightH_I2" ); }
		case RightH_I3  : { return("RightH_I3" ); }
		case RightH_M1  : { return("RightH_M1" ); }
		case RightH_M2  : { return("RightH_M2" ); }
		case RightH_M3  : { return("RightH_M3" ); }
		case RightH_R1  : { return("RightH_R1" ); }
		case RightH_R2  : { return("RightH_R2" ); }
		case RightH_R3  : { return("RightH_R3" ); }
		case RightH_P1  : { return("RightH_P1" ); }
		case RightH_P2  : { return("RightH_P2" ); }
		case RightH_P3  : { return("RightH_P3" ); }

		default:
		{
			std::cout << "illegal JointType error in manBvhJointTypeStr(ManBvhJointType e)." << std::endl;
			exit(EXIT_FAILURE);
		}
	}
}

std::string ManBvhPosture::manBvhJointTypeStrX(ManBvhJointType type)
{
	return ManBvhPosture::manBvhJointTypeStr(type) + "_X";
}

std::string ManBvhPosture::manBvhJointTypeStrY(ManBvhJointType type)
{
	return ManBvhPosture::manBvhJointTypeStr(type) + "_Y";
}

std::string ManBvhPosture::manBvhJointTypeStrZ(ManBvhJointType type)
{
	return ManBvhPosture::manBvhJointTypeStr(type) + "_Z";
}
