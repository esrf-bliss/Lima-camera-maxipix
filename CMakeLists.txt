############################################################################
# This file is part of LImA, a Library for Image Acquisition
#
# Copyright (C) : 2009-2017
# European Synchrotron Radiation Facility
# BP 220, Grenoble 38043
# FRANCE
#
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, see <http://www.gnu.org/licenses/>.
############################################################################

cmake_minimum_required(VERSION 3.1)

project(pco)
include(cmake/project_version.cmake)

set(NAME "maxipix") # This is used by LimaTools

# Include additional modules that are used inconditionnaly
include(GNUInstallDirs)
include(GenerateExportHeader)

# If conda build, always set lib dir to 'lib'
if($ENV{CONDA_BUILD})
  set(CMAKE_INSTALL_LIBDIR "lib")
endif()

# Check if project is being used directly or via add_subdirectory
set(CAMERA_MASTER_PROJECT OFF)
if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  set(CAMERA_MASTER_PROJECT ON)
endif()

if (CAMERA_MASTER_PROJECT)
  find_package(Lima REQUIRED)
endif()

# CMake additional macros
if(CAMERA_MASTER_PROJECT)
  list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${LIMA_CMAKE_INCLUDE_DIRS} ${ESPIA_CMAKE_INCLUDE_DIRS})
else()
  list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_CURRENT_SOURCE_DIR}/../common/espia/cmake)
endif()

# Enable python binding code compilation using sip generator
option(CAMERA_ENABLE_PYTHON "compile python binding code?" LIMA_ENABLE_PYTHON)

if (CAMERA_ENABLE_PYTHON)
  # Find python interpreter and libs
  find_package(PythonInterp)
  find_package(PythonLibs)
  if(${PYTHONINTERP_FOUND})
    # python site-packages folder
    execute_process(
      COMMAND ${PYTHON_EXECUTABLE} -c "from distutils.sysconfig import get_python_lib; print (get_python_lib())"
      OUTPUT_VARIABLE _PYTHON_SITE_PACKAGES_DIR OUTPUT_STRIP_TRAILING_WHITESPACE)
  endif()

  set(PYTHON_SITE_PACKAGES_DIR ${_PYTHON_SITE_PACKAGES_DIR} CACHE PATH "where should python modules be installed?")

  find_package(SIP REQUIRED)
  include(SIPMacros)
  if(WIN32)
    set(SIP_TAGS WIN32_PLATFORM)
  elseif(UNIX)
    set(SIP_TAGS POSIX_PLATFORM)
  endif(WIN32)
  set(SIP_EXTRA_OPTIONS -e -g)

  find_package(NumPy REQUIRED)
endif()

find_package(LimaCommonEspia REQUIRED)

file(GLOB_RECURSE MAXIPIX_INCS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")

add_library(maxipix SHARED
  src/PriamSerial.cpp
  src/PriamAcq.cpp
  src/PixelArray.cpp
  src/MaxipixReconstruction.cpp
  src/MaxipixCamera.cpp
  src/MaxipixInterface.cpp
  src/MpxDetConfig.cpp
  src/MpxCommon.cpp
  src/MpxChipConfig.cpp
  src/MpxDacs
  tools/src/INIReader.cpp
  tools/ini.c
  ${MAXIPIX_INCS}
)

# Generate export macros
generate_export_header(maxipix)

# Add dependencies
target_include_directories(maxipix
    PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/tools/src>"
    PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"           # For export header
    PUBLIC "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

# Set version
set_target_properties(maxipix PROPERTIES
    VERSION "${VERSION}"
    SOVERSION "${PACKAGE_VERSION_MAJOR}.${PACKAGE_VERSION_MINOR}")

target_link_libraries(maxipix PUBLIC limacore)

target_compile_definitions(maxipix PUBLIC ${ANDOR3_DEFINITIONS})

target_include_directories(maxipix PUBLIC $<BUILD_INTERFACE:${ANDOR3_INCLUDE_DIRS}>)

target_link_libraries(maxipix PUBLIC ${ANDOR3_LIBRARIES})

if(WIN32)
  target_compile_definitions(maxipix
    PRIVATE maxipix_EXPORTS
    PUBLIC NOMINMAX)
  set_target_properties(maxipix PROPERTIES PREFIX "lib" IMPORT_PREFIX "lib")
endif()

# Binding code for python
if(CAMERA_ENABLE_PYTHON)
  include(LimaTools)
  limatools_run_sip_for_camera(maxipix)
endif()

# Generate and install package config file and version
if(CAMERA_MASTER_PROJECT)
    set(PROJECT_LIBRARIES maxipix)
    set(SIP_INSTALL_DIR ${CMAKE_INSTALL_DATADIR}/sip/lima)
    set(CMAKE_INSTALL_DIR ${CMAKE_INSTALL_DATADIR}/cmake/lima)
    include(package_config)
endif()

## Installation

install(
  TARGETS maxipix
  EXPORT "${TARGETS_EXPORT_NAME}"
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}   # import library
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}   # .so files are libraries
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}   # .dll files are binaries
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}  # this does not actually install anything (but used by downstream projects)
)

install(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
  COMPONENT devel
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  FILES_MATCHING PATTERN "*.h"
)

install(
  FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/project_version.h
    ${PROJECT_BINARY_DIR}/maxipix_export.h
  COMPONENT devel
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

## Tests
if(CAMERA_ENABLE_TESTS)
    enable_testing()
    #add_subdirectory(test)
endif()
