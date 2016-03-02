#include <iostream>
#include <sigverse/plugin/plugin/PerceptionNeuron/PerceptionNeuronDevice.h>

#include "NeuronDataReader.h"


int main(int argc, char **argv)
{
	try
	{
		PerceptionNeuronDevice PerceptionNeuronDevice(argc, argv);

		PerceptionNeuronDevice.run();

		return 0;
	}
	catch (std::exception ex)
	{
		std::cout << ex.what() << std::endl;
	}
}