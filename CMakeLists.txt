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

# Support for pulling in dependencies required for Windows install
include(InstallRequiredSystemLibraries)

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

# Packaging configuration
include(packaging)

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

# Package SRecord
include(CPack)