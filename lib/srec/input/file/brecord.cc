//
//      srecord - The "srecord" program.
//      Copyright (C) 2007 Peter Miller
//
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111, USA.
//
// MANIFEST: functions to impliment the srec_input_file_brecord class
//

#include <lib/srec/input/file/brecord.h>
#include <lib/srec/record.h>


srec_input_file_brecord::srec_input_file_brecord(const string &a_file_name) :
    srec_input_file(a_file_name),
    seen_some_input(false)
{
}


srec_input_file_brecord::~srec_input_file_brecord()
{
}


int
srec_input_file_brecord::read_inner(srec_record &record)
{
    if (peek_char() < 0)
        return 0;

    unsigned long address = get_4bytes();
    unsigned char length = get_byte();
    if (length & 0x20)
        fatal_error("read mode not supported");
    length &= 0x1F;
    unsigned char data[32];
    for (unsigned j = 0; j < length; ++j)
        data[j] = get_byte();
    if (get_char() != '\n')
        fatal_error("end of line expected");

    if (length == 0)
        record = srec_record(srec_record::type_start_address, address, 0, 0);
    else
        record = srec_record(srec_record::type_data, address, data, length);
    return 1;
}


int
srec_input_file_brecord::read(srec_record &record)
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
srec_input_file_brecord::get_file_format_name()
    const
{
    return "Motorola MC68EZ328 bootstrap b-record";
}
