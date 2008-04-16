//
//      srecord - manipulate eprom load files
//      Copyright (C) 2000-2002, 2005-2008 Peter Miller
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

#include <lib/srec/arglex.h>
#include <lib/srec/input/filter/byte_swap.h>
#include <lib/srec/record.h>


srec_input_filter_byte_swap::~srec_input_filter_byte_swap()
{
}


srec_input_filter_byte_swap::srec_input_filter_byte_swap(
        const srec_input::pointer &arg) :
    srec_input_filter(arg),
    buffer(),
    buffer_pos(0),
    mask(1)
{
}


srec_input::pointer
srec_input_filter_byte_swap::create(const srec_input::pointer &a_deeper)
{
    return pointer(new srec_input_filter_byte_swap(a_deeper));
}


bool
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
                return false;
            if (buffer.get_type() != srec_record::type_data)
            {
                record = buffer;
                record.set_address(record.get_address() ^ mask);
                return true;
            }
            buffer_pos = 0;
        }

        unsigned long addr = (buffer.get_address() + buffer_pos) ^ mask;
        unsigned char c = buffer.get_data(buffer_pos++);
        record = srec_record(srec_record::type_data, addr, &c, 1);
        return true;
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
