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

# Build Documentation and man pages
set(GROFF sh ${CMAKE_BINARY_DIR}/groff.sh)

#FUNCTION(PREPEND var prefix)
#  SET(listVar "")
#  FOREACH(f ${ARGN})
#    LIST(APPEND listVar "${prefix}/${f}")
#  ENDFOREACH(f)
#  SET(${var} "${listVar}" PARENT_SCOPE)
#ENDFUNCTION(PREPEND)

set(CHANGE_LOG "etc/README.man etc/new.1.1.so etc/new.1.10.so etc/new.1.11.so \
		etc/new.1.12.so etc/new.1.13.so etc/new.1.14.so \
		etc/new.1.15.so etc/new.1.16.so etc/new.1.17.so \
		etc/new.1.18.so etc/new.1.19.so etc/new.1.2.so etc/new.1.20.so \
		etc/new.1.21.so etc/new.1.22.so etc/new.1.23.so \
		etc/new.1.24.so etc/new.1.25.so etc/new.1.26.so \
		etc/new.1.27.so etc/new.1.28.so etc/new.1.29.so etc/new.1.3.so \
		etc/new.1.30.so etc/new.1.31.so etc/new.1.32.so \
		etc/new.1.33.so etc/new.1.34.so etc/new.1.35.so \
		etc/new.1.36.so etc/new.1.37.so etc/new.1.38.so \
		etc/new.1.39.so etc/new.1.4.so etc/new.1.40.so etc/new.1.41.so \
		etc/new.1.42.so etc/new.1.43.so etc/new.1.44.so \
		etc/new.1.45.so etc/new.1.46.so etc/new.1.47.so \
		etc/new.1.48.so etc/new.1.49.so etc/new.1.5.so etc/new.1.50.so \
		etc/new.1.51.so etc/new.1.52.so etc/new.1.53.so \
		etc/new.1.54.so etc/new.1.55.so etc/new.1.56.so \
		etc/new.1.57.so etc/new.1.58.so etc/new.1.59.so etc/new.1.6.so \
		etc/new.1.60.so etc/new.1.61.so etc/new.1.62.so \
		etc/new.1.63.so etc/new.1.64.so etc/new.1.7.so etc/new.1.8.so \
		etc/new.1.9.so")

set(REFERENCE_FILES ${CHANGE_LOG}  "etc/ref-toc.so etc/reference.man man/man1/o_at.so \
		man/man1/o_ignore_checksums.so man/man1/o_input.so \
		man/man1/o_multiple.so man/man1/o_sequence.so \
		man/man1/srec_cat.1 man/man1/srec_cmp.1 \
		man/man1/srec_examples.1 man/man1/srec_info.1 \
		man/man1/srec_input.1 man/man1/srec_license.1 \
		man/man1/z_copyright.so man/man1/z_exit.so \
		man/man1/z_options.so man/man3/srecord.3 \
		man/man3/srecord_license.3 man/man5/srec_aomf.5 \
		man/man5/srec_ascii_hex.5 man/man5/srec_atmel_generic.5 \
		man/man5/srec_binary.5 man/man5/srec_brecord.5 \
		man/man5/srec_coe.5 man/man5/srec_cosmac.5 \
		man/man5/srec_dec_binary.5 man/man5/srec_emon52.5 \
		man/man5/srec_fairchild.5 man/man5/srec_fastload.5 \
		man/man5/srec_formatted_binary.5 man/man5/srec_forth.5 \
		man/man5/srec_fpc.5 man/man5/srec_idt.5 man/man5/srec_intel.5 \
		man/man5/srec_intel16.5 man/man5/srec_logisim.5 \
		man/man5/srec_mem.5 man/man5/srec_mif.5 \
		man/man5/srec_mips_flash.5 man/man5/srec_mos_tech.5 \
		man/man5/srec_motorola.5 man/man5/srec_msbin.5 \
		man/man5/srec_needham.5 man/man5/srec_os65v.5 \
		man/man5/srec_ppb.5 man/man5/srec_ppx.5 \
		man/man5/srec_signetics.5 man/man5/srec_spasm.5 \
		man/man5/srec_spectrum.5 man/man5/srec_stewie.5 \
		man/man5/srec_tektronix.5 man/man5/srec_tektronix_extended.5 \
		man/man5/srec_ti_tagged.5 man/man5/srec_ti_tagged_16.5 \
		man/man5/srec_ti_txt.5 man/man5/srec_trs80.5 \
		man/man5/srec_vmem.5 man/man5/srec_wilson.5")

FUNCTION(ADD_DOC T S DEPS)
  set(TARGET ${CMAKE_BINARY_DIR}/doc/${T})
  set(SRC ${CMAKE_SOURCE_DIR}/etc/${S})
  add_custom_command(OUTPUT ${TARGET}
    COMMAND ${GROFF} -Tps -s -I${CMAKE_SOURCE_DIR} -t -man ${SRC} > ${TARGET}.ps
    COMMAND ps2pdf ${TARGET}.ps ${TARGET}
    COMMAND rm ${TARGET}.ps
    DEPENDS ${SRC}
    # WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Building ${TARGET}"
    VERBATIM)
  LIST(APPEND listVar "${prefix}/${f}")
