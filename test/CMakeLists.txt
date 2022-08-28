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

#Build test support executables
file(GLOB_RECURSE TEST_ARGLEX_AMBIGUOUS_SRC "arglex_ambiguous/*.cc")
add_executable(test_arglex_ambiguous ${TEST_ARGLEX_AMBIGUOUS_SRC})
target_link_libraries(test_arglex_ambiguous srecord ${LIB_GCRYPT})

file(GLOB_RECURSE TEST_CRC16_SRC "crc16/*.cc")
add_executable(test_crc16 ${TEST_CRC16_SRC})
target_link_libraries(test_crc16 srecord)

file(GLOB_RECURSE TEST_FLETCHER16_SRC "fletcher16/*.cc")
add_executable(test_fletcher16 ${TEST_FLETCHER16_SRC})
target_link_libraries(test_fletcher16 srecord)

file(GLOB_RECURSE TEST_GECOS_SRC "gecos/*.cc")
add_executable(test_gecos ${TEST_GECOS_SRC})
target_link_libraries(test_gecos srecord)

file(GLOB_RECURSE TEST_HYPHEN_SRC "hyphen/*.cc")
add_executable(test_hyphen ${TEST_HYPHEN_SRC})
target_link_libraries(test_hyphen srecord)

file(GLOB_RECURSE TEST_URL_DECODE_SRC "url_decode/*.cc")
add_executable(test_url_decode ${TEST_URL_DECODE_SRC})
target_link_libraries(test_url_decode srecord)

configure_file(${CMAKE_SOURCE_DIR}/script/test_prelude.sh ${CMAKE_BINARY_DIR}/test_prelude COPYONLY)

# Tests
enable_testing()
file(GLOB_RECURSE SRECORD_TESTS "*/*.sh")
foreach(t ${SRECORD_TESTS})
  # TODO: would be nice to extract test description from .sh file
  file(STRINGS ${t} test_subject REGEX ^TEST_SUBJECT)
#  message(${test_subject})
#  string(REGEX REPLACE "TEST_SUBJECT\=" "" test_name ${test_subject})
#  message(${test_name})
#  add_test(NAME ${test_name} COMMAND $ENV{SHELL} ${t})

  add_test(NAME ${t} COMMAND $ENV{SHELL} ${t})

  #  set_tests_properties(testPyMyproj PROPERTIES
  #    ENVIRONMENT LD_LIBRARY_PATH=${CMAKE_BINARY_DIR}/lib:$ENV{LD_LIBRARY_PATH})
  set_tests_properties(${t} PROPERTIES ENVIRONMENT PATH=${CMAKE_BINARY_DIR}:${CMAKE_BINARY_DIR}/srec_cat:${CMAKE_BINARY_DIR}/srec_cmp:${CMAKE_BINARY_DIR}/srec_info:${CMAKE_BINARY_DIR}/test:${CMAKE_SOURCE_DIR}/script:$ENV{PATH})
endforeach()
