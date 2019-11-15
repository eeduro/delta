#
# versionCheckHelpers
#
# This module provides functions that simplify
# versioning of libraries and dependency management.
#
# Dependencies:
# * MunkeiVersionFromGit.cmake
#

cmake_minimum_required(VERSION 3.5.1)


#
# Simple logging function used in this module.
# Logging outputs are executed when LOGGER variable evaluates to true.
# Usage: set(LOGGER ON) turns logging on.
#
function(log string)
  if(LOGGER)
    message(STATUS "[versionCheckHelpers]: ${string}")
  endif()
endfunction(log string)



#
# Fetches the version information from git and sets the following global variables:
# LIBRARY_VERSION
#
# Usage: setLibraryVersion()
#
function(setLibraryVersion)
  include(cmake/MunkeiVersionFromGit.cmake)
  version_from_git()
  set(LIBRARY_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}.${VERSION_TWEAK}" PARENT_SCOPE)
endfunction(setLibraryVersion)



#
# Creates the package config and package version files.
#
# The <projectName>Config.cmake.in is expected to be found in the cmake directory
# inside of the project root directory. The inFileName parameter provides its name.
#
# Parameter:
# inFileName        - the name of the <projectName>Config.cmake.in file
# includeInstallDir - the path of the include directory, relative to the root install dir
# libInstallDir     - the path of the lib directory, relative to the root install dir
#
#
# Usage: createPackageFiles("MyProjectConfig.cmake.in" "/include" "/lib")
#
function(createPackageFiles inFileName includeInstallDir libInstallDir)
  set(INCLUDE_INSTALL_DIR ${includeInstallDir})
  set(LIB_INSTALL_DIR ${libInstallDir})

  include(CMakePackageConfigHelpers)

  configure_package_config_file("cmake/${inFileName}"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION "${LIB_INSTALL_DIR}/${PROJECT_NAME}/cmake"
    PATH_VARS INCLUDE_INSTALL_DIR LIB_INSTALL_DIR)

  write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${LIBRARY_VERSION}
    COMPATIBILITY ExactVersion)

  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    DESTINATION ${LIB_INSTALL_DIR}/${PROJECT_NAME}/cmake)
endfunction(createPackageFiles)



#
# Checks the requiredVersion variable if it is a valid version string
# and sets it to the VERSION_TO_FIND variable.
# Uses the recommendedVersion variable otherwise.
#
# Parameter:
# modulename         - the name of the module, used for log messages
# variablename       - the name of the required variable, used for log messages
# recommendedVersion - the version string, which is used if the required isn't valid
# requiredVersion    - fourth variable (optional); the required version string
#
# Sets the following global variables:
# VERSION_TO_FIND
#
# Usage: set(RECOMMENDED_MYLIB_VERSION 1.0.0.0)
#        setVersionToFind("my-lib" "REQUIRED_MYLIB_VERSION"
#                       ${RECOMMENDED_MYLIB_VERSION} ${REQUIRED_MYLIB_VERSION})
#        find_package(my-lib ${VERSION_TO_FIND} EXACT)
#
function(setVersionToFind modulename variablename recommendedVersion)
  if(NOT ${ARGC} EQUAL 4 OR
     NOT ${ARGV3} MATCHES
       "^(0|[1-9][0-9]*)[.](0|[1-9][0-9]*)[.](0|[1-9][0-9]*)[.](0|[1-9][0-9]*)$")

    log("${variablename} variable was not set or is no valid version number")
    log("Using recommended ${modulename} version, which is: v${recommendedVersion}")
    set(VERSION_TO_FIND ${recommendedVersion} PARENT_SCOPE)

  else()
    set(requiredVersion ${ARGV3})

    if(NOT ${requiredVersion} VERSION_EQUAL ${recommendedVersion})
      log("You are not using the recommended ${modulename} version, which would be: v${recommendedVersion}")
    endif()

    set(VERSION_TO_FIND ${requiredVersion} PARENT_SCOPE)
  endif()
endfunction(setVersionToFind requiredVersion recommendedVersion modulename)



#
# Checks if the module was found by evaluating the MY_LIB_FOUND variable.
# The MY_LIB_FOUND variable is set by the cmake function "find_package(..)"
#
# Parameter:
# modulename         - the name of the module, used for log messages
# variablename       - the name of the required variable, used for log messages
#
# Triggers a FATAL_ERROR if the MY_LIB_FOUND variable is not set.
#
# Usage: find_package(my-lib ${VERSION_TO_FIND} EXACT)
#        checkIfModuleFound("my-lib" "REQUIRED_MY_LIB_VERSION")
#
#
function(checkIfModuleFound modulename variablename)
  if(${modulename}_FOUND) # set by find_package()
     log("Version check done; v${VERSION_TO_FIND} found.")

  else()
    message(FATAL_ERROR "Make sure the required ${modulename} version is available "
            "or set the ${variablename} variable appropriately.")

  endif()
endfunction(checkIfModuleFound modulename)

