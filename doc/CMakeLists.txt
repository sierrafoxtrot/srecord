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

# Configuration
configure_file(etc/version.so.in etc/version.so @ONLY)

# Target directories
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/man1)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/man3)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/man5)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html/man)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html/man/man1)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html/man/man3)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html/man/man5)

# man pages
file(GLOB_RECURSE MAN_PAGES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "man*/*.[135]")
file(GLOB_RECURSE MAN_COMMON RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "man*/[zo]_*.so")
file(GLOB_RECURSE NEW_CHANGE_NOTES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "etc/new.*.so")

# ...and now an aside on the subtle differences between linux/unix/posix
# and Windows. Under Msys2, most tools are able to cope with windows path
# format filenames e.g. C:\msys2\bin. groff sadly, doesn't; specifically
# when handling the -I option. So on Windows, let's give groff an assist
# by using the cygpath tool to pass it in unix format.
if(WIN32)
    execute_process(COMMAND ${CYGPATH} -u ${CMAKE_CURRENT_SOURCE_DIR}
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        OUTPUT_VARIABLE DocSourcePath
        ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)

    execute_process(COMMAND ${CYGPATH} -u ${CMAKE_CURRENT_BINARY_DIR}
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
        OUTPUT_VARIABLE DocBinaryPath
        ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)
else()
    set(DocSourcePath ${CMAKE_CURRENT_SOURCE_DIR})
    set(DocBinaryPath ${CMAKE_CURRENT_BINARY_DIR})
endif()

foreach(m_src ${MAN_PAGES})
    # troff formatted man pages
    set(m_target ${CMAKE_CURRENT_BINARY_DIR}/${m_src})
    add_custom_command(OUTPUT ${m_target}
        #COMMAND soelim -I${CMAKE_CURRENT_SOURCE_DIR}  ${m_src} | sed '/^[.]XX ./d' > ${m_target}
        COMMAND soelim -I${CMAKE_CURRENT_SOURCE_DIR} ${m_src} > ${m_target}
        DEPENDS ${m_src} ${MAN_COMMON}
        COMMENT "Building ${m_target}"
        VERBATIM)
    list(APPEND MAN_PAGE_OUTPUTS ${m_target})

    # HTML formatted man pages for web viewing
    set(m_target ${CMAKE_CURRENT_BINARY_DIR}/html/man/${m_src}.html)
    cmake_path(GET m_src STEM image_stem_name)
    cmake_path(GET m_target PARENT_PATH m_target_path)
    add_custom_command(OUTPUT ${m_target}
        COMMAND
            sh ${CMAKE_CURRENT_SOURCE_DIR}/script/groff.sh
            -T html                # Output device
            -man                   # Macro Package man
            # Order of preprocessors may be important
            -t                     # Preprocess with tbl
            -s                     # Preprocess with soelim, with search paths:
            -I ${DocSourcePath}
            -I ${DocBinaryPath}
            -P-I${image_stem_name} # Preprocess with pic
            ${m_src}               # Source
            > ${m_target}
        DEPENDS
            ${m_src}
            ${MAN_COMMON}
            ${CMAKE_CURRENT_SOURCE_DIR}/script/groff.sh
        WORKING_DIRECTORY ${m_target_path}
        COMMENT "Building ${m_target} in ${m_target_path}"
        VERBATIM)
    list(APPEND HTML_MAN_PAGE_OUTPUTS ${m_target})
endforeach()

# Generate the index page for the HTML formatted man pages
set(i_target ${CMAKE_CURRENT_BINARY_DIR}/html/man/index.html)
set(HTML_MAN_PAGE_INDEX ${i_target})
add_custom_command(OUTPUT ${HTML_MAN_PAGE_INDEX}
    COMMAND sh ${CMAKE_CURRENT_SOURCE_DIR}/script/man-html-index.sh ${MAN_PAGES} > ${i_target}
    DEPENDS
        ${MAN_PAGES}
        ${MAN_COMMON}
        ${CMAKE_CURRENT_SOURCE_DIR}/script/man-html-index.sh
    COMMENT "Building the HTML man page index"
    VERBATIM)

add_custom_target(man ALL DEPENDS ${MAN_PAGE_OUTPUTS})
if(NOT WIN32)
    # groff which is used to generate HTML formatted man pages relies on underlying
    # tools from the netpmb package which is as yet unsupported under MSYS2.
    # For now, building HTML web pages only run
    add_custom_target(man-html ALL DEPENDS ${HTML_MAN_PAGE_OUTPUTS} ${HTML_MAN_PAGE_INDEX})
endif()

# The website
file(GLOB_RECURSE WEBSITE_IMAGES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    "html/*.png" "html/*.ico")
foreach(i ${WEBSITE_IMAGES})
    configure_file(${i} ${i} COPYONLY)
endforeach()

