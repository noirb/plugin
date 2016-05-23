#include <sigverse/plugin/plugin/PerceptionNeuron/PerceptionNeuronDevice.h>
#include <sigverse/plugin/common/sensor/PerceptionNeuronSensorData.h>
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
const std::string Device::parameterFileName = "PerceptionNeuron.ini";

//Parameter file information
const std::string PerceptionNeuronDevice::paramFileKeyPerceptionNeuronDataType        = "PerceptionNeuron.data_type";
const std::string PerceptionNeuronDevice::paramFileKeyPerceptionNeuronBvhIpAddress    = "PerceptionNeuron.bvh_ip_address";
const std::string PerceptionNeuronDevice::paramFileKeyPerceptionNeuronBvhPort         = "PerceptionNeuron.bvh_port";
const std::string PerceptionNeuronDevice::paramFileKeyPerceptionNeuronCalcIpAddress   = "PerceptionNeuron.calc_ip_address";
const std::string PerceptionNeuronDevice::paramFileKeyPerceptionNeuronCalcPort        = "PerceptionNeuron.calc_port";


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
		SOCKET_REF sockRefBvh  = NULL;
		SOCKET_REF sockRefCalc = NULL;

		if (this->dataType == STR(BVH))
		{
			this->connect4Bvh(sockRefBvh);

			// Receive BVH data by the callback function.
			BRRegisterFrameDataCallback(this, PerceptionNeuronDevice::bvhFrameDataReceived);
		}
		else if (this->dataType == STR(CALC))
		{
			this->connect4Calc(sockRefCalc);

			// Receive Calculation data by the callback function.
			BRRegisterFrameDataCallback(this, PerceptionNeuronDevice::bvhFrameDataReceived); // This line is necessary because the API(ver b15) have a bug 
			BRRegisterCalculationDataCallback(this, PerceptionNeuronDevice::calcFrameDataReceived);
		}

		// Receive Socket status by the callback function.
		BRRegisterSocketStatusCallback(this, this->socketStatusChanged);

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
		if (this->dataType == STR(BVH))
		{
			BRCloseSocket(sockRefBvh);
		}
		else if (this->dataType == STR(CALC))
		{
			BRCloseSocket(sockRefCalc);
		}

		sigService.disconnect();
	}
	catch (std::exception &ex) 
	{
		std::cout << "run ERR :" << ex.what() << std::endl;
		throw ex;
	}

	return 0;
}


void PerceptionNeuronDevice::connect4Bvh(SOCKET_REF sockRef)
{
	int ipAddressLen = this->neuronBvhIpAddress.length();
	char* ipAddressChar = new char[ipAddressLen+1];
	memcpy(ipAddressChar, this->neuronBvhIpAddress.c_str(), ipAddressLen+1);

	sockRef = BRConnectTo(ipAddressChar, this->neuronBvhPort);

	if (sockRef)
	{
		std::cout << "Connection for BVH is OK" << std::endl;
	}
	else
	{
		std::cout << "Connection for BVH is NG" << std::endl;
		Sleep(15*1000);
		exit(EXIT_FAILURE);
	}

	delete ipAddressChar;
}

void PerceptionNeuronDevice::connect4Calc(SOCKET_REF sockRef)
{
	int ipAddressLen = this->neuronCalcIpAddress.length();
	char* ipAddressChar = new char[ipAddressLen+1];
	memcpy(ipAddressChar, this->neuronCalcIpAddress.c_str(), ipAddressLen+1);

	sockRef = BRConnectTo(ipAddressChar, this->neuronCalcPort);

	if (sockRef)
	{
		std::cout << "Connection for Calculation is OK" << std::endl;
	}
	else
	{
		std::cout << "Connection for Calculation is NG" << std::endl;
		Sleep(15*1000);
		exit(EXIT_FAILURE);
	}

	delete ipAddressChar;
}


void PerceptionNeuronDevice::sendBvhData(void* customedObj, SOCKET_REF sender, BvhDataHeader* header, float* data)
{
	PerceptionNeuronDevice* pthis = (PerceptionNeuronDevice*)customedObj;

	PerceptionNeuronSensorData sensorData;

	sensorData.setDataType(PerceptionNeuronSensorData::DataTypeEnum::BVH);

	sensorData.bvhData.avatarIndex   = header->AvatarIndex;
	sensorData.bvhData.avatarName    = std::string((char*)header->AvatarName);
	sensorData.bvhData.withDisp      = (header->WithDisp==1);
	sensorData.bvhData.withReference = (header->WithReference==1);
	sensorData.bvhData.frameIndex    = header->FrameIndex;
	sensorData.bvhData.dataCount     = header->DataCount;
	sensorData.bvhData.data          = data;

	// Send message to SigServer.
	std::string messageHeader = pthis->generateMessageHeader();
	std::string sensorDataMessage = sensorData.encodeSensorData();
	std::string message = messageHeader + sensorDataMessage;
	pthis->sendMessage(pthis->sigService, message);
}

