/*
 * ManBvhPosture.cpp
 *
 *  Created on: 2016/03/08
 *      Author: Yamada
 */

#include <sigverse/plugin/controller/common/ManBvhPosture.h>

ManBvhPosture::ManBvhPosture()
{
	this->joint[HIP_JOINT      ].jointType = HIP_JOINT;
	this->joint[WAIST_JOINT    ].jointType = WAIST_JOINT;
	this->joint[NECK_JOINT     ].jointType = NECK_JOINT,
	this->joint[LSHOULDER_JOINT].jointType = LSHOULDER_JOINT;
	this->joint[LELBOW_JOINT   ].jointType = LELBOW_JOINT;
	this->joint[LWRIST_JOINT   ].jointType = LWRIST_JOINT;
	this->joint[RSHOULDER_JOINT].jointType = RSHOULDER_JOINT;
	this->joint[RELBOW_JOINT   ].jointType = RELBOW_JOINT;
	this->joint[RWRIST_JOINT   ].jointType = RWRIST_JOINT;
	this->joint[LHIP_JOINT     ].jointType = LHIP_JOINT;
	this->joint[LKNEE_JOINT    ].jointType = LKNEE_JOINT;
	this->joint[LANKLE_JOINT   ].jointType = LANKLE_JOINT;
	this->joint[RHIP_JOINT     ].jointType = RHIP_JOINT;
	this->joint[RKNEE_JOINT    ].jointType = RKNEE_JOINT;
	this->joint[RANKLE_JOINT   ].jointType = RANKLE_JOINT;

	for (int i = 0; i < ManBvhJointType_Count; i++)
	{
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

