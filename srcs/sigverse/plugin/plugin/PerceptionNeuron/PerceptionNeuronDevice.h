#pragma once

#include "NeuronDataReader.h"
#include <SIGService.h>
#include <sigverse/plugin/common/sensor/PerceptionNeuronSensorData.h>
#include <sigverse/plugin/plugin/common/Device.h>
//#include <Kinect.h>
//#include <opencv2/opencv.hpp>



//template<class Interface>
//inline void SafeRelease(Interface *& pInterfaceToRelease)
//{
//	if (pInterfaceToRelease != NULL)
//	{
//		pInterfaceToRelease->Release();
//		pInterfaceToRelease = NULL;
//	}
//}

//#define ENUMSTR(val) #val


class PerceptionNeuronDevice : public Device
{
public:
	//enum
	//{ 
	//	BVHBoneCount  = 59,
	//	CalcBoneCount = 21,
	//};

	//enum SmoothingType
	//{
	//	NONE,
	//	SMA,
	//	WMA,
	//};

	///@brief Parameter file name.
	static const std::string parameterFileName;

	//Parameter file info (key)
	static const std::string paramFileKeyPerceptionNeuronIpAddress;
	static const std::string paramFileKeyPerceptionNeuronPort;
	//static const std::string paramFileKeyPerceptionNeuronSmoothingType;
	//static const std::string paramFileKeyPerceptionNeuronSmoothingSMANum;
	//static const std::string paramFileKeyPerceptionNeuronSmoothingWMAWeight;

private:
	sigverse::SIGService sigService;

	std::string neuronIpAddress;
	int         neuronPort;

//	bool sendHandState;
//	SmoothingType smoothingType;
	//int    smoothingSMANum;
	//double smoothingWMAWeight;

	//int colorFrameWidth;
	//int colorFrameHeight;

	///@brief Read parameter file.
	void readIniFile();

//	void handStateProcessing(const JointType &hand, const HandState &handState, ICoordinateMapper* &coordinateMapper, Joint* joint, cv::Mat &image);

	/////@brief Set whole body joint positions.
	//void convertJointPositions2PerceptionNeuronJointPositions(Joint *joints, PerceptionNeuronSensorData::PerceptionNeuronJointPosition *PerceptionNeuronJointPositions);

	/////@brief Set whole body joint orientations.
	//void convertJointOrientations2PerceptionNeuronJointOrientations(JointOrientation *orientations, PerceptionNeuronSensorData::PerceptionNeuronJointOrientation *PerceptionNeuronJointOrientations);

	/////@brief Convert JointType to PerceptionNeuronJointType.
	//PerceptionNeuronSensorData::PerceptionNeuronJointType getPerceptionNeuronJointType(const JointType jointType);

	/////@brief Set smoothing information.
	//void setSmoothingInfo(std::string typeStr, std::string smaNumStr, std::string wmaWeightStr);

	/////@brief Only get latest sensor data (No Smoothing)
	//PerceptionNeuronSensorData getLatestSensorData(const std::vector<PerceptionNeuronSensorData> &sensorDataList);

	/////@brief Smoothing by Simple Moving Average. (Only for POSITION mode)
	//PerceptionNeuronSensorData smoothingBySMA(const std::vector<PerceptionNeuronSensorData> &sensorDataList);

	/////@brief Smoothing by Weighted Moving Average. (Only for POSITION mode)
	//PerceptionNeuronSensorData smoothingByWMA(const std::vector<PerceptionNeuronSensorData> &sensorDataList);

public:
	PerceptionNeuronDevice();
	PerceptionNeuronDevice(int argc, char **argv);
	~PerceptionNeuronDevice();

	///@brief Execute Preception Neuron device.
	int run();


//	std::vector<PerceptionNeuronSensorData> sensorDataList;
//	unsigned int sensorDataListNum;

	static void __stdcall bvhFrameDataReceived(void* customedObj, SOCKET_REF sender, BvhDataHeader* header, float* data);

	static void __stdcall socketStatusChanged(void* customedObj, SOCKET_REF sender, SocketStatus status, char* message);

	void sendBvhData(void* customedObj, SOCKET_REF sender, BvhDataHeader* header, float* data);

//
//#ifdef DEBUG_PRINT
//	typedef struct TIME_INFO 
//	{
//		SYSTEMTIME systemTime;
//		float val;
//	};
//
//	void debugPrint();
//	std::vector<TIME_INFO> timeInfoList;
//#endif
};

