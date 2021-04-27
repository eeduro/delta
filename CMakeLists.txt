cmake_minimum_required(VERSION 3.5.1)

project(delta-ntb)

set(CMAKE_CXX_STANDARD 14)


set(RECOMMENDED_EEROS_VERSION 1.1.0.0)
set(RECOMMENDED_BBBLUE_EEROS_VERSION 1.2.0.0)

if(CMAKE_BUILD_TYPE MATCHES "Debug|DEBUG")
	set(LOGGER ON)
endif()


include(cmake/versionCheckHelpers.cmake)

# Find EEROS
setVersionToFind("EEROS" "REQUIRED_EEROS_VERSION" ${RECOMMENDED_EEROS_VERSION} ${REQUIRED_EEROS_VERSION})
find_package(EEROS ${VERSION_TO_FIND} EXACT)
checkIfModuleFound("EEROS" "REQUIRED_EEROS_VERSION")

# Find BBBlueEEROS
if(USE_BBBLUE)
  setVersionToFind("bbblue-eeros" "REQUIRED_BBBLUE_EEROS_VERSION" ${RECOMMENDED_BBBLUE_EEROS_VERSION} ${REQUIRED_BBBLUE_EEROS_VERSION})
  find_package(bbblue-eeros ${VERSION_TO_FIND} EXACT)
  checkIfModuleFound("bbblue-eeros" "REQUIRED_BBBLUE_EEROS_VERSION")
endif(USE_BBBLUE)


include_directories(${EEROS_INCLUDE_DIR};${EEROS_LIBUCL_INCLUDE_DIR})
link_directories(${EEROS_LIB_DIR};${EEROS_LIBUCL_LINK_DIR})

file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/*.cpp)
add_executable(delta-ntb ${SRC_FILES})
target_link_libraries(delta-ntb eeros ucl ${CMAKE_DL_LIBS})




