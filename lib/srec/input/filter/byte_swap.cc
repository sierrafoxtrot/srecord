//
//      srecord - manipulate eprom load files
//      Copyright (C) 2000-2002, 2005-2007 Peter Miller
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
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
//      02110-1301 USA.
//
// MANIFEST: functions to impliment the srec_input_filter_byte_swap class
//


#include <lib/srec/arglex.h>
#include <lib/srec/input/filter/byte_swap.h>
#include <lib/srec/record.h>


srec_input_filter_byte_swap::srec_input_filter_byte_swap(srec_input *arg) :
    srec_input_filter(arg),
    buffer(),
    buffer_pos(0),
    mask(1)
{
}


srec_input_filter_byte_swap::~srec_input_filter_byte_swap()
{
}


int
srec_input_filter_byte_swap::read(srec_record &record)
{
    for (;;)
    {
        while
        (
            buffer.get_type() != srec_record::type_data
        ||
            buffer_pos >= buffer.get_length()
        )
        {
            if (!srec_input_filter::read(buffer))
                return 0;
            if (buffer.get_type() != srec_record::type_data)
            {
                record = buffer;
                record.set_address(record.get_address() ^ mask);
                return 1;
            }
            buffer_pos = 0;
        }

        unsigned long addr = (buffer.get_address() + buffer_pos) ^ mask;
        unsigned char c = buffer.get_data(buffer_pos++);
        record = srec_record(srec_record::type_data, addr, &c, 1);
        return 1;
    }
}


void
srec_input_filter_byte_swap::command_line(srec_arglex *cmdln)
{
    if (cmdln->can_get_number())
    {
        int width = cmdln->get_number("width");
        switch (width)
        {
        case 2:
        case 4:
        case 8:
            break;

        case 16:
            width = 2;
            break;

        case 32:
            width = 4;
            break;

        case 64:
            width = 8;
            break;

        case 128:
            width = 16;
            break;

        case 256:
            width = 32;
            break;

        default:
            fatal_error("width %d not understood", width);
            // NOTREACHED
        }
        mask = width - 1;
    }
}
