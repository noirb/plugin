# Setup:
-----------

1. Download the NatNet SDK: http://optitrack.com/downloads/developer-tools.html#natnet-sdk
2. Set the environment variable `NATNET_SD_ROOT` to the directory you extracted the SDK to
3. If NOT using Visual Studio 2013, add `legacy_stdio_definitions.lib` to the list of libraries to link against. This is necessary due to the latest version of the NatNet SDK only officially supporting up to VS 2013.