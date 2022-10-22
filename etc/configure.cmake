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
function(get_git_hash)
  if(NOT Git_FOUND)
    return()
  endif()

  set(hash_length 10)
  math(EXPR dirty_length "${hash_length} + 6")

  execute_process(COMMAND
    "${GIT_EXECUTABLE}" describe --match=NeVeRmAtCh --always --abbrev=${hash_length} --dirty
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    RESULT_VARIABLE exitcode
    OUTPUT_VARIABLE GIT_SHA1
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET)

  # String length shall either be equal to hash length or +6 for "-dirty" suffix
  string(LENGTH "${GIT_SHA1}" length)

  if(("${exitcode}" EQUAL "0") AND (("${length}" EQUAL "${hash_length}") OR ("${length}" EQUAL "${dirty_length}")))
    message(STATUS "GIT_SHA1 ${GIT_SHA1}")
    set(GIT_SHA1 "${GIT_SHA1}" PARENT_SCOPE)
  endif()
endfunction()

# Get a single list of copyright years for changes made to SRecord
function(get_copyright_years_from_git)
  if(NOT DEFINED GIT_SHA1)
    return()
  endif()

  execute_process(
    COMMAND "${GIT_EXECUTABLE}" log --reverse --date=format:%Y --pretty=format:%ad%n%cd
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    RESULT_VARIABLE exitcode
    OUTPUT_VARIABLE all_years
    ERROR_QUIET)

  if("${exitcode}" EQUAL "0")
    string(REGEX MATCHALL "[[0-9][0-9][0-9][0-9]" all_years ${all_years})
    if(NOT "${all_years}" STREQUAL "")
      list(REMOVE_DUPLICATES all_years)
      list(SORT all_years)
      list(JOIN all_years ", " COPYRIGHT_YEARS)
      set(COPYRIGHT_YEARS "${COPYRIGHT_YEARS}" PARENT_SCOPE)
    endif()
  endif()
endfunction()

find_package(Git)
get_git_hash()
get_copyright_years_from_git()

if(NOT DEFINED COPYRIGHT_YEARS)
  # In all likelihood, we are building from a source copy outside of the
  # git repo so let's use some reasonable/meaningful defaults.
  message(STATUS "Git is not installed or this is not a repository clone")
  set(GIT_SHA1 "unknown")
  set(COPYRIGHT_YEARS "1998...")
endif()
message(STATUS "COPYRIGHT_YEARS ${COPYRIGHT_YEARS}")

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

# ps2pdf used in building the PDF version of the documentation
find_program(PS2PDF ps2pdf)
message(STATUS "PS2PDF ${PS2PDF}")

# cygpath used in helping the build run on Windows
find_program(CYGPATH cygpath)
message(STATUS "CYGPATH ${CYGPATH}")

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

# Doxygen configuration
find_package(Doxygen REQUIRED doxygen dot)

set(DOXYGEN_DOT_GRAPH_MAX_NODES 150)
set(DOXYGEN_ALPHABETICAL_INDEX NO)
set(DOXYGEN_BUILTIN_STL_SUPPORT YES)
set(DOXYGEN_CASE_SENSE_NAMES NO)
set(DOXYGEN_CLASS_DIAGRAMS YES)
set(DOXYGEN_DISTRIBUTE_GROUP_DOC YES)
set(DOXYGEN_EXCLUDE bin)
set(DOXYGEN_EXTRACT_ALL YES)
set(DOXYGEN_EXTRACT_LOCAL_CLASSES NO)
set(DOXYGEN_FILE_PATTERNS *.h)
set(DOXYGEN_GENERATE_TREEVIEW YES)
set(DOXYGEN_HIDE_FRIEND_COMPOUNDS YES)
set(DOXYGEN_HIDE_IN_BODY_DOCS YES)
set(DOXYGEN_HIDE_UNDOC_CLASSES YES)
set(DOXYGEN_HIDE_UNDOC_MEMBERS YES)
set(DOXYGEN_JAVADOC_AUTOBRIEF YES)
set(DOXYGEN_QT_AUTOBRIEF YES)
set(DOXYGEN_QUIET YES)
set(DOXYGEN_RECURSIVE YES)
set(DOXYGEN_REFERENCED_BY_RELATION YES)
set(DOXYGEN_REFERENCES_RELATION YES)
set(DOXYGEN_SORT_BY_SCOPE_NAME YES)
set(DOXYGEN_SORT_MEMBER_DOCS NO)
set(DOXYGEN_SOURCE_BROWSER YES)
set(DOXYGEN_STRIP_CODE_COMMENTS NO)
set(DOXYGEN_GENERATE_HTML YES)
