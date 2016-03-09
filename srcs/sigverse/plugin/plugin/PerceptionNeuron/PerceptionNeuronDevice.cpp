#include <sigverse/plugin/plugin/PerceptionNeuron/PerceptionNeuronDevice.h>
#include <sigverse/plugin/plugin/common/CheckRecvSIGServiceData.h>

#include <iostream>
#include <fstream>
#include <boost/thread.hpp>
#include <boost/thread/thread.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <conio.h>


///@brief Parameter file name.
const std::string PerceptionNeuronDevice::parameterFileName = "PerceptionNeuron.ini";

//Parameter file information
const std::string PerceptionNeuronDevice::paramFileKeyPerceptionNeuronIpAddress    = "PerceptionNeuron.ip_address";
const std::string PerceptionNeuronDevice::paramFileKeyPerceptionNeuronPort         = "PerceptionNeuron.port";


///@brief Default Constructor
PerceptionNeuronDevice::PerceptionNeuronDevice()
{
}


///@brief Constructor
PerceptionNeuronDevice::PerceptionNeuronDevice(int argc, char **argv)
{
	if (argc == 3) 
	{
		std::cout << "SIGServer IP address:" << argv[1] << std::endl;
		std::cout << "Port number: " << argv[2] << std::endl;
		this->sendMessageFlag = true;
		this->serverAddress = argv[1];
		this->portNumber = atoi(argv[2]);
	}
	else
	{
		std::cout << "Please set the arguments (IP address, port number)." << std::endl;
		exit(-1);
	}
}


PerceptionNeuronDevice::~PerceptionNeuronDevice()
{
}


///@brief Execute Preception Neuron device.
int PerceptionNeuronDevice::run()
{
	try 
	{
		// Read the initialize file.
		this->readIniFile();

		// Prepare to use SIGService.
		this->sigService.setName(this->serviceName);
		this->initializeSigService(sigService);

		// check receive SIGService data by another thread
		CheckRecvSIGServiceData checkRecvSIGServiceData;
		boost::thread thCheckRecvData(&CheckRecvSIGServiceData::run, &checkRecvSIGServiceData, &this->sigService);

		// connect to Perception Neuron
		SOCKET_REF sockTCPRef = NULL;

		int neuronIpAddressLen = this->neuronIpAddress.length();
		char* neuronIpAddressChar = new char[neuronIpAddressLen+1];
		memcpy(neuronIpAddressChar, this->neuronIpAddress.c_str(), neuronIpAddressLen+1);

		sockTCPRef = BRConnectTo(neuronIpAddressChar, this->neuronPort);

		if (sockTCPRef)
		{
			std::cout << "Connection is OK" << std::endl;
		}
		else
		{
			std::cout << "Connection is NG" << std::endl;
			return;
		}

		// Receive BVH data by the callback function.
		BRRegisterFrameDataCallback(this, this->bvhFrameDataReceived);

		// Receive Socket status by the callback function.
		BRRegisterSocketStatusCallback(this, this->socketStatusChanged);
			
		delete neuronIpAddressChar;

		while (true)
		{
			// Quit
			if (GetAsyncKeyState('Q') || GetAsyncKeyState(VK_ESCAPE))
			{
				while (_kbhit() != 0){ _getch(); }
				break;
			}
			Sleep(100);
		}

		// close socket
		BRCloseSocket(sockTCPRef);

		sigService.disconnect();
	}
	catch (std::exception &ex) 
	{
		std::cout << "run ERR :" << ex.what() << std::endl;
		throw ex;
	}

	return 0;
}


void PerceptionNeuronDevice::sendBvhData(void* customedObj, SOCKET_REF sender, BvhDataHeader* header, float* data)
{
	PerceptionNeuronDevice* pthis = (PerceptionNeuronDevice*)customedObj;

	//Execute smoothing 
	PerceptionNeuronSensorData sensorData;

	sensorData.bvhData.avatarIndex   = header->AvatarIndex;
	sensorData.bvhData.avatarName    = std::string((char*)header->AvatarName);
	sensorData.bvhData.withDisp      = (header->WithDisp==1);
	sensorData.bvhData.withReference = (header->WithReference==1);
	sensorData.bvhData.frameIndex    = header->FrameIndex;
	sensorData.bvhData.dataCount     = header->DataCount;
	sensorData.bvhData.data          = data;

	// Send message to SigServer.
	std::string messageHeader = this->generateMessageHeader();
	std::string sensorDataMessage = sensorData.encodeSensorData();
	std::string message = messageHeader + sensorDataMessage;
	this->sendMessage(pthis->sigService, message);
}

void __stdcall PerceptionNeuronDevice::bvhFrameDataReceived(void* customedObj, SOCKET_REF sender, BvhDataHeader* header, float* data)
{
	PerceptionNeuronDevice* pthis = (PerceptionNeuronDevice*)customedObj;
	pthis->sendBvhData(customedObj, sender, header, data);
}

void __stdcall PerceptionNeuronDevice::socketStatusChanged(void* customedObj, SOCKET_REF sender, SocketStatus status, char* message)
{
	switch (status)
	{
		case CS_Running:
		{
			std::cout << "Socket is working correctly." << std::endl;
			break;
		}
		case CS_Starting:
		{
			std::cout << "Is trying to start service." << std::endl;
			break;
		}
		case CS_OffWork:
		{
			std::cout << "Not working." << std::endl;
			break;
		}
	}
}




///@brief Read parameter file.
void PerceptionNeuronDevice::readIniFile()
{
	try 
	{
		std::ifstream ifs(this->parameterFileName.c_str());

		// Parameter file is "not" exists.
		if (ifs.fail()) 
		{
			std::cout << "Not exist : " << this->parameterFileName << std::endl;
			exit(-1);
		}

		// Parameter file is exists.
		std::cout << "Read " << this->parameterFileName << std::endl;
		boost::property_tree::ptree pt;
		boost::property_tree::read_ini(this->parameterFileName, pt);

		this->serviceName    = pt.get<std::string>(PARAMETER_FILE_KEY_GENERAL_SERVICE_NAME);
		this->deviceType     = pt.get<std::string>(PARAMETER_FILE_KEY_GENERAL_DEVICE_TYPE);
		this->deviceUniqueID = pt.get<std::string>(PARAMETER_FILE_KEY_GENERAL_DEVICE_UNIQUE_ID);

		this->neuronIpAddress = pt.get<std::string>(paramFileKeyPerceptionNeuronIpAddress);
		this->neuronPort      = pt.get<int>        (paramFileKeyPerceptionNeuronPort);

	
		std::cout << PARAMETER_FILE_KEY_GENERAL_SERVICE_NAME     << ":" << this->serviceName       << std::endl;
		std::cout << PARAMETER_FILE_KEY_GENERAL_DEVICE_TYPE      << ":" << this->deviceType        << std::endl;
		std::cout << PARAMETER_FILE_KEY_GENERAL_DEVICE_UNIQUE_ID << ":" << this->deviceUniqueID    << std::endl;
		std::cout << paramFileKeyPerceptionNeuronIpAddress       << ":" << this->neuronIpAddress   << std::endl;
		std::cout << paramFileKeyPerceptionNeuronPort            << ":" << this->neuronPort        << std::endl;
	}
	catch (std::string &ex)
	{
		std::cout << ex << std::endl;
		exit(EXIT_FAILURE);
	}
	catch (boost::exception &ex) 
	{
		std::cout << this->parameterFileName << " ERR :" << *boost::diagnostic_information_what(ex) << std::endl;
		exit(EXIT_FAILURE);
	}
}

