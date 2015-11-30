# target file (.so)
TARGET = ./ManNiiAvatarControllerByKinectV2.so

# sources
SRCS =\
	../../sigverse/plugin/common/SigCmn.cpp \
	../../sigverse/plugin/common/Quaternion.cpp \
	../../sigverse/plugin/common/sensor/SensorData.cpp \
	../../sigverse/plugin/common/sensor/KinectV2SensorData.cpp \
	../../sigverse/plugin/controller/common/Posture.cpp \
	../../sigverse/plugin/controller/common/ManNiiPosture.cpp \
	../../sigverse/plugin/controller/common/AvatarController.cpp \
	../../sigverse/plugin/controller/common/ManNiiAvatarController.cpp \
	../../sigverse/plugin/controller/common/sensor/KinectV2DeviceManager.cpp \
	ManNiiAvatarControllerByKinectV2.cpp \

# sigverse path
SIG_SRC = $(SIGVERSE_PATH)/include

# compiler
CXX = g++

# compile option
CFLAGS = -std=c++11 -DCONTROLLER -DNDEBUG -DUSE_ODE -DdDOUBLE -I$(SIG_SRC) -I../../ -fPIC -shared

# generate intermediate object file from sources.
OBJS := $(SRCS:.cpp=.o)

# -- compile rule --

all: $(TARGET)

# intermediate output file
%.o: %.cpp
	$(CXX) $(CFLAGS) -o $@ -c $<

# target file(.so)
$(TARGET): $(OBJS) $(SRCS)
	$(CXX) $(CFLAGS) -o $@ $(OBJS) # Link object files.
	rm -f $(OBJS) # Remove intermediate files.

# remove object file
clean:
	rm -f $(TARGET) $(OBJS)
