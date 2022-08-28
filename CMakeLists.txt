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

# Pull in the rest of the pieces
include(etc/configure.cmake)
include(etc/documentation.cmake)
include(etc/tests.cmake)

add_subdirectory(srecord)
# # Pull in the libSrecord sources
# file(GLOB_RECURSE LIB_SRECORD_SRC "srecord/*.cc")

# # Generate the shared library from the sources
# set (LIB_SRECORD libsrecord.so)
# link_directories( ~/work/srecord-cmake/build)
# add_library(srecord SHARED ${LIB_SRECORD_SRC})
# target_link_libraries(srecord -lgcrypt -lgpg-error)

# Set the location for library installation -- i.e., /usr/lib in this case
# not really necessary in this example. Use "sudo make install" to apply
#install(TARGETS srecord DESTINATION /usr/lib)

# Build srecord executables
file(GLOB_RECURSE SREC_CAT_SRC "srec_cat/*.cc")
add_executable(srec_cat ${SREC_CAT_SRC})
target_link_libraries(srec_cat srecord ${LIB_GCRYPT})

file(GLOB_RECURSE SREC_CMP_SRC "srec_cmp/*.cc")
add_executable(srec_cmp ${SREC_CMP_SRC})
target_link_libraries(srec_cmp srecord ${LIB_GCRYPT})

file(GLOB_RECURSE SREC_INFO_SRC "srec_info/*.cc")
add_executable(srec_info ${SREC_INFO_SRC})
target_link_libraries(srec_info srecord ${LIB_GCRYPT})
