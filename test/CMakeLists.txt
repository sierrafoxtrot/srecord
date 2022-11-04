#
# srecord - Manipulate EPROM load files
# Copyright (C) 2018 Scott Finneran
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

# Build test support executables
file(GLOB_RECURSE TEST_ARGLEX_AMBIGUOUS_SRC "arglex_ambiguous/*.cc")
add_executable(test_arglex_ambiguous ${TEST_ARGLEX_AMBIGUOUS_SRC})
target_link_libraries(test_arglex_ambiguous lib_srecord ${LIB_GCRYPT})

file(GLOB_RECURSE TEST_CRC16_SRC "crc16/*.cc")
add_executable(test_crc16 ${TEST_CRC16_SRC})
target_link_libraries(test_crc16 lib_srecord)

file(GLOB_RECURSE TEST_FLETCHER16_SRC "fletcher16/*.cc")
add_executable(test_fletcher16 ${TEST_FLETCHER16_SRC})
target_link_libraries(test_fletcher16 lib_srecord)

file(GLOB_RECURSE TEST_HYPHEN_SRC "hyphen/*.cc")
add_executable(test_hyphen ${TEST_HYPHEN_SRC})
target_link_libraries(test_hyphen lib_srecord)

file(GLOB_RECURSE TEST_URL_DECODE_SRC "url_decode/*.cc")
add_executable(test_url_decode ${TEST_URL_DECODE_SRC})
target_link_libraries(test_url_decode lib_srecord)

add_custom_target(prepare-test
    DEPENDS
        srecord-executables
        test_arglex_ambiguous
        test_crc16
        test_fletcher16
        test_hyphen
        test_url_decode
)

# Tests
enable_testing()
file(GLOB_RECURSE SRECORD_TESTS "*/*.sh")
foreach(t ${SRECORD_TESTS})
    cmake_path(GET t FILENAME filename)

    # Extract test description from .sh file
    file(STRINGS ${t} test_subject REGEX ^TEST_SUBJECT)
    string(REGEX REPLACE "TEST_SUBJECT\=" "" subject ${test_subject})
    set(test_name "${filename}: ${subject}")

    add_test(NAME ${test_name} COMMAND sh ${t})
    set_property(TEST ${test_name} APPEND PROPERTY
        ENVIRONMENT_MODIFICATION
        PATH=path_list_prepend:${CMAKE_BINARY_DIR}/srec_cat
        PATH=path_list_prepend:${CMAKE_BINARY_DIR}/srec_cmp
        PATH=path_list_prepend:${CMAKE_BINARY_DIR}/srec_info
        PATH=path_list_prepend:${CMAKE_BINARY_DIR}/test
        PATH=path_list_prepend:${CMAKE_SOURCE_DIR}/test)
endforeach()
