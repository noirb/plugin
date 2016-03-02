#include <sigverse/plugin/plugin/PerceptionNeuron/PerceptionNeuronDevice.h>
#include <sigverse/plugin/plugin/common/CheckRecvSIGServiceData.h>

//#include <opencv2/opencv.hpp>
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
//const std::string PerceptionNeuronDevice::paramFileKeyPerceptionNeuronSmoothingType         = "PerceptionNeuron.smoothing_type";
//const std::string PerceptionNeuronDevice::paramFileKeyPerceptionNeuronSmoothingSMANum       = "PerceptionNeuron.smoothing_sma_num";
//const std::string PerceptionNeuronDevice::paramFileKeyPerceptionNeuronSmoothingWMAWeight    = "PerceptionNeuron.smoothing_wma_weight";


///@brief Default Constructor
PerceptionNeuronDevice::PerceptionNeuronDevice()
{
}


///@brief Constructor
PerceptionNeuronDevice::PerceptionNeuronDevice(int argc, char **argv)
{
	if (argc == 1) 
	{
		// When it works without SIGVerse.
		fprintf(stdout, "Work stand alone.\n");
		this->sendMessageFlag = false;
		this->serverAddress = "";
		this->portNumber = -1;
	}
	else if (argc == 3) 
	{
		// Work with SIGVerse.
		fprintf(stdout, "SIGServer IP address: %s\n", argv[1]);
		fprintf(stdout, "Port number: %s\n", argv[2]);
		this->sendMessageFlag = true;
		this->serverAddress = argv[1];
		this->portNumber = atoi(argv[2]);
	}
}


PerceptionNeuronDevice::~PerceptionNeuronDevice()
{
}


//#ifdef DEBUG_PRINT
//void PerceptionNeuronDevice::debugPrint()
//{
//	// init output file stream
//	std::ofstream ofs("C:\\tmp\\plugin.log");
//
//	std::vector<TIME_INFO>::iterator it;
//
//	it = timeInfoList.begin();
//
//	//printf("%d/%d/%d %d:%d:%d'%d ElbowRight.w,%f\n", systime.wYear, systime.wMonth, systime.wDay, systime.wHour, systime.wMinute, systime.wSecond, systime.wMilliseconds, tmpOrientations[PerceptionNeuronSensorData::PerceptionNeuronJointType::ElbowRight].Orientation.w);
//
//	while( it != timeInfoList.end() ) 
//	{
//		ofs << (*it).systemTime.wYear << "/" 
//			<< (*it).systemTime.wMonth << "/"
//			<< (*it).systemTime.wDay  << " " 
//			<< (*it).systemTime.wHour << ":"
//			<< (*it).systemTime.wMinute << ":" 
//			<< (*it).systemTime.wSecond << "'"
//			<< (*it).systemTime.wMilliseconds
//			<< ", value," << std::setprecision(4) << (*it).val  << std::endl;
//		
//		it++;
//	}
//}
//#endif