ENDFUNCTION(ADD_DOC)

#awk -f etc/ref-ptx1.awk man/man1/*.1 man/man3/*.3 man/man5/*.5 | ptx -O -r -w 1000 -ietc/ref-ptx.ignore | sort  -f -d -t'"' +5 -6 +3 -4 | awk -F'"' -f etc/ref-ptx2.awk > etc/ref-index.so

# Target directories
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/doc)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/man)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/man/man1)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/man/man3)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/man/man5)

set(BUILDING_PDF ${CMAKE_BINARY_DIR}/doc/BUILDING.pdf)
set(BUILDING_SRC ${CMAKE_SOURCE_DIR}/etc/BUILDING.man)
add_custom_command(OUTPUT ${BUILDING_PDF}
  COMMAND ${GROFF} -Tps -s -I${CMAKE_SOURCE_DIR} -t -man ${BUILDING_SRC} > ${BUILDING_PDF}.ps
  COMMAND ps2pdf ${BUILDING_PDF}.ps ${BUILDING_PDF}
  COMMAND rm ${BUILDING_PDF}.ps
  DEPENDS ${BUILDING_SRC}
#  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  COMMENT "Building ${BUILDING_PDF}"
  VERBATIM)

set(README_PDF ${CMAKE_BINARY_DIR}/doc/README.pdf)
set(README_SRC ${CMAKE_SOURCE_DIR}/etc/README.man)
add_custom_command(OUTPUT ${README_PDF}
  COMMAND ${GROFF} -Tps -s -I${CMAKE_SOURCE_DIR} -t -man ${README_SRC} > ${README_PDF}.ps
  COMMAND ps2pdf ${README_PDF}.ps ${README_PDF}
  COMMAND rm ${README_PDF}.ps
  DEPENDS ${README_SRC} ${CHANGE_LOG_SRC}
  COMMENT "Building ${README_PDF}"
  VERBATIM)

set(CHANGE_LOG_PDF ${CMAKE_BINARY_DIR}/doc/change_log.pdf)
set(CHANGE_LOG_SRC ${CMAKE_SOURCE_DIR}/etc/change_log.man)
add_custom_command(OUTPUT ${CHANGE_LOG_PDF}
  COMMAND ${GROFF} -Tps -s -I${CMAKE_SOURCE_DIR} -t -man ${CHANGE_LOG_SRC} > ${CHANGE_LOG_PDF}.ps
  COMMAND ps2pdf ${CHANGE_LOG_PDF}.ps ${CHANGE_LOG_PDF}
  COMMAND rm ${CHANGE_LOG_PDF}.ps
  DEPENDS ${CHANGE_LOG_SRC}
  COMMENT "Building ${CHANGE_LOG_PDF}"
  VERBATIM)

set(REFERENCE_PDF ${CMAKE_BINARY_DIR}/doc/reference.pdf)
set(REFERENCE_SRC ${CMAKE_SOURCE_DIR}/etc/reference.man)
add_custom_command(OUTPUT ${REFERENCE_PDF}
  COMMAND ${GROFF} -Tps -s -I${CMAKE_SOURCE_DIR} -t -man ${REFERENCE_SRC} > ${REFERENCE_PDF}.ps
  COMMAND ps2pdf ${REFERENCE_PDF}.ps ${REFERENCE_PDF}
  COMMAND rm ${REFERENCE_PDF}.ps
  DEPENDS ${REFERENCE_SRC} ${REFERENCE_SRC}
  COMMENT "Building ${REFERENCE_PDF}"
  VERBATIM)

add_custom_target(doco ALL DEPENDS ${BUILDING_PDF} ${README_PDF} ${CHANGE_LOG_PDF} ${REFERENCE_PDF})

# man pages
file(GLOB_RECURSE MAN_PAGES RELATIVE ${CMAKE_SOURCE_DIR} "man/man*/*.[135]")
file(GLOB_RECURSE MAN_COMMON RELATIVE ${CMAKE_SOURCE_DIR} "man/man*/[zo]_*.so")

FOREACH(m_src ${MAN_PAGES})
  set(m_target ${CMAKE_BINARY_DIR}/${m_src})
  add_custom_command(OUTPUT ${m_src}
    COMMAND ${GROFF} -I${CMAKE_SOURCE_DIR} -Tascii -t -man ${m_src} > ${m_target}
    DEPENDS ${m_src} ${MAN_COMMON}
    COMMENT "Building ${m_target}"
    VERBATIM)
  LIST(APPEND MAN_PAGE_OUTPUTS ${m_src})
ENDFOREACH(m_src)

add_custom_target(man ALL DEPENDS ${MAN_PAGE_OUTPUTS})
