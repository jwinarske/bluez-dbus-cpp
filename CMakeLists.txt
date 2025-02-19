##
# Copyright Joel Winarske 2025.
# Copyright Audiofile LLC 2019 - 2020.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
##

cmake_minimum_required(VERSION 3.10.2)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: Debug, Release, RelWithDebInfo, or MinSizeRel." FORCE)
    message(STATUS "CMAKE_BUILD_TYPE not set, defaulting to Release.")
endif ()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/third_party/sanitizers-cmake/cmake)

project(bluez-dbus-cpp
        VERSION 0.0.1
        LANGUAGES C CXX
)

add_subdirectory(third_party)

# Installation directories for `install` command and pkgconfig file
include(GNUInstallDirs)

# Set up directory with 3rd party cmake modules
# set (CMAKE_MODULE_PATH
# 	${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/build_files/cmake/modules/")

#-------------------------------
# GENERAL COMPILER CONFIGURATION
#-------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#-------------------------------
# SOURCE FILES CONFIGURATION
#-------------------------------
set(BLUEZ_HEADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(BLUEZ_HEADER_PREFIX "bluez-dbus-cpp")
set(BLUEZ_HEADERS
        "${BLUEZ_HEADER_DIR}/${BLUEZ_HEADER_PREFIX}/adaptor/GattCharacteristic1_adaptor.h"
        "${BLUEZ_HEADER_DIR}/${BLUEZ_HEADER_PREFIX}/adaptor/GattDescriptor1_adaptor.h"
        "${BLUEZ_HEADER_DIR}/${BLUEZ_HEADER_PREFIX}/adaptor/GattService1_adaptor.h"
        "${BLUEZ_HEADER_DIR}/${BLUEZ_HEADER_PREFIX}/adaptor/ObjectManagerExt_adaptor.h"
        "${BLUEZ_HEADER_DIR}/${BLUEZ_HEADER_PREFIX}/adaptor/PropertiesExt_adaptor.h"
        "${BLUEZ_HEADER_DIR}/${BLUEZ_HEADER_PREFIX}/proxy/Adapter1_proxy.h"
        "${BLUEZ_HEADER_DIR}/${BLUEZ_HEADER_PREFIX}/proxy/AgentManager1_proxy.h"
        "${BLUEZ_HEADER_DIR}/${BLUEZ_HEADER_PREFIX}/proxy/HealthManager1_proxy.h"
        "${BLUEZ_HEADER_DIR}/${BLUEZ_HEADER_PREFIX}/proxy/Media1_proxy.h"
        "${BLUEZ_HEADER_DIR}/${BLUEZ_HEADER_PREFIX}/proxy/NetworkServer1_proxy.h"
        "${BLUEZ_HEADER_DIR}/${BLUEZ_HEADER_PREFIX}/proxy/ProfileManager1_proxy.h"
        "${BLUEZ_HEADER_DIR}/${BLUEZ_HEADER_PREFIX}/proxy/SimAccess1_proxy.h"
        "${BLUEZ_HEADER_DIR}/${BLUEZ_HEADER_PREFIX}/Adapter1.h"
        "${BLUEZ_HEADER_DIR}/${BLUEZ_HEADER_PREFIX}/bluez.h"
        "${BLUEZ_HEADER_DIR}/${BLUEZ_HEADER_PREFIX}/GattApplication1.h"
        "${BLUEZ_HEADER_DIR}/${BLUEZ_HEADER_PREFIX}/GattCharacteristic1.h"
        "${BLUEZ_HEADER_DIR}/${BLUEZ_HEADER_PREFIX}/GattDescriptor1.h"
        "${BLUEZ_HEADER_DIR}/${BLUEZ_HEADER_PREFIX}/GattManager1.h"
        "${BLUEZ_HEADER_DIR}/${BLUEZ_HEADER_PREFIX}/GattService1.h"
        "${BLUEZ_HEADER_DIR}/${BLUEZ_HEADER_PREFIX}/LEAdvertisement1.h"
        "${BLUEZ_HEADER_DIR}/${BLUEZ_HEADER_PREFIX}/LEAdvertisingManager1.h"
        "${BLUEZ_HEADER_DIR}/${BLUEZ_HEADER_PREFIX}/Util.h"
        "${BLUEZ_HEADER_DIR}/${BLUEZ_HEADER_PREFIX}/Socket.h"
        "${BLUEZ_HEADER_DIR}/${BLUEZ_HEADER_PREFIX}/Client.h"
        "${BLUEZ_HEADER_DIR}/${BLUEZ_HEADER_PREFIX}/bluez-ext.h"
        "${BLUEZ_HEADER_DIR}/${BLUEZ_HEADER_PREFIX}/GenericCharacteristic.h"
        "${BLUEZ_HEADER_DIR}/${BLUEZ_HEADER_PREFIX}/ReadOnlyCharacteristic.h")

set(BLUEZ_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(BLUEZ_SOURCES
        "${BLUEZ_SOURCE_DIR}/GattService1.cpp"
        "${BLUEZ_SOURCE_DIR}/GattCharacteristic1.cpp"
        "${BLUEZ_SOURCE_DIR}/GattDescriptor1.cpp"
        "${BLUEZ_SOURCE_DIR}/Util.cpp")


add_library(bluez-dbus-cpp SHARED
        ${BLUEZ_SOURCES}
        ${BLUEZ_HEADERS})

add_sanitizers(bluez-dbus-cpp)

target_include_directories(bluez-dbus-cpp PUBLIC
        $<BUILD_INTERFACE:${BLUEZ_HEADER_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

option(BUILD_SHARED_LIBS "Build shared libraries (.so) instead of static ones (.a)" ON)

if (DEFINED BUILD_SHARED_LIBS)
    set_target_properties(bluez-dbus-cpp PROPERTIES POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS})
endif ()

set_target_properties(bluez-dbus-cpp
        PROPERTIES PUBLIC_HEADER "${BLUEZ_HEADERS}"
        VERSION "${PROJECT_VERSION}"
        SOVERSION "${PROJECT_VERSION_MAJOR}"
        OUTPUT_NAME "bluez-dbus-cpp")

# Link up
find_package(sdbus-c++ REQUIRED)
target_link_libraries(bluez-dbus-cpp
        PUBLIC
        SDBusCpp::sdbus-c++
)
target_link_libraries(bluez-dbus-cpp
        PRIVATE
        spdlog
)

#-------------------------------
# INSTALL
#-------------------------------
install(TARGETS bluez-dbus-cpp
        EXPORT bluez-dbus-cpp-targets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT libraries
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT libraries
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT static_libraries
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${BLUEZ_HEADER_PREFIX} COMPONENT dev
        COMPONENT dev)

# Manually install headers to preserve folder structure	
install(DIRECTORY "${BLUEZ_HEADER_DIR}/${BLUEZ_HEADER_PREFIX}/adaptor/"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${BLUEZ_HEADER_PREFIX}/adaptor"
        FILES_MATCHING PATTERN "*.h"
)
install(DIRECTORY "${BLUEZ_HEADER_DIR}/${BLUEZ_HEADER_PREFIX}/proxy/"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${BLUEZ_HEADER_PREFIX}/proxy"
        FILES_MATCHING PATTERN "*.h"
)
install(DIRECTORY "${BLUEZ_HEADER_DIR}/${BLUEZ_HEADER_PREFIX}/"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${BLUEZ_HEADER_PREFIX}"
        FILES_MATCHING PATTERN "*.h"
)

# install docs
install(FILES README.md LICENSE_1_0.txt DESTINATION ${CMAKE_INSTALL_DOCDIR})

#-------------------------------
# EXAMPLES
#-------------------------------
if (BUILD_EXAMPLE)
    add_subdirectory(example)
endif ()

if (BUILD_EXAMPLE_ELL)
    add_subdirectory(example-ell-sockets)
endif ()

#----------------------------------
# CMAKE CONFIG & PACKAGE CONFIG
#----------------------------------

include(CMakePackageConfigHelpers)

install(EXPORT bluez-dbus-cpp-targets
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/bluez-dbus-cpp
        NAMESPACE BlueZ::
        COMPONENT dev)

configure_package_config_file(cmake/bluez-dbus-cpp-config.cmake.in cmake/bluez-dbus-cpp-config.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/bluez-dbus-cpp)
write_basic_package_version_file(cmake/bluez-dbus-cpp-config-version.cmake COMPATIBILITY SameMajorVersion)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/cmake/bluez-dbus-cpp-config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/cmake/bluez-dbus-cpp-config-version.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/bluez-dbus-cpp
        COMPONENT dev)

configure_file(pkgconfig/bluez-dbus-cpp.pc.in pkgconfig/bluez-dbus-cpp.pc @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/pkgconfig/bluez-dbus-cpp.pc
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig COMPONENT dev)