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
	static const std::string paramFileKeyPerceptionNeuronIpAddress;
	static const std::string paramFileKeyPerceptionNeuronPort;

private:
	sigverse::SIGService sigService;

	std::string neuronIpAddress;
	int         neuronPort;

	///@brief Read parameter file.
	void readIniFile();

public:
	PerceptionNeuronDevice();
	PerceptionNeuronDevice(int argc, char **argv);
	~PerceptionNeuronDevice();

	///@brief Execute Preception Neuron device.
	int run();


	static void __stdcall bvhFrameDataReceived(void* customedObj, SOCKET_REF sender, BvhDataHeader* header, float* data);

	static void __stdcall socketStatusChanged(void* customedObj, SOCKET_REF sender, SocketStatus status, char* message);

	void sendBvhData(void* customedObj, SOCKET_REF sender, BvhDataHeader* header, float* data);
};

