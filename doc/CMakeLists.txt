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

# Build Documentation and man pages
configure_file(script/groff.sh ${CMAKE_BINARY_DIR}/groff.sh COPYONLY)
configure_file(script/page_select.sh ${CMAKE_BINARY_DIR}/page_select.sh COPYONLY)
configure_file(script/ref-ptx.sh  ${CMAKE_BINARY_DIR}/ref_ptx.sh COPYONLY)
configure_file(script/new.sh  ${CMAKE_BINARY_DIR}/new.sh COPYONLY)
configure_file(script/build_ref_index.sh ${CMAKE_BINARY_DIR}/build_ref_index.sh COPYONLY)

set(VERBOSE yep)

set(GROFF sh ${CMAKE_BINARY_DIR}/groff.sh)
set(PAGE_SELECT sh ${CMAKE_BINARY_DIR}/page_select.sh)
set(NEW_CH sh ${CMAKE_BINARY_DIR}/new.sh)
set(REF_PTX sh ${CMAKE_BINARY_DIR}/ref_ptx.sh)
set(BUILD_REF_INDEX sh ${CMAKE_BINARY_DIR}/build_ref_index.sh)

# man pages
file(GLOB_RECURSE MAN_PAGES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "man*/*.[135]")
file(GLOB_RECURSE MAN_COMMON RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "man*/[zo]_*.so")
file(GLOB_RECURSE NEW_CHANGE_NOTES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "etc/new.*.so")

foreach(m_src ${MAN_PAGES})
  set(m_target ${CMAKE_BINARY_DIR}/doc/${m_src})
  add_custom_command(OUTPUT ${m_target}
#    COMMAND soelim -I${CMAKE_CURRENT_SOURCE_DIR}  ${m_src} | sed '/^[.]XX ./d' > ${m_target}
    COMMAND soelim -I${CMAKE_CURRENT_SOURCE_DIR} ${m_src} > ${m_target}
    DEPENDS ${m_src} ${MAN_COMMON}
    COMMENT "Building ${m_target}"
    VERBATIM)
  list(APPEND MAN_PAGE_OUTPUTS ${m_target})
endforeach()

add_custom_target(man ALL DEPENDS ${MAN_PAGE_OUTPUTS})

# Install all the man pages on not-windows
if(NOT WIN32)
  set(sections 1 3 5)
  foreach(i ${sections})
    install(DIRECTORY ${CMAKE_BINARY_DIR}/doc/man${i}/
            DESTINATION ${CMAKE_INSTALL_MANDIR}/man${i}
	    FILES_MATCHING PATTERN "*.${i}")
  endforeach()
endif()

# Reference Manual Components
set(REF_INDEX ${CMAKE_BINARY_DIR}/doc/ref-index.so)
add_custom_command(OUTPUT ${REF_INDEX}
  COMMAND ${BUILD_REF_INDEX} ${CMAKE_CURRENT_SOURCE_DIR} ${REF_INDEX} ${MAN_PAGES}
  DEPENDS ${MAN_PAGES} ${CMAKE_CURRENT_SOURCE_DIR}/script/ref-ptx.ignore ${CMAKE_CURRENT_SOURCE_DIR}/script/ref-ptx1.awk ${CMAKE_CURRENT_SOURCE_DIR}/script/ref-ptx2.awk
  COMMENT "Building ${REF_INDEX}"
  VERBATIM
  )

set(REF_PARTS ${CMAKE_BINARY_DIR}/doc/ref-parts.so)
add_custom_command(OUTPUT ${REF_PARTS}
  COMMAND ${REF_PTX} ${MAN_PAGES} > ${REF_PARTS}
  DEPENDS ${MAN_PAGES} ${CMAKE_CURRENT_SOURCE_DIR}/script/ref-ptx.sh
  COMMENT "Building ${REF_PARTS}"
  VERBATIM
  )

set(NEW_CHANGES ${CMAKE_BINARY_DIR}/doc/new.so)
add_custom_command(OUTPUT ${NEW_CHANGES}
  COMMAND ${NEW_CH} -r ${NEW_CHANGE_NOTES} > ${NEW_CHANGES}
  DEPENDS ${NEW_CHANGE_NOTES} ${CMAKE_CURRENT_SOURCE_DIR}/script/ref-ptx.sh
  COMMENT "Building ${NEW_CHANGES}"
  VERBATIM
  )

# Standalone Documents

# Transform man page(s) (ie troff format files) into a PDF
FUNCTION(ADD_DOC TARGET SOURCE DEPS)
  add_custom_command(OUTPUT ${TARGET}
    COMMAND ${GROFF} -Tps -s -I${CMAKE_CURRENT_SOURCE_DIR} -I${CMAKE_BINARY_DIR} -t -man ${SOURCE} > ${TARGET}.ps.tmp
    COMMAND ${PAGE_SELECT} ${CMAKE_CURRENT_SOURCE_DIR}/etc/page-list.awk ${TARGET}.ps.tmp ${TARGET}.ps
    COMMAND ps2pdf ${TARGET}.ps ${TARGET}
    COMMAND rm ${TARGET}.ps ${TARGET}.ps.tmp
    DEPENDS ${SOURCE} ${DEPS}
    COMMENT "Building ${TARGET}"
    VERBATIM)
  list(APPEND DOC_TARGETS ${TARGET})
  set(DOC_TARGETS ${DOC_TARGETS} PARENT_SCOPE)
ENDFUNCTION()

# Target directories
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/doc)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/doc/man1)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/doc/man3)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/doc/man5)

ADD_DOC(${CMAKE_BINARY_DIR}/doc/BUILDING.pdf ${CMAKE_CURRENT_SOURCE_DIR}/etc/BUILDING.man "")
ADD_DOC(${CMAKE_BINARY_DIR}/doc/README.pdf ${CMAKE_CURRENT_SOURCE_DIR}/etc/README.man "")
ADD_DOC(${CMAKE_BINARY_DIR}/doc/change_log.pdf ${CMAKE_CURRENT_SOURCE_DIR}/etc/change_log.man "${NEW_CHANGES}")
ADD_DOC(${CMAKE_BINARY_DIR}/doc/reference.pdf ${CMAKE_CURRENT_SOURCE_DIR}/etc/reference.man "${REF_PARTS};${REF_INDEX}")

# Install all the PDFs
install(FILES ${DOC_TARGETS}
        DESTINATION ${CMAKE_INSTALL_DOCDIR})

add_custom_target(doco ALL DEPENDS ${DOC_TARGETS})
