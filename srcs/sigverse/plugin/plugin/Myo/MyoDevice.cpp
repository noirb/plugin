#include <sigverse/plugin/plugin/Myo/DataCollector.h>
#include <sigverse/plugin/plugin/Myo/MyoDevice.h>
#include <sigverse/plugin/common/sensor/MyoSensorData.h>
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
const std::string Device::parameterFileName = "Myo.ini";

///@brief Default Constructor
MyoDevice::MyoDevice()
{
}

///@brief Constructor
MyoDevice::MyoDevice(int argc, char **argv)
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


MyoDevice::~MyoDevice()
{
}


///@brief Execute a device.
int MyoDevice::run()
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

		// First, we create a Hub with our application identifier. Be sure not to use the com.example namespace when
		// publishing your application. The Hub provides access to one or more Myos.
		myo::Hub hub("org.sigverse.myoplugin");

		std::cout << "Attempting to find a Myo..." << std::endl;

		// Next, we attempt to find a Myo to use. If a Myo is already paired in Myo Connect, this will return that Myo
		// immediately.
		// waitForMyo() takes a timeout value in milliseconds. In this case we will try to find a Myo for 10 seconds, and
		// if that fails, the function will return a null pointer.
		myo::Myo* myo = hub.waitForMyo(10000);

		// If waitForMyo() returned a null pointer, we failed to find a Myo, so exit with an error message.
		if (!myo) 
		{
			throw std::runtime_error("Unable to find a Myo!");
		}

		// We've found a Myo.
		std::cout << "Connected to a Myo armband!" << std::endl << std::endl;

		// Next we enable EMG streaming on the found Myo.
		myo->setStreamEmg(myo::Myo::streamEmgEnabled);

		// Next we construct an instance of our DeviceListener, so that we can register it with the Hub.
		DataCollector collector;

		// Hub::addListener() takes the address of any object whose class inherits from DeviceListener, and will cause
		// Hub::run() to send events to all registered device listeners.
		hub.addListener(&collector);

		// Finally we enter our main loop.
		while (true) 
		{
			// In each iteration of our main loop, we run the Myo event loop for a set number of milliseconds.
			// In this case, we wish to update our display 20 times a second, so we run for 1000/20 milliseconds.
			hub.run(1000/20);

			// After processing events, we call the print() member function we defined above to print out the values we've
			// obtained from any events that have occurred.
			MyoSensorData sensorData = collector.getSensorData();

			// Send message to SigServer.
			std::string messageHeader = this->generateMessageHeader();
			std::string sensorDataMessage = sensorData.encodeSensorData();
			std::string message = messageHeader + sensorDataMessage;
			this->sendMessage(this->sigService, message);

//			std::cout << message << std::endl;
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

