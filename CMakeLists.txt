cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0015 NEW)
project(delta)

find_package(EEROS 1.0.0.30 REQUIRED)
include_directories(${EEROS_INCLUDE_DIR};${EEROS_LIBUCL_INCLUDE_DIR})
link_directories(${EEROS_LIB_DIR};${EEROS_LIBUCL_LINK_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

link_directories("../robotics_cape_installer/libraries/")

file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/*.cpp)

add_executable(delta ${SRC_FILES})

target_link_libraries(delta eeros ${CMAKE_DL_LIBS} ucl roboticscape)