void PerceptionNeuronDevice::sendCalcData(void* customedObj, SOCKET_REF sender, CalcDataHeader* header, float* data)
{
	PerceptionNeuronDevice* pthis = (PerceptionNeuronDevice*)customedObj;

	PerceptionNeuronSensorData sensorData;

	sensorData.setDataType(PerceptionNeuronSensorData::DataTypeEnum::CALC);

	sensorData.calcData.avatarIndex   = header->AvatarIndex;
	sensorData.calcData.avatarName    = std::string((char*)header->AvatarName);
	sensorData.calcData.frameIndex    = header->FrameIndex;
	sensorData.calcData.dataCount     = header->DataCount;
	sensorData.calcData.data          = data;

	// Send message to SigServer.
	std::string messageHeader = pthis->generateMessageHeader();
	std::string sensorDataMessage = sensorData.encodeSensorData();
	std::string message = messageHeader + sensorDataMessage;
	pthis->sendMessage(pthis->sigService, message);
}


void __stdcall PerceptionNeuronDevice::bvhFrameDataReceived(void* customedObj, SOCKET_REF sender, BvhDataHeader* header, float* data)
{
	PerceptionNeuronDevice* pthis = (PerceptionNeuronDevice*)customedObj;

	pthis->sendBvhData(customedObj, sender, header, data);
}

void __stdcall PerceptionNeuronDevice::calcFrameDataReceived(void* customedObj, SOCKET_REF sender, CalcDataHeader* header, float* data)
{
	PerceptionNeuronDevice* pthis = (PerceptionNeuronDevice*)customedObj;

	pthis->sendCalcData(customedObj, sender, header, data);
}


void __stdcall PerceptionNeuronDevice::socketStatusChanged(void* customedObj, SOCKET_REF sender, SocketStatus status, char* message)
{
	switch (status)
	{
		case CS_Running:
		{
			std::cout << "Socket is working correctly. msg=" << message << std::endl;
			break;
		}
		case CS_Starting:
		{
			std::cout << "Is trying to start service. msg=" << message << std::endl;
			break;
		}
		case CS_OffWork:
		{
			std::cout << "Not working. msg=" << message << std::endl;
			break;
		}
	}
}


///@brief Read parameter file.
void PerceptionNeuronDevice::readIniFile()
{
	this->readIniFile(this->parameterFileName);
}


///@brief Read parameter file.
void PerceptionNeuronDevice::readIniFile(const std::string &parameterFileName)
{
	try 
	{
		std::ifstream ifs(parameterFileName.c_str());

		// Parameter file is "not" exists.
		if (ifs.fail()) 
		{
			std::cout << "Not exist : " << parameterFileName << std::endl;
			exit(-1);
		}

		// Parameter file is exists.
		std::cout << "Read " << parameterFileName << std::endl;
		boost::property_tree::ptree pt;
		boost::property_tree::read_ini(parameterFileName, pt);

		this->serviceName    = pt.get<std::string>(PARAMETER_FILE_KEY_GENERAL_SERVICE_NAME);
		this->deviceType     = pt.get<std::string>(PARAMETER_FILE_KEY_GENERAL_DEVICE_TYPE);
		this->deviceUniqueID = pt.get<std::string>(PARAMETER_FILE_KEY_GENERAL_DEVICE_UNIQUE_ID);

		this->dataType            = pt.get<std::string>(paramFileKeyPerceptionNeuronDataType);
		this->neuronBvhIpAddress  = pt.get<std::string>(paramFileKeyPerceptionNeuronBvhIpAddress);
		this->neuronBvhPort       = pt.get<int>        (paramFileKeyPerceptionNeuronBvhPort);
		this->neuronCalcIpAddress = pt.get<std::string>(paramFileKeyPerceptionNeuronCalcIpAddress);
		this->neuronCalcPort      = pt.get<int>        (paramFileKeyPerceptionNeuronCalcPort);
	
		std::cout << PARAMETER_FILE_KEY_GENERAL_SERVICE_NAME     << ":" << this->serviceName        << std::endl;
		std::cout << PARAMETER_FILE_KEY_GENERAL_DEVICE_TYPE      << ":" << this->deviceType         << std::endl;
		std::cout << PARAMETER_FILE_KEY_GENERAL_DEVICE_UNIQUE_ID << ":" << this->deviceUniqueID     << std::endl;

		std::cout << paramFileKeyPerceptionNeuronDataType        << ":" << this->dataType            << std::endl;
		std::cout << paramFileKeyPerceptionNeuronBvhIpAddress    << ":" << this->neuronBvhIpAddress  << std::endl;
		std::cout << paramFileKeyPerceptionNeuronBvhPort         << ":" << this->neuronBvhPort       << std::endl;
		std::cout << paramFileKeyPerceptionNeuronCalcIpAddress   << ":" << this->neuronCalcIpAddress << std::endl;
		std::cout << paramFileKeyPerceptionNeuronCalcPort        << ":" << this->neuronCalcPort      << std::endl;

		if (this->dataType != STR(BVH) && this->dataType != STR(CALC))
		{
			throw std::string("Illegal data type : "+this->dataType);
		}
	}
	catch (std::string &ex)
	{
		std::cout << ex << std::endl;
		exit(EXIT_FAILURE);
	}
	catch (boost::exception &ex) 
	{
		std::cout << parameterFileName << " ERR :" << *boost::diagnostic_information_what(ex) << std::endl;
		exit(EXIT_FAILURE);
	}
}

