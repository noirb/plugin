#ifndef SIGVERSE__OPTITRACK_DEVICE_H
#define SIGVERSE__OPTITRACK_DEVICE_H

#include <sigverse/plugin/plugin/common/Device.h>

#include <conio.h>
#include <winsock2.h>
#include <NatNetTypes.h>
#include <NatNetClient.h>

// Default buffer size for IP adrress.
#define BUFFER_SIZE_FOR_IP_ADDRESS 128

// Callback methods.

///@brief receives NatNet error mesages
void __cdecl MessageHandler(int msgType, char* msg);

///@brief receives data from the server.
void __cdecl DataHandler(sFrameOfMocapData* data, void* pUserData);


class OptiTrackDevice : public Device
{
private:
	///@brief NatNet connection type
	int natNetConnectionType;

	char szMyIPAddress    [BUFFER_SIZE_FOR_IP_ADDRESS];
	char szServerIPAddress[BUFFER_SIZE_FOR_IP_ADDRESS];

public:
	OptiTrackDevice(int argc, char **argv);

	///@brief establish NatNet Client connection.
	int createNatNetClient();

	///@brief reset NatNet Client connection.
	void resetNatNetClient();

	///@brief run OptiTrack device.
	int run();

	///@brief Send message to SIGServer.
	///@param message Message you want to send to SIGServer.
	void sendMessage(const std::string &message);

	///@brief NatNet client instance
//	NatNetClient *natNetClient;

	///@brief SIGService instance 
	sigverse::SIGService *sigService;
};

#endif // SIGVERSE__OPTITRACK_DEVICE_H
