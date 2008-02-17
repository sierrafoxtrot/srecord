//
//      srecord - manipulate eprom load files
//      Copyright (C) 1998, 1999, 2001, 2002, 2006-2008 Peter Miller
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

#include <lib/interval.h>
#include <lib/srec/input/filter/length.h>
#include <lib/srec/record.h>


srec_input_filter_length::~srec_input_filter_length()
{
}


srec_input_filter_length::srec_input_filter_length(
        const srec_input::pointer &a1, int a2, int a3, int a4) :
    srec_input_filter(a1),
    length_address(a2),
    length_length(a3),
    length_order(a4),
    minimum(0),
    maximum(0),
    limits_set(false)
{
    if (length_length < 0)
        length_length = 0;
    else if (length_length > 8)
        length_length = 8;
    minimum = length_address;
    maximum = length_address + length_length;
    limits_set = true;
}


srec_input::pointer
srec_input_filter_length::create(const srec_input::pointer &a_deeper, int a2,
    int a3, int a4)
{
    return pointer(new srec_input_filter_length(a_deeper, a2, a3, a4));
}


int
srec_input_filter_length::generate(srec_record &record)
{
    if (length_length <= 0)
            return 0;
    if (length_length > 8)
        length_length = 8;
    unsigned char chunk[8];
    int length = maximum - minimum;
    if (length_order)
        srec_record::encode_little_endian(chunk, length, length_length);
    else
        srec_record::encode_big_endian(chunk, length, length_length);
    record =
        srec_record
        (
            srec_record::type_data,
            length_address,
            chunk,
            length_length
        );
    length_length = 0;
    return 1;
}


int
srec_input_filter_length::read(srec_record &record)
{
    if (!srec_input_filter::read(record))
        return generate(record);
    if (record.get_type() == srec_record::type_data)
    {
        if (!limits_set)
        {
            minimum = record.get_address();
            maximum = record.get_address_end();
            limits_set = true;
        }
        else
        {
            if (minimum > record.get_address())
                minimum = record.get_address();
            if (maximum < record.get_address_end())
                maximum = record.get_address_end();
        }
    }
    return 1;
}
