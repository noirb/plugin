/*
 * Quaternion.cpp
 *
 *  Created on: 2015/07/01
 *      Author: tome-yamada
 */

#include <sigverse/plugin/common/Quaternion.h>
#include <sstream>
#include <cmath>


Quaternion::Quaternion()
{
	this->w = 0.0;
	this->x = 0.0;
	this->y = 0.0;
	this->z = 0.0;
};

Quaternion::~Quaternion()
{
};


Quaternion::Quaternion(const double w, const double x, const double y, const double z)
{
	this->w = w;
	this->x = x;
	this->y = y;
	this->z = z;
};

Quaternion::Quaternion(const Quaternion &v)
{
	this->w = v.w;
	this->x = v.x;
	this->y = v.y;
	this->z = v.z;
};


void Quaternion::getQuaternion(double &w, double &x, double &y, double &z) const
{
	w = this->w;
	x = this->x;
	y = this->y;
	z = this->z;
}

void Quaternion::setQuaternion(const double w, const double x, const double y, const double z)
{
	this->w = w;
	this->x = x;
	this->y = y;
	this->z = z;
}

///@param[in] qa Input quaternion.
///@param[in] qb Input quaternion.
///@return Cross product of two input quaternions.
const Quaternion Quaternion::calcCrossProduct(const Quaternion &qa, const Quaternion &qb)
{
	Quaternion tmp;

	tmp.w = + qa.w * qb.w - qa.x * qb.x - qa.y * qb.y - qa.z * qb.z;
	tmp.x = + qa.w * qb.x + qa.x * qb.w + qa.y * qb.z - qa.z * qb.y;
	tmp.y = + qa.w * qb.y + qa.y * qb.w + qa.z * qb.x - qa.x * qb.z;
	tmp.z = + qa.w * qb.z + qa.z * qb.w + qa.x * qb.y - qa.y * qb.x;

	return tmp;
};

///@param[in] qa Input quaternion.
///@param[in] qb Input quaternion.
///@return rotate by qb, then qa.
const Quaternion Quaternion::multiplyNN(const Quaternion &qa, const Quaternion &qb)
{
	return calcCrossProduct(qa, qb);
};

///@param[in] qa Input quaternion.
///@param[in] qb Input quaternion.
///@return rotate by qb, then inverse of qa.
const Quaternion Quaternion::multiplyNI(const Quaternion &qa, const Quaternion &qb)
{
	Quaternion tmp;

	tmp.w = + qa.w * qb.w + qa.x * qb.x + qa.y * qb.y + qa.z * qb.z;
	tmp.x = + qa.w * qb.x - qa.x * qb.w - qa.y * qb.z + qa.z * qb.y;
	tmp.y = + qa.w * qb.y - qa.y * qb.w - qa.z * qb.x + qa.x * qb.z;
	tmp.z = + qa.w * qb.z - qa.z * qb.w - qa.x * qb.y + qa.y * qb.x;

	return tmp;
};

///@param[in] qa Input quaternion.
///@param[in] qb Input quaternion.
///@return rotate by inverse of qb, then qa.
const Quaternion Quaternion::multiplyIN(const Quaternion &qa, const Quaternion &qb)
{
	Quaternion tmp;

	tmp.w = + qa.w * qb.w + qa.x * qb.x + qa.y * qb.y + qa.z * qb.z;
	tmp.x = - qa.w * qb.x + qa.x * qb.w - qa.y * qb.z + qa.z * qb.y;
	tmp.y = - qa.w * qb.y + qa.y * qb.w - qa.z * qb.x + qa.x * qb.z;
	tmp.z = - qa.w * qb.z + qa.z * qb.w - qa.x * qb.y + qa.y * qb.x;

	return tmp;
};

///@param[in] qa Input quaternion.
///@param[in] qb Input quaternion.
///@return rotate by inverse of qb, then inverse of qa.
const Quaternion Quaternion::multiplyII(const Quaternion &qa, const Quaternion &qb)
{
	Quaternion tmp;

	tmp.w = + qa.w * qb.w - qa.x * qb.x - qa.y * qb.y - qa.z * qb.z;
	tmp.x = - qa.w * qb.x - qa.x * qb.w + qa.y * qb.z - qa.z * qb.y;
	tmp.y = - qa.w * qb.y - qa.y * qb.w + qa.z * qb.x - qa.x * qb.z;
	tmp.z = - qa.w * qb.z - qa.z * qb.w + qa.x * qb.y - qa.y * qb.x;

	return tmp;
};



const Quaternion Quaternion::calcInverse(const Quaternion &q)
{
	double norm2 = q.w*q.w + q.x*q.x + q.y*q.y + q.z*q.z;

	if(norm2==0){ return Quaternion(1.0, 0.0, 0.0, 0.0); }

	Quaternion tmp;
	tmp.w = +q.w / norm2;
	tmp.x = -q.x / norm2;
	tmp.y = -q.y / norm2;
	tmp.z = -q.z / norm2;
	return tmp;
};


/*
 * Calculate Quaternion from 2 Vectors
 */
const Quaternion Quaternion::calcQuaternionFromVector(const SigCmn::Vector3 &kvec, const SigCmn::Vector3 &svec)
{
	Quaternion q;

	if (kvec.x == svec.x && kvec.y == svec.y && kvec.z == svec.z)
	{
		q.w = 1;
		q.x = 0;
		q.y = 0;
		q.z = 0;
		return q;
	}

	double x = kvec.y*svec.z - kvec.z*svec.y;
	double y = kvec.z*svec.x - kvec.x*svec.z;
	double z = kvec.x*svec.y - kvec.y*svec.x;

	//Normalization
	double sum = std::sqrt(x*x + y*y + z*z);
	x = x / sum;
	y = y / sum;
	z = z / sum;

	double angle = std::acos(kvec.x*svec.x + kvec.y*svec.y + kvec.z*svec.z);
	q.w = (float)(std::cos(angle / 2));
	q.x = (float)(x*std::sin(angle / 2));
	q.y = (float)(y*std::sin(angle / 2));
	q.z = (float)(z*std::sin(angle / 2));

	return q;
}


/*
 * Rotate Vector by Quaternion
 */
void Quaternion::rotVec(SigCmn::Vector3 &v, const Quaternion &q)
{
	double rx = v.x *  q.w + v.y * -q.z + v.z *  q.y;
	double ry = v.x *  q.z + v.y *  q.w + v.z * -q.x;
	double rz = v.x * -q.y + v.y *  q.x + v.z *  q.w;
	double rw = v.x *  q.x + v.y *  q.y + v.z *  q.z;

	v.x = (float)(q.x *  rw + q.y *  rz + q.z * -ry + q.w * rx);
	v.y = (float)(q.x * -rz + q.y *  rw + q.z *  rx + q.w * ry);
	v.z = (float)(q.x *  ry + q.y * -rx + q.z *  rw + q.w * rz);
}

