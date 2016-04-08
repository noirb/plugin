#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <vector>
#include <algorithm>

#include <myo/myo.hpp>
#include <sigverse/plugin/common/sensor/MyoSensorData.h>

class DataCollector : public myo::DeviceListener 
{
public:
	DataCollector();

	// onUnpair() is called whenever the Myo is disconnected from Myo Connect by the user.
	void onUnpair(myo::Myo* myo, uint64_t timestamp);

	// onOrientationData() is called whenever the Myo device provides its current orientation, which is represented
	// as a unit quaternion.
	void onOrientationData(myo::Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& quat);

	// onPose() is called whenever the Myo detects that the person wearing it has changed their pose, for example,
	// making a fist, or not making a fist anymore.
	void onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose);

	// onArmSync() is called whenever Myo has recognized a Sync Gesture after someone has put it on their
	// arm. This lets Myo know which arm it's on and which way it's facing.
	void onArmSync(myo::Myo* myo, uint64_t timestamp, myo::Arm arm, myo::XDirection xDirection, float rotation, myo::WarmupState warmupState);

	// onArmUnsync() is called whenever Myo has detected that it was moved from a stable position on a person's arm after
	// it recognized the arm. Typically this happens when someone takes Myo off of their arm, but it can also happen
	// when Myo is moved around on the arm.
	void onArmUnsync(myo::Myo* myo, uint64_t timestamp);

	void onEmgData(myo::Myo* myo, uint64_t timestamp, const int8_t* emg);

	MyoSensorData getSensorData();

	// These values are set by onArmSync() and onArmUnsync() above.
	bool onArm;
//	myo::Arm whichArm;

	// These values are set by onOrientationData() and onPose() above.
	float roll, pitch, yaw;
	myo::Pose currentPose;

	// The values of this array is set by onEmgData() above.
	std::vector<int> emgData;
};