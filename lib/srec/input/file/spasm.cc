//
//      srecord - manipulate eprom load files
//      Copyright (C) 2001-2003, 2006, 2007 Peter Miller
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


#include <lib/srec/input/file/spasm.h>
#include <ctype.h>
#include <lib/srec/record.h>


srec_input_file_spasm::srec_input_file_spasm(const string &a_file_name,
        bool arg2) :
    srec_input_file(a_file_name),
    seen_some_input(false),
    bigend(arg2)
{
}


srec_input_file_spasm::~srec_input_file_spasm()
{
}


int
srec_input_file_spasm::read_inner(srec_record &record)
{
    if (peek_char() < 0)
        return 0;

    int address = get_word();
    if (get_char() != ' ')
        fatal_error("space expected");
    unsigned char data[2];
    data[bigend] = get_byte();
    data[!bigend] = get_byte();
    if (get_char() != '\n')
        fatal_error("end of line expected");

    record =
        srec_record
        (
            srec_record::type_data,
            address * 2,                        // is this right??
            data,
            2
        );
    return 1;
}


int
srec_input_file_spasm::read(srec_record &record)
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
srec_input_file_spasm::get_file_format_name()
    const
{
    return (bigend ? "SPASM (big-endian)" : "SPASM (little-endian)");
}
