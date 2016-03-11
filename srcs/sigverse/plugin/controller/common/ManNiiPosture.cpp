/*
 * ManNiiPosture.cpp
 *
 *  Created on: 2015/03/12
 *      Author: Nozaki
 */

#include <sigverse/plugin/controller/common/ManNiiPosture.h>

ManNiiPosture::ManNiiPosture()
{
	this->joint[HEAD_JOINT0].jointType = HEAD_JOINT0;
	this->joint[HEAD_JOINT1].jointType = HEAD_JOINT1;
	this->joint[LARM_JOINT0].jointType = LARM_JOINT0,
	this->joint[LARM_JOINT1].jointType = LARM_JOINT1;
	this->joint[LARM_JOINT2].jointType = LARM_JOINT2;
	this->joint[LARM_JOINT3].jointType = LARM_JOINT3;
	this->joint[LARM_JOINT4].jointType = LARM_JOINT4;
	this->joint[LARM_JOINT5].jointType = LARM_JOINT5;
	this->joint[LARM_JOINT6].jointType = LARM_JOINT6;
	this->joint[LARM_JOINT7].jointType = LARM_JOINT7;
	this->joint[RARM_JOINT0].jointType = RARM_JOINT0;
	this->joint[RARM_JOINT1].jointType = RARM_JOINT1;
	this->joint[RARM_JOINT2].jointType = RARM_JOINT2;
	this->joint[RARM_JOINT3].jointType = RARM_JOINT3;
	this->joint[RARM_JOINT4].jointType = RARM_JOINT4;
	this->joint[RARM_JOINT5].jointType = RARM_JOINT5;
	this->joint[RARM_JOINT6].jointType = RARM_JOINT6;
	this->joint[RARM_JOINT7].jointType = RARM_JOINT7;
	this->joint[WAIST_JOINT0].jointType = WAIST_JOINT0;
	this->joint[WAIST_JOINT1].jointType = WAIST_JOINT1;
	this->joint[WAIST_JOINT2].jointType = WAIST_JOINT2;
	this->joint[ROOT_JOINT0].jointType = ROOT_JOINT0;
	this->joint[ROOT_JOINT1].jointType = ROOT_JOINT1;
	this->joint[ROOT_JOINT2].jointType = ROOT_JOINT2;
	this->joint[LLEG_JOINT2].jointType = LLEG_JOINT2;
	this->joint[LLEG_JOINT4].jointType = LLEG_JOINT4;
	this->joint[LLEG_JOINT6].jointType = LLEG_JOINT6;
	this->joint[RLEG_JOINT2].jointType = RLEG_JOINT2;
	this->joint[RLEG_JOINT4].jointType = RLEG_JOINT4;
	this->joint[RLEG_JOINT6].jointType = RLEG_JOINT6;
	this->joint[LEYE_JOINT1].jointType = LEYE_JOINT1;
	this->joint[LEYE_JOINT0].jointType = LEYE_JOINT0;
	this->joint[REYE_JOINT1].jointType = REYE_JOINT1;
	this->joint[REYE_JOINT0].jointType = REYE_JOINT0;

	for (int i = 0; i < ManNiiJointType_Count; i++)
	{
		this->joint[i].quaternion = Quaternion(0.0, 0.0, 0.0, 0.0);
		this->joint[i].isValid    = false;
	}
}


std::string ManNiiPosture::manNiiJointTypeStr(ManNiiJointType e)
{
	switch(e)
	{
		case HEAD_JOINT0 : { return("HEAD_JOINT0" ); }
		case HEAD_JOINT1 : { return("HEAD_JOINT1" ); }
		case LARM_JOINT0 : { return("LARM_JOINT0" ); }
		case LARM_JOINT1 : { return("LARM_JOINT1" ); }
		case LARM_JOINT2 : { return("LARM_JOINT2" ); }
		case LARM_JOINT3 : { return("LARM_JOINT3" ); }
		case LARM_JOINT4 : { return("LARM_JOINT4" ); }
		case LARM_JOINT5 : { return("LARM_JOINT5" ); }
		case LARM_JOINT6 : { return("LARM_JOINT6" ); }
		case LARM_JOINT7 : { return("LARM_JOINT7" ); }
		case RARM_JOINT0 : { return("RARM_JOINT0" ); }
		case RARM_JOINT1 : { return("RARM_JOINT1" ); }
		case RARM_JOINT2 : { return("RARM_JOINT2" ); }
		case RARM_JOINT3 : { return("RARM_JOINT3" ); }
		case RARM_JOINT4 : { return("RARM_JOINT4" ); }
		case RARM_JOINT5 : { return("RARM_JOINT5" ); }
		case RARM_JOINT6 : { return("RARM_JOINT6" ); }
		case RARM_JOINT7 : { return("RARM_JOINT7" ); }
		case WAIST_JOINT0: { return("WAIST_JOINT0"); }
		case WAIST_JOINT1: { return("WAIST_JOINT1"); }
		case WAIST_JOINT2: { return("WAIST_JOINT2"); }
		case ROOT_JOINT0 : { return("ROOT_JOINT0" ); }
		case ROOT_JOINT1 : { return("ROOT_JOINT1" ); }
		case ROOT_JOINT2 : { return("ROOT_JOINT2" ); }
		case LLEG_JOINT2 : { return("LLEG_JOINT2" ); }
		case LLEG_JOINT4 : { return("LLEG_JOINT4" ); }
		case LLEG_JOINT6 : { return("LLEG_JOINT6" ); }
		case RLEG_JOINT2 : { return("RLEG_JOINT2" ); }
		case RLEG_JOINT4 : { return("RLEG_JOINT4" ); }
		case RLEG_JOINT6 : { return("RLEG_JOINT6" ); }
		case LEYE_JOINT1 : { return("LEYE_JOINT1" ); }
		case LEYE_JOINT0 : { return("LEYE_JOINT0" ); }
		case REYE_JOINT1 : { return("REYE_JOINT1" ); }
		case REYE_JOINT0 : { return("REYE_JOINT0" ); }
		//default: { return("illegal"); }
	}
}


void ManNiiPosture::clearJointValidFlag()
{
	for (int i = 0; i < ManNiiJointType_Count; i++)
	{
		this->joint[i].isValid = false;
	}
}


/*
 * Get initial vector of avatar joints.
 */
SigCmn::Vector3 ManNiiPosture::getSigVec(const SigVec &sigvec)
{
	SigCmn::Vector3 p;

	if (sigvec == WAIST || sigvec == HTOTOR)
	{
		p.x = 0;
		p.y = 1;
		p.z = 0;
	}
	else if (sigvec == RSHOULDER || sigvec == RELBOW)
	{
		p.x = -1;
		p.y = 0;
		p.z = 0;
	}
	else if (sigvec == HIP || sigvec == LSHOULDER || sigvec == LELBOW)
	{
		p.x = 1;
		p.y = 0;
		p.z = 0;
	}
	else if (sigvec == LEG)
	{
		p.x = 0;
		p.y = -1;
		p.z = 0;
	}
	else if (sigvec == FOOT)
	{
		p.x = 0;
		p.y = -0.5;
		p.z = +std::sqrt(3.0)/2.0;
	}
	return p;
}
