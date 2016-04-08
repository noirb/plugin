#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <algorithm>
#include <iterator>
#include <myo/myo.hpp>

#include <sigverse/plugin/plugin/Myo/DataCollector.h>


DataCollector::DataCollector() : onArm(false), roll(0), pitch(0), yaw(0), currentPose()
{
	this->emgData.resize(MyoSensorData::EMG_SENSOR_NUM);
}

// onUnpair() is called whenever the Myo is disconnected from Myo Connect by the user.
void DataCollector::onUnpair(myo::Myo* myo, uint64_t timestamp)
{
	this->roll  = 0;
	this->pitch = 0;
	this->yaw   = 0;
	this->onArm = false;
}

// onOrientationData() is called whenever the Myo device provides its current orientation, which is represented
// as a unit quaternion.
void DataCollector::onOrientationData(myo::Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& quat)
{
	using std::atan2;
	using std::asin;
	using std::sqrt;
	using std::max;
	using std::min;

	// Calculate Euler angles (roll, pitch, and yaw) from the unit quaternion.
	this->roll  = atan2(2.0f * (quat.w() * quat.x() + quat.y() * quat.z()), 1.0f - 2.0f * (quat.x() * quat.x() + quat.y() * quat.y()));
	this->pitch = asin(max(-1.0f, min(1.0f, 2.0f * (quat.w() * quat.y() - quat.z() * quat.x()))));
	this->yaw   = atan2(2.0f * (quat.w() * quat.z() + quat.x() * quat.y()), 1.0f - 2.0f * (quat.y() * quat.y() + quat.z() * quat.z()));
}

// onPose() is called whenever the Myo detects that the person wearing it has changed their pose, for example,
// making a fist, or not making a fist anymore.
void DataCollector::onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose)
{
	this->currentPose = pose;
}

// onArmSync() is called whenever Myo has recognized a Sync Gesture after someone has put it on their
// arm. This lets Myo know which arm it's on and which way it's facing.
void DataCollector::onArmSync(myo::Myo* myo, uint64_t timestamp, myo::Arm arm, myo::XDirection xDirection, float rotation, myo::WarmupState warmupState)
{
	this->onArm = true;
//	this->whichArm = arm;

	std::cout << "onArmSync! (Myo has recognized a Sync Gesture after someone has put it on their arm.)" << std::endl;
}

// onArmUnsync() is called whenever Myo has detected that it was moved from a stable position on a person's arm after
// it recognized the arm. Typically this happens when someone takes Myo off of their arm, but it can also happen
// when Myo is moved around on the arm.
void DataCollector::onArmUnsync(myo::Myo* myo, uint64_t timestamp)
{
	this->onArm = false;

	std::cout << "onArmUnsync! (Myo was moved from a stable position on a person's arm.)" << std::endl;
}


// onEmgData() is called whenever a paired Myo has provided new EMG data, and EMG streaming is enabled.
void DataCollector::onEmgData(myo::Myo* myo, uint64_t timestamp, const int8_t* emg)
{
	for (int i = 0; i < MyoSensorData::EMG_SENSOR_NUM; i++)
	{
		this->emgData[i] = emg[i];
	}
}


MyoSensorData DataCollector::getSensorData()
{
	MyoSensorData sensorData;

	sensorData.roll    = this->roll;
	sensorData.pitch   = this->pitch;
	sensorData.yaw     = this->yaw;
	sensorData.poseStr = this->currentPose.toString();
	std::copy(this->emgData.begin(), this->emgData.end(), back_inserter(sensorData.emgData));

	return sensorData;
}

