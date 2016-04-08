#ifndef SIGVERSE_MYO_SENSOR_DATA_H
#define SIGVERSE_MYO_SENSOR_DATA_H

#include <sigverse/plugin/common/sensor/SensorData.h>
#include <sigverse/plugin/common/Quaternion.h>
#include <sigverse/plugin/common/SigCmn.h>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>


///@brief Sensor data class for using a Myo device.
class MyoSensorData : public SensorData
{
public:
	static const int EMG_SENSOR_NUM = 8;

	enum PoseType
	{
		Rest,
		Fist,
		WaveIn,
		WaveOut,
		FingersSpread,
		DoubleTap,
		Unknown,
	};

	enum MsgItemEnum
	{
		Orientation,
		Pose,
		EmgData,
	};

	static std::map<MsgItemEnum, std::string> mapMsgItemEnum2Str;
	static std::map<std::string, MsgItemEnum> mapMsgItemStr2Enum;
	static std::map<PoseType,std::string> mapPoseType2Str;

public:
	float roll, pitch, yaw;
	std::string poseStr;
	std::vector<int> emgData;

	///@brief Constructor.
	MyoSensorData();

	///@brief Convert sensor information to string.
	///@param itemsDelim Symbol to split items.
	///@param keyValueDelim Symbol to split KEY and VALUE.
	///@param valuesDelim Symbol to split each component of VALUE. When VALUE is multidimensional data, you use this symbol.
	std::string encodeSensorData(const std::string &itemsDelim = ITEMS_DELIMITER, const std::string &keyValueDelim = KEY_AND_VALUE_DELIMITER, const std::string &valuesDelim = VALUES_DELIMITER) const;

	///@brief Get a Emg data string for sending message.
	std::string getEmgDataString(const std::vector<int> &data, const std::string &valuesDelim = VALUES_DELIMITER) const;

	///@brief Set to sensor data.
	///@param Map of sensor data;
	bool setSensorData(const std::map<std::string, std::vector<std::string> > &sensorDataMap);
};

#endif // SIGVERSE_MYO_SENSOR_DATA_H


