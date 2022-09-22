#
# srecord - Manipulate EPROM load files
# Copyright (C) 2018 - 2022 Scott Finneran
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or (at
# your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.
#

cmake_minimum_required(VERSION 3.22)
project(srecord VERSION "1.64.3")
set(CMAKE_BUILD_TYPE Release)

# Headers are all relative to project root
include_directories(./)

# Support standard install locations
include(GNUInstallDirs)

# FHS compliant paths for Linux installation
if(NOT WIN32 AND CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
#  set(CMAKE_INSTALL_PREFIX "/opt/${PROJECT_NAME}")
  set(CMAKE_INSTALL_PREFIX "/usr")
endif()

# Pull in the rest of the pieces
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/etc")
include(configure)

# libsrecord
add_subdirectory(srecord)

# Build srecord executables
add_subdirectory(srec_cat)
add_subdirectory(srec_cmp)
add_subdirectory(srec_info)

# Tests & support tools
enable_testing()
add_subdirectory(test)

# Documentation & Man Pages
add_subdirectory(doc)

# Packaging
set(CPACK_PACKAGE_NAME srecord)
set(CPACK_PACKAGE_VENDOR "Scott Finneran")
set(CPACK_PACKAGE_CONTACT "Scott Finneran <scottfinneran@yahoo.com.au>")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "SRecord - Manipulate EPROM Load Files")
set(CPACK_PACKAGE_INSTALL_DIRECTORY ${CPACK_PACKAGE_NAME})
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_VERBATIM_VARIABLES YES)
#set(CPACK_PACKAGE_DESCRIPTION_FILE ${CMAKE_CURRENT_LIST_DIR}/Description.txt)
#set(CPACK_RESOURCE_FILE_WELCOME ${CMAKE_CURRENT_LIST_DIR}/Welcome.txt)
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_LIST_DIR}/LICENSE)
#set(CPACK_RESOURCE_FILE_README ${CMAKE_CURRENT_LIST_DIR}/Readme.txt)
SET (CPACK_DEBIAN_PACKAGE_DEPENDS "libboost-dev")
SET (CPACK_DEBIAN_PACKAGE_DEPENDS "libgcrypt-dev")

set(CPACK_SOURCE_IGNORE_FILES
  /\\.git/
  \\.swp
  \\.orig
  /CMakeLists\\.txt\\.user
  /build/
)

if(WIN32)
  message(STATUS "Packaging for Windows")
  set(CPACK_GENERATOR ZIP WIX)
elseif(APPLE)
  message(STATUS "Packaging for Macintosh")
  set(CPACK_GENERATOR TGZ productbuild)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  message(STATUS "Packaging for Linux")
  set(CPACK_GENERATOR TGZ RPM DEB)
else()
  message(STATUS "OS not detected. Packaging default")
  set(CPACK_GENERATOR TGZ)
endif()

include(CPack)
