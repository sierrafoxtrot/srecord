//
//	srecord - manipulate eprom load files
//	Copyright (C) 2001-2003 Peter Miller;
//	All rights reserved.
//
//	This program is free software; you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published by
//	the Free Software Foundation; either version 2 of the License, or
//	(at your option) any later version.
//
//	This program is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.
//
//	You should have received a copy of the GNU General Public License
//	along with this program; if not, write to the Free Software
//	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111, USA.
//
// MANIFEST: functions to impliment the srec_input_file_atmel_generic class
//

#pragma implementation "srec_input_file_atmel_generic"

#include <srec/input/file/atmel_generic.h>
#include <ctype.h>
#include <srec/record.h>


srec_input_file_atmel_generic::srec_input_file_atmel_generic(
	const char *filename, bool arg2) :
    srec_input_file(filename),
    seen_some_input(false),
    bigend(arg2)
{
}


srec_input_file_atmel_generic::~srec_input_file_atmel_generic()
{
}


int
srec_input_file_atmel_generic::read_inner(srec_record &record)
{
    if (peek_char() < 0)
	return 0;

    int address = get_3bytes();
    if (get_char() != ':')
	fatal_error("colon expected");
    unsigned char data[2];
    data[bigend] = get_byte();
    data[!bigend] = get_byte();
    if (get_char() != '\n')
	fatal_error("end of line expected");

    record = srec_record(srec_record::type_data, address * 2, data, 2);
    return 1;
}


int
srec_input_file_atmel_generic::read(srec_record &record)
{
    if (!read_inner(record))
    {
	if (!seen_some_input)
    	    fatal_error("file contains no data");
	return 0;
    }
    seen_some_input = true;
    return 1;
}


const char *
srec_input_file_atmel_generic::get_file_format_name()
    const
{
    return (bigend ? "Atmel Generic (big-endian)"
                   : "Atmel Generic (little-endian)");
}
