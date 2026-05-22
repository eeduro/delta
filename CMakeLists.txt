cmake_minimum_required(VERSION 3.10)

project(delta-ntb)

set(CMAKE_CXX_STANDARD 20)

if(CMAKE_BUILD_TYPE MATCHES "Debug|DEBUG")
  set(LOGGER ON)
endif()

find_package(EEROS REQUIRED)


file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/*.cpp)
add_executable(delta-ntb ${SRC_FILES})
target_include_directories(delta-ntb PRIVATE ${PROJECT_SOURCE_DIR})
target_link_libraries(delta-ntb EEROS::eeros)

include(GNUInstallDirs)
install(TARGETS delta-ntb DESTINATION ${CMAKE_INSTALL_BINDIR})
install(DIRECTORY robotList/ DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/EEROS/HwConfigs/${PROJECT_NAME}/)
