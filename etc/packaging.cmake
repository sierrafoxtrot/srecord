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

# Installation
if (WIN32)
  set(CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION ${CMAKE_INSTALL_BINDIR})
else()
  set(CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif()

FUNCTION(INSTALL_SRECORD_EXECUTABLE_TARGET target)
  # Get runtime dependencies
  install(TARGETS ${target}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT ${target}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT ${target}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT ${target}
    )
if (WIN32)
  message(STATUS "CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION ${CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION}")
  message(STATUS "CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}")
  # Find standard library DLL's (if needed, this covers mingw clang and gcc on windows)
  cmake_path(REMOVE_FILENAME CMAKE_C_COMPILER OUTPUT_VARIABLE COMPILER_C_DIR)
  cmake_path(REMOVE_FILENAME CMAKE_CXX_COMPILER OUTPUT_VARIABLE COMPILER_CXX_DIR)
  set(DLL_SEARCH_DIRS "")
  if ("${COMPILER_C_DIR}" STREQUAL "${COMPILER_CXX_DIR}")
    list(APPEND DLL_SEARCH_DIRS "${COMPILER_C_DIR}")
  else()
    list(APPEND DLL_SEARCH_DIRS "${COMPILER_C_DIR}")
    list(APPEND DLL_SEARCH_DIRS "${COMPILER_CXX_DIR}")
  endif()

  install(IMPORTED_RUNTIME_ARTIFACTS ${target}
    RUNTIME_DEPENDENCY_SET ${PROJECT_NAME}_dlls COMPONENT ${target})
  install(RUNTIME_DEPENDENCY_SET ${PROJECT_NAME}_dlls
    PRE_EXCLUDE_REGEXES "api-ms-" "ext-ms-"
    POST_EXCLUDE_REGEXES ".*system32/.*\\.dll"
    DIRECTORIES ${DLL_SEARCH_DIRS}
    COMPONENT ${target}
    )
endif()
ENDFUNCTION()

# Packaging
set(CPACK_PACKAGE_NAME srecord)
set(CPACK_PACKAGE_VENDOR "Scott Finneran")
set(CPACK_PACKAGE_CONTACT "Scott Finneran <scottfinneran@yahoo.com.au>")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "SRecord - Manipulate EPROM Load Files")
set(CPACK_PACKAGE_INSTALL_DIRECTORY ${CPACK_PACKAGE_NAME})
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_VERBATIM_VARIABLES YES)
#set(CPACK_PACKAGE_DESCRIPTION_FILE ${CMAKE_CURRENT_LIST_DIR}/Description.txt)
#set(CPACK_RESOURCE_FILE_WELCOME ${CMAKE_CURRENT_LIST_DIR}/Welcome.txt)
#set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_LIST_DIR}/LICENSE)
set(CPACK_RESOURCE_FILE_LICENSE ${PROJECT_SOURCE_DIR}/LICENSE)
#set(CPACK_RESOURCE_FILE_README ${CMAKE_CURRENT_LIST_DIR}/Readme.txt)

# Debian Specifics
SET (CPACK_DEBIAN_PACKAGE_DEPENDS "libgcrypt-dev")

# Windows(NSIS) Specifics
set(CPACK_NSIS_MUI_ICON ${CMAKE_CURRENT_SOURCE_DIR}/doc/html/srecord.ico)
set(CPACK_NSIS_MUI_UNIICON ${CMAKE_CURRENT_SOURCE_DIR}/doc/html/srecord.ico)
set(CPACK_NSIS_INSTALLED_ICON_NAME ${CMAKE_CURRENT_SOURCE_DIR}/doc/html/srecord.ico)
set(CPACK_NSIS_DISPLAY_NAME "SRecord")
set(CPACK_NSIS_PACKAGE_NAME "SRecord")
set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL YES)

# Helper script to add the BUILDING instructions into the source distribution
configure_file(etc/my_install_script.cmake.in my_install_script.cmake @ONLY)
set(CPACK_INSTALL_SCRIPT "${CMAKE_BINARY_DIR}/my_install_script.cmake")

set(CPACK_SOURCE_IGNORE_FILES
  /\\.git/
  \\.gitignore
  \\.swp
  \\.orig
  /CMakeLists\\.txt\\.user
  /build/
)

if(WIN32)
  message(STATUS "Packaging for Windows")
  set(CPACK_GENERATOR ZIP WIX)
elseif(APPLE)
  message(STATUS "Packaging for Macintosh")
  set(CPACK_GENERATOR TGZ productbuild)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  message(STATUS "Packaging for Linux")
  set(CPACK_GENERATOR TGZ RPM DEB)
else()
  message(STATUS "OS not detected. Packaging default")
  set(CPACK_GENERATOR TGZ)
endif()
