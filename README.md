How to use
-----------------
This repository provides general plugins for getting sensor data from various devices.

Please perform the following steps.

1. Create SIGVerse plugin(.sig) for getting sensor data.
2. Create common library(sigplugin.so) for connecting to plugin.
3. Use the created common library(sigplugin.so) in your controllers, and get sensor data.


Create SIGVerse plugin(.sig) for getting sensor data.
-----------------
Please open the solution file(.sln) by VisualStudio and build.
Solution files are in under the projects/ directory.

Notes are the following.
* Have to prepare SIGService.lib.
* Some projects need OpenCV, boost or various SDK.
* Please check [Additional Include directories], [Additional Library Directories], [Additional Dependencies], [Post-build Event], and so on.


Create common library(sigplugin.so) for connecting to plugin.
-----------------
Please perform the following steps.
Created library(sigplugin.so) and headers will copy into an install directory of SIGServer.

0. Please install SIGServer and set environment variables SIGVERSE_PATH.
1. mkdir build
2. cd build
3. cmake ..
4. make
5. make install


Use the created common library(sigplugin.so) in your controllers, and get sensor data.
-----------------
You can use sigplugin.so in your controllers.
Please add linker flags "-L$(SIGVERSE_PATH)/lib -lsigplugin" into your Makefile.
Sample controllers are in sample_controllers/ directory.


