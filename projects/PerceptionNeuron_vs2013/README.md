# Setup
-----------------
1. Setup Boost and [SigService](https://github.com/noirb/SIGService)
2. Download [NeuronDataReader SDK b15](https://neuronmocap.com/content/neurondatareader-sdk-b15)
3. Set the following environment variables:

    SIGBUILD_NEURONDATAREADER_INC : <NeuronDataReader SDK>\Windows\include
    SIGBUILD_NEURONDATAREADER_LIB : <NeuronDataReader SDK>\Windows\lib\x86
    
    
If you used the global sigverse setup script, this should already be done for you.

NOTE: If you are NOT using Visual Studio 2013 and you have build issues, you may need to manually update the project to match your VS version. Right-click the project in the Solution Explorer and select "Upgrade VC++ Compiler and Libraries".

# How to Use
-----------------

Before performing the plugin, please launch Axis Neuron and connect Perception Neuron.

Please set the following Axis Neuron settings.
* [Settings]-[Broadcasting] : select "TCP", and check "BVH", and set port number.
* [Settings]-[Output Format]-[BVH Data] : select Block type "Binary", and select Rotation "YXZ", and uncheck "Displacement".
