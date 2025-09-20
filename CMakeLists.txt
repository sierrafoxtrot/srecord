#
# srecord - Manipulate EPROM load files
# Copyright (C) 2018 - 2023 Scott Finneran
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
project(srecord VERSION "1.65")
set(CMAKE_BUILD_TYPE Release)

# Generate compile_commands.json for use by RTags, clang tools etc.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Headers are all relative to project root
include_directories(./)

# Support for pulling in dependencies required for Windows install
include(InstallRequiredSystemLibraries)

# Support standard install locations
include(GNUInstallDirs)

# FHS compliant paths for Linux installation
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT
        AND NOT WIN32 AND CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  set(CMAKE_INSTALL_PREFIX "/usr" CACHE PATH "Install prefix" FORCE)
endif()

# Pull in the rest of the pieces
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/etc")
include(configure)

# Packaging configuration
include(packaging)

# libsrecord
add_subdirectory(srecord)

# Build srecord executables
add_custom_target(srecord-executables)
add_custom_target(srecord-executables-version DEPENDS srecord-executables)
foreach(p IN ITEMS srec_cat srec_cmp srec_info)
  add_subdirectory("${p}")

  add_dependencies(srecord-executables "${p}")
  if(NOT CMAKE_CROSSCOMPILING)
    add_custom_target("${p}-version" COMMAND "${p}" --version)
    add_dependencies(srecord-executables-version "${p}-version")
  endif()
endforeach()

# Tests & support tools
enable_testing()
add_subdirectory(test)

# Documentation & Man Pages
add_subdirectory(doc)

# Package SRecord
include(CPack)
