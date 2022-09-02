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

# libSrecord - Need to move away from globbing
file(GLOB_RECURSE LIB_SRECORD_SRC "*.cc")

message(STATUS "gcrypt location ${LIB_GCRYPT}")
add_library(srecord STATIC ${LIB_SRECORD_SRC} ${LIB_GCRYPT})

# Set the location for library installation -- i.e., /usr/lib in this case
# not really necessary in this example. Use "sudo make install" to apply
install(TARGETS srecord
	DESTINATION ${CMAKE_INSTALL_LIBDIR})
