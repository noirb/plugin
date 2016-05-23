#include <iostream>
#include <fstream>

#include <sigverse/plugin/plugin/common/Device.h>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/exception/diagnostic_information.hpp>

/**
 * @brief Initialize SIGService.
 */
void Device::initializeSigService(sigverse::SIGService &sigService)
{
	if (this->sendMessageFlag) 
	{
		// Initialize sigservice.
		try 
		{
			if (sigService.connect(this->serverAddress, this->portNumber)) 
			{
				std::cout << "Success to connect to SIGServer(IP: " << this->serverAddress << ", port: " << this->portNumber << " )." << std::endl;
				
				//The following commands are for automatically shutdown at QuitSimulation.
				if (sigService.connectToViewer()) 
				{
					std::cout << "Success to connect to SIGViewer." << std::endl;
					sigService.setAutoExitProc(true);
					sigService.setAutoExitLoop(true);
				}
			}
		}
		catch (std::exception &ex) 
		{
			std::cout << "Initialize sigservice ERR :" << ex.what() << std::endl;
		}
	}
}


/**
 * @brief send message to Controller
 */
const void Device::sendMessage(sigverse::SIGService &sigService, const std::string &message)
{
	try 
	{
		std::vector<std::string> connectedEntitiesName = sigService.getAllConnectedEntitiesName();
			
		for (int i = 0; i < (int)connectedEntitiesName.size(); i++) 
		{
			sigService.sendMsgToCtr(connectedEntitiesName[i].c_str(), message);
		}
	}
	catch (...) 
	{
		std::cout << "catch (...) in Device::sendMessage" << std::endl;
		Sleep(20000);
	}
}


///@brief Read parameter file.
void Device::readIniFile()
{
	this->readIniFile(this->parameterFileName);
}


///@brief Read parameter file.
void Device::readIniFile(const std::string &parameterFileName)
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

		std::cout << PARAMETER_FILE_KEY_GENERAL_SERVICE_NAME     << ":" << this->serviceName        << std::endl;
		std::cout << PARAMETER_FILE_KEY_GENERAL_DEVICE_TYPE      << ":" << this->deviceType         << std::endl;
		std::cout << PARAMETER_FILE_KEY_GENERAL_DEVICE_UNIQUE_ID << ":" << this->deviceUniqueID     << std::endl;
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

