/*
 * OpenVRDeviceManager.cpp
 *
 *  Created on: 2016/09/10
  */

#include <sigverse/plugin/controller/common/sensor/OpenVRDeviceManager.h>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <cmath>


const double OpenVRDeviceManager::normalization_range = 0.1;

const dQuaternion OpenVRDeviceManager::defaultHeadJoint0Quaternion = { 1.0, 0.0, 0.0, 0.0 };


OpenVRDeviceManager::OpenVRDeviceManager()
{
    this->service        = NULL;
    this->serviceName    = "";
    this->deviceType     = "";
    this->deviceUniqueID = "";
}

OpenVRDeviceManager::OpenVRDeviceManager(std::string &name, std::string &deviceType, std::string &deviceUniqueID)
{
    this->service        = NULL;
    this->serviceName    = name;
    this->deviceType     = deviceType;
    this->deviceUniqueID = deviceUniqueID;
}

void OpenVRDeviceManager::setJointQuaternions2ManNii(SimObj *obj, const ManNiiPosture &posture)
{
    ManNiiPosture::ManNiiJoint joint = posture.joint[ManNiiPosture::HEAD_JOINT0];

    double angle = acos(joint.quaternion.w)*2.0;
    double tmp = sin(angle/2.0);
    double vx = joint.quaternion.x/tmp;
    double vy = joint.quaternion.y/tmp;
    double vz = joint.quaternion.z/tmp;
    double len = sqrt(vx*vx + vy*vy + vz*vz);
    if (len < (1.0-normalization_range) || (1+normalization_range) < len){ return; }

    obj->setJointQuaternion(ManNiiPosture::manNiiJointTypeStr(joint.jointType).c_str(), joint.quaternion.w, joint.quaternion.x, joint.quaternion.y, joint.quaternion.z);
}


void OpenVRDeviceManager::setJointQuaternion(SimObj *obj, const std::string &jointName, const Quaternion &quaternion)
{
    double angle = acos(quaternion.w)*2.0;
    double tmp = sin(angle/2.0);
    double vx = quaternion.x/tmp;
    double vy = quaternion.y/tmp;
    double vz = quaternion.z/tmp;
    double len = sqrt(vx*vx + vy*vy + vz*vz);
    if (len < (1.0-normalization_range) || (1+normalization_range) < len){ return; }

    obj->setJointQuaternion(jointName.c_str(), quaternion.w, quaternion.x, quaternion.y, quaternion.z);
}


ManNiiPosture OpenVRDeviceManager::convertQuaternion2ManNiiPosture(const SigCmn::Vector4 &quaternion)
{
    ManNiiPosture posture;

    dQuaternion tmpQ1;
    tmpQ1[0] = +quaternion.w;
    tmpQ1[1] = -quaternion.x;
    tmpQ1[2] = +quaternion.y;
    tmpQ1[3] = -quaternion.z;

    dQuaternion tmpQ2;

    //dQuaternion defaultQ = this->defaultHeadJointQuaternion;
    dQMultiply1(tmpQ2, defaultHeadJoint0Quaternion, tmpQ1);

    Quaternion tmpQ3(tmpQ2[0], tmpQ2[1], tmpQ2[2], tmpQ2[3]);

    posture.joint[ManNiiPosture::HEAD_JOINT0].jointType  = ManNiiPosture::HEAD_JOINT0;
    posture.joint[ManNiiPosture::HEAD_JOINT0].quaternion = tmpQ3;

    return posture;
}


