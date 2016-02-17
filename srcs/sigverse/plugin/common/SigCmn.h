/*
 * SigCmn.h
 *
 *  Created on: 2015/07/01
 *      Author: tome-yamada
 */

#ifndef SIGVERSE_SIG_CMN_H
#define SIGVERSE_SIG_CMN_H

#include <string>
#include <sstream>
#include <cmath>


class SigCmn
{
public:

	///@brief For expression position.
	struct Vector3
	{
		float x;
		float y;
		float z;

		void operator+=(const Vector3 &vec)
		{
			x += vec.x;
			y += vec.y;
			z += vec.z;
		}

		void operator-=(const Vector3 &vec)
		{
			x -= vec.x;
			y -= vec.y;
			z -= vec.z;
		}

		void operator*=(double val)
		{
			x *= val;
			y *= val;
			z *= val;
		}

		void operator/=(double val)
		{
			x /= val;
			y /= val;
			z /= val;
		}
	};

	///@brief For expression position.
	struct Vector4
	{
		float w;
		float x;
		float y;
		float z;
	};

	///@brief For expression position.
	struct EulerAngleType
	{
		float yaw;
		float pitch;
		float roll;
	};

	///@brief Calculate Vector from 2 position.
	static bool diffVec(SigCmn::Vector3 &rvec, const SigCmn::Vector3 &jvec, const SigCmn::Vector3 &kvec);

	///@brief Translate degree to radian.
	static const double deg2rad(const double deg);
};

#endif /* SIGVERSE_SIG_CMN_H */
