# target file (.so)
TARGET1 = ./OpenHouse2015Controller.so
TARGET2 = ./RHIBallController.so

# sources
SRCS1 =\
	$(SIGVERSE_BUILD_PATH)/sigverse/devicecommon/SigCmn.cpp \
	$(SIGVERSE_BUILD_PATH)/sigverse/devicecommon/Quaternion.cpp \
	$(SIGVERSE_BUILD_PATH)/sigverse/devicecommon/device/SensorData.cpp \
	$(SIGVERSE_BUILD_PATH)/sigverse/devicecommon/device/KinectV2SensorData.cpp \
	$(SIGVERSE_BUILD_PATH)/sigverse/devicecommon/device/OculusRiftDK1SensorData.cpp \
	$(SIGVERSE_BUILD_PATH)/sigverse/controller/common/Posture.cpp \
	$(SIGVERSE_BUILD_PATH)/sigverse/controller/common/ManNiiPosture.cpp \
	$(SIGVERSE_BUILD_PATH)/sigverse/controller/common/AvatarController.cpp \
	$(SIGVERSE_BUILD_PATH)/sigverse/controller/common/ManNiiAvatarController.cpp \
	$(SIGVERSE_BUILD_PATH)/sigverse/controller/common/device/KinectV2DeviceManager.cpp \
	$(SIGVERSE_BUILD_PATH)/sigverse/controller/common/device/OculusDK1DeviceManager.cpp \
	OpenHouse2015Controller.cpp \

SRCS2 =\
	$(SIGVERSE_BUILD_PATH)/sigverse/devicecommon/SigCmn.cpp \
	RHIBallController.cpp \
	
	
# sigverse path
SIG_SRC = $(SIGVERSE_PATH)/include

# compiler
CXX = g++

# compile option
CFLAGS = -std=c++11 -DCONTROLLER -DNDEBUG -DUSE_ODE -DdDOUBLE -I$(SIG_SRC) -I$(SIGVERSE_BUILD_PATH) -fPIC -shared

# generate intermediate object file from sources.
OBJS1 := $(SRCS1:.cpp=.o)
OBJS2 := $(SRCS2:.cpp=.o)

# -- compile rule --

all: $(TARGET1) $(TARGET2) 

# intermediate output file
%.o: %.cpp
	$(CXX) $(CFLAGS) -o $@ -c $<

# target file(.so)
$(TARGET1): $(OBJS1) $(SRCS1)
	$(CXX) $(CFLAGS) -o $@ $(OBJS1)
#	rm -f $(OBJS1) # Remove intermediate files.

$(TARGET2): $(OBJS2) $(SRCS2)
	$(CXX) $(CFLAGS) -o $@ $(OBJS2)
#	rm -f $(OBJS2) # Remove intermediate files.

# remove object file
clean:
	rm -f $(TARGET1) $(OBJS1)
	rm -f $(TARGET2) $(OBJS2)
	