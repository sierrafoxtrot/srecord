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
project(srecord)
set(CMAKE_BUILD_TYPE Release)

# Headers are all relative to project root
include_directories(./)

# Support standard install locations
include(GNUInstallDirs)

# Pull in the rest of the pieces
include(etc/configure.cmake)
#include(etc/documentation.cmake)

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

# Installation
if(NOT WIN32 AND CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
#  set(CMAKE_INSTALL_PREFIX "/opt/mycompany.com/${PROJECT_NAME}")
  set(CMAKE_INSTALL_PREFIX "/usr")
endif()
