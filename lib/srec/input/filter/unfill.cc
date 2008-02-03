//
//      srecord - manipulate eprom load files
//      Copyright (C) 2001, 2002, 2006-2008 Peter Miller
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


#include <lib/srec/input/filter/unfill.h>


srec_input_filter_unfill::srec_input_filter_unfill(srec_input *a1, int a2,
        int a3) :
    srec_input_filter(a1),
    fill_value(a2),
    fill_minimum(a3),
    buffer(),
    buffer_pos(0)
{
}


srec_input_filter_unfill::~srec_input_filter_unfill()
{
}


int
srec_input_filter_unfill::read(srec_record &record)
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
                return 1;
            }
            buffer_pos = 0;
        }

        int first_pos = buffer_pos;
        unsigned long addr = buffer.get_address() + buffer_pos;
        unsigned char c = buffer.get_data(buffer_pos++);
        if (c == fill_value)
        {
            //
            // Let runs of fill characters go through,
            // if the run is shorter than the minimum.
            // This is so that single bytes can be left in,
            // but long oatches are thrown away.
            //
            while (buffer_pos < buffer.get_length())
            {
                c = buffer.get_data(buffer_pos);
                if (c != fill_value)
                    break;
                ++buffer_pos;
            }
            if (buffer_pos - first_pos < fill_minimum)
            {
                record =
                    srec_record
                    (
                        srec_record::type_data,
                        addr,
                        buffer.get_data() + first_pos,
                        buffer_pos - first_pos
                    );
                return 1;
            }
        }
        else
        {
            //
            // Let runs of non-fill characters go straight through.
            //
            while (buffer_pos < buffer.get_length())
            {
                c = buffer.get_data(buffer_pos);
                if (c == fill_value)
                    break;
                ++buffer_pos;
            }
            record =
                srec_record
                (
                    srec_record::type_data,
                    addr,
                    buffer.get_data() + first_pos,
                    buffer_pos - first_pos
                );
            return 1;
        }
    }
}
