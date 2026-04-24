cmake_minimum_required(VERSION 3.10)

project(delta-ntb)

set(CMAKE_CXX_STANDARD 20)

set(REQUIRED_EEROS_VERSION 1.4.0.0)
set(REQUIRED_BBBLUE_EEROS_VERSION 1.3.0.0)

if(CMAKE_BUILD_TYPE MATCHES "Debug|DEBUG")
	set(LOGGER ON)
endif()

include(cmake/versionCheckHelpers.cmake)

# Find EEROS
find_package(EEROS ${REQUIRED_EEROS_VERSION} REQUIRED)
message(STATUS "Found EEROS version: ${EEROS_VERSION}")

# Find BBBlueEEROS, will be later used for the HAL
find_package(bbblue-eeros ${REQUIRED_BBBLUE_EEROS_VERSION} REQUIRED)
message(STATUS "Found bbblue-eeros version: ${bbblue-eeros_VERSION}")

file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/*.cpp)
add_executable(delta-ntb ${SRC_FILES})
target_link_libraries(delta-ntb PRIVATE EEROS::eeros)

