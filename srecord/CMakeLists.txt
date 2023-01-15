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

# lib_srecord - Need to move away from globbing
file(GLOB_RECURSE LIB_SRECORD_SRC "*.cc")
file(GLOB_RECURSE LIB_SRECORD_HDR "*.h")

message(STATUS "gcrypt location ${LIB_GCRYPT}")
add_library(lib_srecord STATIC ${LIB_SRECORD_SRC} ${LIB_SRECORD_HDR} ${LIB_GCRYPT})
target_compile_features(lib_srecord PUBLIC cxx_std_11)

# Install the library
install(TARGETS lib_srecord
        DESTINATION ${CMAKE_INSTALL_LIBDIR})

# Install the development headers
# A more modern approach would use a cmake 3.23 FILE SET
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING PATTERN "*.h")

# Install the Doxygen output for the website
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html
        DESTINATION ${CMAKE_INSTALL_DOCDIR}/htdocs/srecord
        COMPONENT website)

# Generate config.h to suit local environment
configure_file(config.h.in config.h @ONLY)
configure_file(patchlevel.h.in patchlevel.h @ONLY)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# Generate doxygen content
doxygen_add_docs(doxygen
                 ${LIB_SRECORD_HDR}
                 ALL)
