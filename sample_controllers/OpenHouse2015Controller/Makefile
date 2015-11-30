# target file (.so)
TARGET1 = ./OpenHouse2015Controller.so
TARGET2 = ./RHIBallController.so

# sources
SRCS1 =\
	../../sigverse/plugin/common/SigCmn.cpp \
	../../sigverse/plugin/common/Quaternion.cpp \
	../../sigverse/plugin/common/sensor/SensorData.cpp \
	../../sigverse/plugin/common/sensor/KinectV2SensorData.cpp \
	../../sigverse/plugin/common/sensor/OculusRiftDK1SensorData.cpp \
	../../sigverse/plugin/controller/common/Posture.cpp \
	../../sigverse/plugin/controller/common/ManNiiPosture.cpp \
	../../sigverse/plugin/controller/common/AvatarController.cpp \
	../../sigverse/plugin/controller/common/ManNiiAvatarController.cpp \
	../../sigverse/plugin/controller/common/sensor/KinectV2DeviceManager.cpp \
	../../sigverse/plugin/controller/common/sensor/OculusDK1DeviceManager.cpp \
	OpenHouse2015Controller.cpp \

SRCS2 =\
	$(SIGVERSE_BUILD_PATH)/sigverse/common/SigCmn.cpp \
	RHIBallController.cpp \
	
	
# sigverse path
SIG_SRC = $(SIGVERSE_PATH)/include

# compiler
CXX = g++

# compile option
CFLAGS = -std=c++11 -DCONTROLLER -DNDEBUG -DUSE_ODE -DdDOUBLE -I$(SIG_SRC) -I../../ -fPIC -shared

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
	