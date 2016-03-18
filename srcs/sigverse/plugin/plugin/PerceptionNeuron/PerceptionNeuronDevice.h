#pragma once

#include "NeuronDataReader.h"
#include <SIGService.h>
#include <sigverse/plugin/common/sensor/PerceptionNeuronSensorData.h>
#include <sigverse/plugin/plugin/common/Device.h>

class PerceptionNeuronDevice : public Device
{
public:
	///@brief Parameter file name.
	static const std::string parameterFileName;

	//Parameter file info (key)
	static const std::string paramFileKeyPerceptionNeuronDataType;
	static const std::string paramFileKeyPerceptionNeuronBvhIpAddress;
	static const std::string paramFileKeyPerceptionNeuronBvhPort;
	static const std::string paramFileKeyPerceptionNeuronCalcIpAddress;
	static const std::string paramFileKeyPerceptionNeuronCalcPort;

private:
	sigverse::SIGService sigService;

	std::string dataType;
	std::string neuronBvhIpAddress;
	int         neuronBvhPort;
	std::string neuronCalcIpAddress;
	int         neuronCalcPort;

	///@brief Read parameter file.
	void readIniFile();

public:
	PerceptionNeuronDevice();
	PerceptionNeuronDevice(int argc, char **argv);
	~PerceptionNeuronDevice();

	///@brief Execute Preception Neuron device.
	int run();

	void connect4Bvh(SOCKET_REF sockTCPRef);
	void connect4Calc(SOCKET_REF sockTCPRef);

	///@brief Callback function for receiving BVH data.
	static void __stdcall bvhFrameDataReceived(void* customedObj, SOCKET_REF sender, BvhDataHeader* header, float* data);

	///@brief Callback function for receiving Calculation data.
	static void __stdcall calcDataReceived(void* customedObj, SOCKET_REF sender, CalcDataHeader* header, float* data);

	///@brief Callback function for receiving Socket status.
	static void __stdcall socketStatusChanged(void* customedObj, SOCKET_REF sender, SocketStatus status, char* message);

	///@brief Send BVH data.
	void sendBvhData(void* customedObj, SOCKET_REF sender, BvhDataHeader* header, float* data);

	///@brief Send Calculation data.
	void sendCalcData(void* customedObj, SOCKET_REF sender, CalcDataHeader* header, float* data);
};

