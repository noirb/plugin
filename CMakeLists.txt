#
# SIGVerse Plugin
#

cmake_minimum_required(VERSION 2.8)

project(SIGVerse CXX)

set(SIGVERSE_PLUGIN_MAJOR_VERSION 1) 
set(SIGVERSE_PLUGIN_MINOR_VERSION 0)
set(SIGVERSE_PLUGIN_PATCH_VERSION 0)
set(SIGVERSE_PLUGIN_VERSION ${SIGVERSE_PLUGIN_MAJOR_VERSION}.${SIGVERSE_PLUGIN_MINOR_VERSION})
set(SIGVERSE_PLUGIN_FULL_VERSION ${SIGVERSE_PLUGIN_MAJOR_VERSION}.${SIGVERSE_PLUGIN_MINOR_VERSION}.${SIGVERSE_PLUGIN_PATCH_VERSION})


# Get SIGServer Install path
set(ENV_SIGVERSE_PATH $ENV{SIGVERSE_PATH})

if(NOT ENV_SIGVERSE_PATH)
    message(FATAL_ERROR  "Please install SIGServer, and add an environment variable 'SIGVERSE_PATH'.")
else()
    message(STATUS "SIGVERSE_PATH:" $ENV{SIGVERSE_PATH})
endif()
    

# Set Compile options
add_compile_options(-std=c++11 -D__cplusplus=201103L)
add_definitions(-DdDOUBLE -DNDEBUG -DUSE_ODE -DCONTROLLER)


# Set Include directories
include_directories("${PROJECT_SOURCE_DIR}/srcs" "${ENV_SIGVERSE_PATH}/include")

# Set Link directories
link_directories("${PROJECT_BINARY_DIR}/lib")
    
# Sources
file(GLOB_RECURSE plugin_srcs    "${PROJECT_SOURCE_DIR}/srcs/*.cpp")
file(GLOB_RECURSE plugin_headers "${PROJECT_SOURCE_DIR}/srcs/*.h")

file(GLOB_RECURSE exclude_files "${PROJECT_SOURCE_DIR}/srcs/sigverse/plugin/plugin/*")
    
list(REMOVE_ITEM plugin_srcs    ${exclude_files})
list(REMOVE_ITEM plugin_headers ${exclude_files})

# Libraries
add_library(sigplugin SHARED ${plugin_srcs} ${plugin_headers})
#add_library(sigplugin STATIC ${plugin_srcs} ${plugin_headers})

set_target_properties(sigplugin PROPERTIES 
    ARCHIVE_OUTPUT_DIRECTORY "${ENV_SIGVERSE_PATH}/lib" 
    LIBRARY_OUTPUT_DIRECTORY "${ENV_SIGVERSE_PATH}/lib" 
    RUNTIME_OUTPUT_DIRECTORY "${ENV_SIGVERSE_PATH}/bin" 
    LINKER_LANGUAGE CXX
)

# Install Headers
install(DIRECTORY "${PROJECT_SOURCE_DIR}/srcs/sigverse/plugin/common/"     DESTINATION "${ENV_SIGVERSE_PATH}/include/sigverse/plugin/common"     FILES_MATCHING PATTERN "*.h" )
install(DIRECTORY "${PROJECT_SOURCE_DIR}/srcs/sigverse/plugin/controller/" DESTINATION "${ENV_SIGVERSE_PATH}/include/sigverse/plugin/controller" FILES_MATCHING PATTERN "*.h" )


