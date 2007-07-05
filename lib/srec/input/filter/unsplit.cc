//
//      srecord - manipulate eprom load files
//      Copyright (C) 1998, 1999, 2001, 2002, 2006, 2007 Peter Miller
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
// MANIFEST: functions to impliment the srec_input_filter_unsplit class
//


#include <lib/srec/input/filter/unsplit.h>
#include <lib/srec/record.h>


srec_input_filter_unsplit::srec_input_filter_unsplit(srec_input *a1, int a2,
                int a3, int a4) :
        srec_input_filter(a1),
        modulus(a2),
        offset(a3),
        width(a4),
        buffer(),
        buffer_pos(0)
{
}


srec_input_filter_unsplit::~srec_input_filter_unsplit()
{
}


int
srec_input_filter_unsplit::read(srec_record &record)
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

                unsigned long addr = buffer.get_address() + buffer_pos;
                unsigned char c = buffer.get_data(buffer_pos++);
                int phase = addr % width;
                addr = (addr / width) * modulus + phase + offset;
                record = srec_record(srec_record::type_data, addr, &c, 1);
                return 1;
        }
}
