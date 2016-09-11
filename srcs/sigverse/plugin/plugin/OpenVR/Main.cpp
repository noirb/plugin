#include <iostream>
#include <sigverse/plugin/plugin/OpenVR/OpenVRDevice.h>

int main(int argc, char* argv[])
{
    try {
        OpenVRDevice openVRDevice(argc, argv);

        openVRDevice.run();
    }
    catch (...) {
        std::cout << "catch (...)" << std::endl;
    }

    return 0;
}