file(GLOB_RECURSE WEB_PAGES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "html/*.html")
foreach(w_src ${WEB_PAGES})
    set(w_target ${CMAKE_CURRENT_BINARY_DIR}/${w_src})
    add_custom_command(OUTPUT ${w_target}
        COMMAND
            sh ${CMAKE_CURRENT_SOURCE_DIR}/script/groff.sh
            -T ascii               # Output device
            -man                   # Macro Package man
            # Order of preprocessors may be important
            -t                     # Preprocess with tbl
            -s                     # Preprocess with soelim, with search paths:
            -I${CMAKE_CURRENT_SOURCE_DIR}
            -P-bcu                 # Preprocess with pic
            ${w_src}               # Source
            | awk -f ${CMAKE_CURRENT_SOURCE_DIR}/script/ssp.awk
            > ${w_target}
        DEPENDS
            ${w_src}
            ${CMAKE_CURRENT_SOURCE_DIR}/script/groff.sh
            ${CMAKE_CURRENT_SOURCE_DIR}/script/ssp.awk
        COMMENT "Building ${w_target}"
        VERBATIM)
    list(APPEND WEB_PAGE_OUTPUTS ${w_target})
endforeach()
add_custom_target(web-site ALL DEPENDS ${WEB_PAGE_OUTPUTS})

# Reference Manual Components
set(REF_PARTS ${CMAKE_CURRENT_BINARY_DIR}/etc/ref-parts.so)
add_custom_command(OUTPUT ${REF_PARTS}
    COMMAND sh ${CMAKE_CURRENT_SOURCE_DIR}/script/ref-ptx.sh ${MAN_PAGES} > ${REF_PARTS}
    DEPENDS
        ${MAN_PAGES}
        ${CMAKE_CURRENT_SOURCE_DIR}/script/ref-ptx.sh
    COMMENT "Building ${REF_PARTS}"
    VERBATIM)

set(NEW_CHANGES ${CMAKE_CURRENT_BINARY_DIR}/etc/new.so)
add_custom_command(OUTPUT ${NEW_CHANGES}
    COMMAND sh ${CMAKE_CURRENT_SOURCE_DIR}/script/new.sh -r ${NEW_CHANGE_NOTES} > ${NEW_CHANGES}
    DEPENDS
        ${NEW_CHANGE_NOTES}
        ${CMAKE_CURRENT_SOURCE_DIR}/script/new.sh
    COMMENT "Building ${NEW_CHANGES}"
    VERBATIM)

# Standalone Documents

# Transform man page(s) (ie troff format files) into a PDF
function(add_doc TARGET SOURCE DEPS)
    add_custom_command(OUTPUT ${TARGET}
        COMMAND
            sh ${CMAKE_CURRENT_SOURCE_DIR}/script/groff.sh
            -T ps                  # Output device
            -man                   # Macro Package man
            # Order of preprocessors may be important
            -t                     # Preprocess with tbl
            -s                     # Preprocess with soelim, with search paths:
            -I${DocSourcePath}
            -I${DocBinaryPath}
            ${SOURCE}              # Source
            > ${TARGET}.ps.tmp
        COMMAND
            sh ${CMAKE_CURRENT_SOURCE_DIR}/script/page_select.sh
            ${CMAKE_CURRENT_SOURCE_DIR}/script/page-list.awk
            ${TARGET}.ps.tmp
            ${TARGET}.ps
        COMMAND ${PS2PDF} ${TARGET}.ps ${TARGET}
        COMMAND rm ${TARGET}.ps ${TARGET}.ps.tmp
        DEPENDS
            ${SOURCE}
            ${DEPS}
            ${CMAKE_CURRENT_SOURCE_DIR}/script/groff.sh
            ${CMAKE_CURRENT_SOURCE_DIR}/script/page_select.sh
            ${CMAKE_CURRENT_SOURCE_DIR}/script/page-list.awk
        COMMENT "Building ${TARGET}"
        VERBATIM)
    list(APPEND DOC_TARGETS ${TARGET})
    set(DOC_TARGETS ${DOC_TARGETS} PARENT_SCOPE)
endfunction()

add_doc(${CMAKE_CURRENT_BINARY_DIR}/BUILDING.pdf ${CMAKE_CURRENT_SOURCE_DIR}/etc/BUILDING.man "")
add_doc(${CMAKE_CURRENT_BINARY_DIR}/README.pdf ${CMAKE_CURRENT_SOURCE_DIR}/etc/README.man "${NEW_CHANGES}")
add_doc(${CMAKE_CURRENT_BINARY_DIR}/change_log.pdf ${CMAKE_CURRENT_SOURCE_DIR}/etc/change_log.man "${NEW_CHANGES}")
add_doc(${CMAKE_CURRENT_BINARY_DIR}/reference-${srecord_VERSION_MAJOR}.${srecord_VERSION_MINOR}.pdf
        ${CMAKE_CURRENT_SOURCE_DIR}/etc/reference.man
        "${REF_PARTS};${NEW_CHANGES}")
add_custom_target(doco ALL DEPENDS ${DOC_TARGETS})

# Installation
# A copy of the generated PDFs In the docdir for local reference
install(FILES ${DOC_TARGETS}
        DESTINATION ${CMAKE_INSTALL_DOCDIR}/)
# Install a copy of the generated PDFs for the website
install(FILES ${DOC_TARGETS}
        DESTINATION ${CMAKE_INSTALL_DOCDIR}/htdocs)

# Install all the man pages on not-windows
if(NOT WIN32)
    set(sections 1 3 5)
    foreach(i ${sections})
        install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/man${i}/
                DESTINATION ${CMAKE_INSTALL_MANDIR}/man${i}
                FILES_MATCHING PATTERN "*.${i}")
    endforeach()
endif()

# Install the website
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html/
        DESTINATION ${CMAKE_INSTALL_DOCDIR}/htdocs
        COMPONENT website)
