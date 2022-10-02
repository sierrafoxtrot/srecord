#
# srecord - Manipulate EPROM load files
# Copyright (C) 2022 Scott Finneran
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

# Build srecord executable
file(GLOB_RECURSE SREC_INFO_SRC "*.cc")
add_executable(srec_info ${SREC_INFO_SRC})
target_link_libraries(srec_info lib_srecord ${LIB_GCRYPT})

INSTALL_SRECORD_EXECUTABLE_TARGET(srec_info)
