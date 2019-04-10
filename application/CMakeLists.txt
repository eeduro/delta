cmake_minimum_required(VERSION 2.8)
project(myApp)

find_package(EEROS 1.0.0.25 REQUIRED)
include_directories(${EEROS_INCLUDE_DIR};${EEROS_LIBUCL_INCLUDE_DIR})
link_directories(${EEROS_LIB_DIR};${EEROS_LIBUCL_LINK_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
# find_library(BEAGLE_BOARD_BLUE_LIBRARY roboticscape)
link_directories("~/workspace/VTDelta/BeagleBoneBlue/robotics_cape_installer/libraries/")

#add_executable(myApp smc4Motor/motorController.cpp smc4Motor/MotorControlSystem.cpp smc4Motor/MotorSafetyProperties.cpp )
#add_executable(myApp smc/SimpleMotorController.cpp smc/MyControlSystem.cpp smc/MySafetyProperties.cpp )
add_executable(myApp delta.cpp safety/DeltaSafetyProperties.cpp control/DeltaControlSystem.cpp control/PathPlanner.cpp control/DirectKinematic.cpp control/Inertia.cpp control/Jacobi.cpp control/Jacobian.cpp control/Kinematic.cpp control/MotorModel.cpp sequence/CalibrateBlockSequence.cpp sequence/MoveBlockSequence.cpp sequence/SortSequence.cpp sequence/ShuffleSequence.cpp sequence/MouseSequence.cpp sequence/AutoMoveSequence.cpp Calibration.cpp sequence/HomingSequence.cpp sequence/ParkSequence.cpp sequence/DetectSequence.cpp sequence/MainSequence.cpp)
#add_executable(myApp mouseTest/MouseTest.cpp)
#add_executable(myApp test/buttonTest.cpp)
#add_executable(myApp test/rtTest1.cpp)

target_link_libraries(myApp eeros ${CMAKE_DL_LIBS} ucl roboticscape)