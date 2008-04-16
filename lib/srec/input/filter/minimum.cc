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

#include <lib/srec/input/filter/minimum.h>
#include <lib/srec/record.h>


srec_input_filter_minimum::~srec_input_filter_minimum()
{
}


srec_input_filter_minimum::srec_input_filter_minimum(
        const srec_input::pointer &a1, int a2, int a3, endian_t a_end) :
    srec_input_filter(a1),
    minimum_address(a2),
    minimum_length(a3),
    end(a_end),
    minimum(0),
    minimum_set(false)
{
    if (minimum_length < 0)
        minimum_length = 0;
    else if (minimum_length > 8)
        minimum_length = 8;
    minimum = minimum_address;
    minimum_set = true;
}


srec_input::pointer
srec_input_filter_minimum::create(const srec_input::pointer &a_deeper, int a2,
    int a3, endian_t a_end)
{
    return pointer(new srec_input_filter_minimum(a_deeper, a2, a3, a_end));
}


bool
srec_input_filter_minimum::generate(srec_record &record)
{
    if (minimum_length <= 0)
        return false;
    if (minimum_length > 8)
        minimum_length = 8;
    unsigned char chunk[8];
    if (end == endian_little)
        srec_record::encode_little_endian(chunk, minimum, minimum_length);
    else
        srec_record::encode_big_endian(chunk, minimum, minimum_length);
    record =
        srec_record
        (
            srec_record::type_data,
            minimum_address,
            chunk,
            minimum_length
        );
    minimum_length = 0;
    return true;
}


bool
srec_input_filter_minimum::read(srec_record &record)
{
    if (!srec_input_filter::read(record))
        return generate(record);
    if (record.get_type() == srec_record::type_data)
    {
        if (!minimum_set || record.get_address() < minimum)
        {
            minimum = record.get_address();
            minimum_set = true;
        }
    }
    return true;
}
