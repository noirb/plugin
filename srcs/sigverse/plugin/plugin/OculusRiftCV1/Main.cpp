#include <iostream>
#include <sigverse/plugin/plugin/OculusRiftCV1/OculusRiftCV1Device.h>

int main(int argc, char* argv[])
{
	try {
		OculusRiftCV1Device oculusRiftCV1Device(argc, argv);

		oculusRiftCV1Device.run();
	}
	catch (...) {
		std::cout << "catch (...)" << std::endl;
	}

	return 0;
}