///@brief Execute kinect v2 device.
int PerceptionNeuronDevice::run()
{
	try 
	{
		// Prepare to use SIGService.
		this->readIniFile();

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

		// if success, change the title of button
		if (sockTCPRef)
		{
			std::cout << "Connection is OK" << std::endl;
		}
		else
		{
			// if failed, show message
			std::cout << "Connection is NG" << std::endl;
		}

		BRRegisterFrameDataCallback(this, this->bvhFrameDataReceived);

		BRRegisterSocketStatusCallback(NULL, this->socketStatusChanged);
			
//		UpdateBvhDataShowUI();  //update the BoneID index when BVH data received

		delete neuronIpAddressChar;

		while (true)
		{
			// Quit
			if (GetAsyncKeyState('Q') || GetAsyncKeyState(VK_ESCAPE))
			{
				while (_kbhit() != 0){ _getch(); }
				break;
			}
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

//#ifdef DEBUG_PRINT
//	debugPrint();
//#endif

	return 0;
}


void PerceptionNeuronDevice::sendBvhData(void* customedObj, SOCKET_REF sender, BvhDataHeader* header, float* data)
{
	PerceptionNeuronDevice* pthis = (PerceptionNeuronDevice*)customedObj;

	//Execute smoothing 
	PerceptionNeuronSensorData sensorData;

	sensorData.bvhData.avatarIndex   = header->AvatarIndex;
	sensorData.bvhData.avatarName    = std::string((char*)header->AvatarName);
	sensorData.bvhData.withDisp      = header->WithDisp;
	sensorData.bvhData.withReference = header->WithReference;
	sensorData.bvhData.frameIndex    = header->FrameIndex;
	sensorData.bvhData.dataCount     = header->DataCount;
	sensorData.bvhData.data          = data;


	//std::cout << "header->DataCount=" << header->DataCount << ", AvatarIndex=" << header->AvatarIndex << ", AvatarName=" << header->AvatarName << ", FrameIndex=" << header->FrameIndex << std::endl;

	//// calculate data index for selected bone
	//int dataIndex = 0;
	////int curSel = m_wndComBoxBone.GetCurSel();   // Gets the currently selected option in the drop down box
	//int curSel = 0;

	//if (header->WithDisp)
	//{
	//	dataIndex = curSel * 6;

	//	if (header->WithReference)
	//	{
	//		dataIndex += 6;
	//	}

	//	float dispX = data[dataIndex + 0];
	//	float dispY = data[dataIndex + 1];
	//	float dispZ = data[dataIndex + 2];

	//	std::cout << "dispX=" << dispX << ", dispY=" << dispY << ", dispZ=" << dispZ << std::endl;

	//	float angX = data[dataIndex + 4];
	//	float angY = data[dataIndex + 3];
	//	float angZ = data[dataIndex + 5];

	//	std::cout << "angX=" << angX << ", angY=" << angY << ", angZ=" << angZ << std::endl;
	//}
	//else
	//{
	//	if (curSel == 0)
	//	{
	//		dataIndex = 0;
	//		if (header->WithReference)
	//		{
	//			dataIndex += 6;
	//		}

	//		// show hip's displacement
	//		float dispX = data[dataIndex + 0];
	//		float dispY = data[dataIndex + 1];
	//		float dispZ = data[dataIndex + 2];

	//		std::cout << "dispX=" << dispX << ", dispY=" << dispY << ", dispZ=" << dispZ << std::endl;

	//		// show hip's angle
	//		float angX = data[dataIndex + 4];
	//		float angY = data[dataIndex + 3];
	//		float angZ = data[dataIndex + 5];

	//		std::cout << "angX=" << angX << ", angY=" << angY << ", angZ=" << angZ << std::endl;
	//	}
	//	else
	//	{
	//		dataIndex = 3 + curSel * 3;
	//		if (header->WithReference)
	//		{
	//			dataIndex += 6;
	//		}

	//		// show angle
	//		float angX = dataIndex[data + 1];
	//		float angY = data[dataIndex + 0];
	//		float angZ = data[dataIndex + 2];

	//		std::cout << "angX=" << angX << ", angY=" << angY << ", angZ=" << angZ << std::endl;
	//	}
	//}

	// Send message to SigServer.
	std::string messageHeader = this->generateMessageHeader();
	std::string sensorDataMessage = sensorData.encodeSensorData();
	std::string message = messageHeader + sensorDataMessage;
	this->sendMessage(pthis->sigService, message);
}

void __stdcall PerceptionNeuronDevice::bvhFrameDataReceived(void* customedObj, SOCKET_REF sender, BvhDataHeader* header, float* data)
{
	this->sendBvhData(customedObj, sender, header, data);
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



/////@brief Set whole body joint positions.
//void PerceptionNeuronDevice::convertJointPositions2PerceptionNeuronJointPositions(Joint *joints, PerceptionNeuronSensorData::PerceptionNeuronJointPosition *PerceptionNeuronJointPositions)
//{
//	for (int i = 0; i < PerceptionNeuronSensorData::PerceptionNeuronJointType_Count; i++) 
//	{
//		PerceptionNeuronJointPositions[i].jointType     = getPerceptionNeuronJointType(joints[i].JointType);
//		PerceptionNeuronJointPositions[i].position.x    = joints[i].Position.X;
//		PerceptionNeuronJointPositions[i].position.y    = joints[i].Position.Y;
//		PerceptionNeuronJointPositions[i].position.z    = joints[i].Position.Z;
//		PerceptionNeuronJointPositions[i].trackingState = (PerceptionNeuronSensorData::PerceptionNeuronTrackingState)(joints[i].TrackingState);
//	}
//}
//
/////@brief Set whole body joint orientations.
//void PerceptionNeuronDevice::convertJointOrientations2PerceptionNeuronJointOrientations(JointOrientation *orientations, PerceptionNeuronSensorData::PerceptionNeuronJointOrientation *PerceptionNeuronJointOrientations) 
//{
//	for (int i = 0; i < PerceptionNeuronSensorData::PerceptionNeuronJointType_Count; i++) 
//	{
//		PerceptionNeuronJointOrientations[i].jointType     = getPerceptionNeuronJointType(orientations[i].JointType);
//		PerceptionNeuronJointOrientations[i].orientation.w = orientations[i].Orientation.w;
//		PerceptionNeuronJointOrientations[i].orientation.x = orientations[i].Orientation.x;
//		PerceptionNeuronJointOrientations[i].orientation.y = orientations[i].Orientation.y;
//		PerceptionNeuronJointOrientations[i].orientation.z = orientations[i].Orientation.z;
//	}
//}
//
/////@brief Convert JointType to PerceptionNeuronJointType.
//PerceptionNeuronSensorData::PerceptionNeuronJointType PerceptionNeuronDevice::getPerceptionNeuronJointType(const JointType jointType) 
//{
//	switch (jointType) 
//	{
//		case JointType_SpineBase:    { return PerceptionNeuronSensorData::SpineBase;     break; }
//		case JointType_SpineMid:     { return PerceptionNeuronSensorData::SpineMid;      break; }
//		case JointType_Neck:         { return PerceptionNeuronSensorData::Neck;          break; }
//		case JointType_Head:         { return PerceptionNeuronSensorData::Head;          break; }
//		case JointType_ShoulderLeft: { return PerceptionNeuronSensorData::ShoulderLeft;  break; }
//		case JointType_ElbowLeft:    { return PerceptionNeuronSensorData::ElbowLeft;     break; }
//		case JointType_WristLeft:    { return PerceptionNeuronSensorData::WristLeft;     break; }
//		case JointType_HandLeft:     { return PerceptionNeuronSensorData::HandLeft;      break; }
//		case JointType_ShoulderRight:{ return PerceptionNeuronSensorData::ShoulderRight; break; }
//		case JointType_ElbowRight:   { return PerceptionNeuronSensorData::ElbowRight;    break; }
//		case JointType_WristRight:   { return PerceptionNeuronSensorData::WristRight;    break; }
//		case JointType_HandRight:    { return PerceptionNeuronSensorData::HandRight;     break; }
//		case JointType_HipLeft:      { return PerceptionNeuronSensorData::HipLeft;       break; }
//		case JointType_KneeLeft:     { return PerceptionNeuronSensorData::KneeLeft;      break; }
//		case JointType_AnkleLeft:    { return PerceptionNeuronSensorData::AnkleLeft;     break; }
//		case JointType_FootLeft:     { return PerceptionNeuronSensorData::FootLeft;      break; }
//		case JointType_HipRight:     { return PerceptionNeuronSensorData::HipRight;      break; }
//		case JointType_KneeRight:    { return PerceptionNeuronSensorData::KneeRight;     break; }
//		case JointType_AnkleRight:   { return PerceptionNeuronSensorData::AnkleRight;    break; }
//		case JointType_FootRight:    { return PerceptionNeuronSensorData::FootRight;     break; }
//		case JointType_SpineShoulder:{ return PerceptionNeuronSensorData::SpineShoulder; break; }
//		case JointType_HandTipLeft:  { return PerceptionNeuronSensorData::HandTipLeft;   break; }
//		case JointType_ThumbLeft:    { return PerceptionNeuronSensorData::ThumbLeft;     break; }
//		case JointType_HandTipRight: { return PerceptionNeuronSensorData::HandTipRight;  break; }
//		case JointType_ThumbRight:   { return PerceptionNeuronSensorData::ThumbRight;    break; }
//		default: { throw std::string("This JointType is invalid. JointType enum number = "+jointType); break; }
//	}
//}

///@brief Read parameter file.
void PerceptionNeuronDevice::readIniFile()
{
	std::ifstream ifs(this->parameterFileName.c_str());

	//std::string sensorDataModeStr;
	//this->sendHandState;
	//std::string smoothingTypeStr;
	//std::string smoothingSMANumStr;
	//std::string smoothingWMAWeightStr;


	// Parameter file is "not" exists.
	if (ifs.fail()) 
	{
		std::cout << "Not exist : " << this->parameterFileName << std::endl;
		exit(-1);
	}
	// Parameter file is exists.
	try 
	{
		std::cout << "Read " << this->parameterFileName << std::endl;
		boost::property_tree::ptree pt;
		boost::property_tree::read_ini(this->parameterFileName, pt);

		this->serviceName    = pt.get<std::string>(PARAMETER_FILE_KEY_GENERAL_SERVICE_NAME);
		this->deviceType     = pt.get<std::string>(PARAMETER_FILE_KEY_GENERAL_DEVICE_TYPE);
		this->deviceUniqueID = pt.get<std::string>(PARAMETER_FILE_KEY_GENERAL_DEVICE_UNIQUE_ID);

		this->neuronIpAddress = pt.get<std::string>(paramFileKeyPerceptionNeuronIpAddress);
		this->neuronPort      = pt.get<int>        (paramFileKeyPerceptionNeuronPort);
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

	
	std::cout << PARAMETER_FILE_KEY_GENERAL_SERVICE_NAME     << ":" << this->serviceName       << std::endl;
	std::cout << PARAMETER_FILE_KEY_GENERAL_DEVICE_TYPE      << ":" << this->deviceType        << std::endl;
	std::cout << PARAMETER_FILE_KEY_GENERAL_DEVICE_UNIQUE_ID << ":" << this->deviceUniqueID    << std::endl;
	std::cout << paramFileKeyPerceptionNeuronIpAddress       << ":" << this->neuronIpAddress   << std::endl;
	std::cout << paramFileKeyPerceptionNeuronPort            << ":" << this->neuronPort        << std::endl;

	//PerceptionNeuronSensorData::setSensorDataMode(sensorDataModeStr);

	//this->setSmoothingInfo(smoothingTypeStr, smoothingSMANumStr, smoothingWMAWeightStr);
}



//
///*
// * @brief Only get latest sensor data (No Smoothing)
// */
//PerceptionNeuronSensorData PerceptionNeuronDevice::getLatestSensorData(const std::vector<PerceptionNeuronSensorData> &sensorDataList)
//{
//	PerceptionNeuronSensorData sensorData;
//
//	sensorData.rootPosition.x = sensorDataList[0].rootPosition.x;
//	sensorData.rootPosition.y = sensorDataList[0].rootPosition.y;
//	sensorData.rootPosition.z = sensorDataList[0].rootPosition.z;
//
//	for (int i = 0; i < JointType::JointType_Count; i++)
//	{
//		sensorData.jointPositions[i].jointType     = sensorDataList[0].jointPositions[i].jointType;
//		sensorData.jointPositions[i].position.x    = sensorDataList[0].jointPositions[i].position.x;
//		sensorData.jointPositions[i].position.y    = sensorDataList[0].jointPositions[i].position.y;
//		sensorData.jointPositions[i].position.z    = sensorDataList[0].jointPositions[i].position.z;
//		sensorData.jointPositions[i].trackingState = sensorDataList[0].jointPositions[i].trackingState;
//
//		sensorData.jointOrientations[i].jointType     = sensorDataList[0].jointOrientations[i].jointType;
//		sensorData.jointOrientations[i].orientation.w    = sensorDataList[0].jointOrientations[i].orientation.w;
//		sensorData.jointOrientations[i].orientation.x    = sensorDataList[0].jointOrientations[i].orientation.x;
//		sensorData.jointOrientations[i].orientation.y    = sensorDataList[0].jointOrientations[i].orientation.y;
//		sensorData.jointOrientations[i].orientation.z    = sensorDataList[0].jointOrientations[i].orientation.z;
//	}
//
//	return sensorData;
//}
//
//
