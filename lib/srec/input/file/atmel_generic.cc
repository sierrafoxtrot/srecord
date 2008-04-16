//
//      srecord - manipulate eprom load files
//      Copyright (C) 2001-2003, 2006-2008 Peter Miller
//
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 3 of the License, or
//      (at your option) any later version.
//
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with this program. If not, see
//      <http://www.gnu.org/licenses/>.
//


#include <lib/srec/input/file/atmel_generic.h>
#include <ctype.h>
#include <lib/srec/record.h>


srec_input_file_atmel_generic::~srec_input_file_atmel_generic()
{
}


srec_input_file_atmel_generic::srec_input_file_atmel_generic(
        const string &a_file_name, bool arg2) :
    srec_input_file(a_file_name),
    seen_some_input(false),
    bigend(arg2)
{
}


srec_input::pointer
srec_input_file_atmel_generic::create_be(const string &a_file_name)
{
    return pointer(new srec_input_file_atmel_generic(a_file_name));
}


srec_input::pointer
srec_input_file_atmel_generic::create(const string &a_file_name, bool bigend)
{
    return pointer(new srec_input_file_atmel_generic(a_file_name, bigend));
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


bool
srec_input_file_atmel_generic::read(srec_record &record)
{
    if (!read_inner(record))
    {
        if (!seen_some_input)
            fatal_error("file contains no data");
        return false;
    }
    seen_some_input = true;
    return true;
}


const char *
srec_input_file_atmel_generic::get_file_format_name()
    const
{
    return (bigend ? "Atmel Generic (big-endian)"
                   : "Atmel Generic (little-endian)");
}
