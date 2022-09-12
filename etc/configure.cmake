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

# Generated content

# Get the git hash
find_package(Git)
if(Git_FOUND)
  message("Git found: ${GIT_EXECUTABLE}")
  execute_process(COMMAND
    "${GIT_EXECUTABLE}" describe --match=NeVeRmAtCh --always --abbrev=10 --dirty
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    OUTPUT_VARIABLE GIT_SHA1
    ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)
else()
  set(GIT_SHA1 "GIT Hash Not Found")
endif()

message(STATUS "git status: GIT_SHA1 ${GIT_SHA1}")

# Get a single list of copyright years for changes made to SRecord
execute_process(
  COMMAND "${GIT_EXECUTABLE}" log --reverse --date=format:%Y --pretty=format:%as
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
  OUTPUT_VARIABLE DATES
  ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)

string(REGEX MATCHALL "[0-9][0-9][0-9][0-9]" all_years ${DATES})
list(REMOVE_DUPLICATES all_years)
string(REPLACE ";" ", " COPYRIGHT_YEARS "${all_years}")

# Autoconf-like tasks:
# Crypto library
include(CheckIncludeFiles)
check_include_files(gcrypt.h HAVE_GCRYPT_H)
if (HAVE_GCRYPT_H)
  set(LIBGCRYPT_HEADERS_FOUND TRUE)
  find_library(LIB_GCRYPT NAMES gcrypt)
  option(HAVE_LIBGCRYPT "libgcrypt" ON)
  option(HAVE_LIBGCRYPT_SHA224 "libgcrypt SHA244" ON)
  option(HAVE_LIBGCRYPT_WHIRLPOOL "libgcrypt whirlpool" ON)
  option(HAVE_GCRY_MD_HD_T "libgcrypt HAVE_GCRY_MD_HD_T" ON)
endif (HAVE_GCRYPT_H)

# Boost library
find_package(Boost 1.60 REQUIRED)
set(HAVE_BOOST ${Boost_FOUND})

# Stdlib functions
include(CheckCXXSymbolExists)
check_cxx_symbol_exists(snprintf string SNPRINTF_FOUND)
if (SNPRINTF_FOUND)
  option(HAVE_SNPRINTF "snprintf function" ON)
endif()

include(CheckCXXSymbolExists)
check_cxx_symbol_exists(vsnprintf string VSNPRINTF_FOUND)
if (VSNPRINTF_FOUND)
  option(HAVE_VSNPRINTF "vsnprintf function" ON)
endif()

# Extensions
# Support for sparse file seeking
option(HAVE_SPARSE_LSEEK OFF)

# Enable extensions on AIX 3, Interix.
option(_ALL_SOURCE ON)

# Enable GNU extensions on systems that have them.
option(_GNU_SOURCE ON)

# Enable threading extensions on Solaris.  */
option(_POSIX_PTHREAD_SEMANTICS ON)

# Enable extensions on HP NonStop.
option(_TANDEM_SOURCE ON)

# Enable general extensions on Solaris.
option(__EXTENSIONS__ ON)
