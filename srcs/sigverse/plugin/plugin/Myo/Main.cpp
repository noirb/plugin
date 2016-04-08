#include <iostream>
#include <sigverse/plugin/plugin/Myo/MyoDevice.h>


int main(int argc, char **argv)
{
	try
	{
		MyoDevice MyoDevice(argc, argv);

		MyoDevice.run();

		return 0;
	}
	catch (std::exception ex)
	{
		std::cout << ex.what() << std::endl;
	}
}