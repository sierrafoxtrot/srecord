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
#include <lib/srec/input/filter/checksum.h>
#include <lib/srec/record.h>


srec_input_filter_checksum::srec_input_filter_checksum(srec_input::pointer a1,
        int a2, int a3, endian_t a_end, int a5) :
    srec_input_filter(a1),
    checksum_address(a2),
    length(a3),
    end(a_end),
    sum(0),
    width(a5)
{
    if (length < 0)
        length = 0;
    else if (length > (int)sizeof(sum_t))
        length = sizeof(sum_t);
    if (width < 1)
        width = 1;
    else if (width > length)
        width = length;
}


srec_input_filter_checksum::~srec_input_filter_checksum()
{
}


bool
srec_input_filter_checksum::generate(srec_record &record)
{
    if (length <= 0)
        return false;
    unsigned char chunk[sizeof(sum_t)];
    sum_t value = calculate();
    if (end == endian_little)
        srec_record::encode_little_endian(chunk, value, length);
    else
        srec_record::encode_big_endian(chunk, value, length);
    record =
        srec_record
        (
            srec_record::type_data,
            checksum_address,
            chunk,
            length
        );
    length = 0;
    return true;
}


bool
srec_input_filter_checksum::read(srec_record &record)
{
    if (!srec_input_filter::read(record))
        return generate(record);
    if (record.get_type() == srec_record::type_data)
    {
        if (width <= 1)
        {
            for (int j = 0; j < record.get_length(); ++j)
            {
                sum += record.get_data(j);
            }
        }
        else if (end == endian_little)
        {
            // Little endian
            for (int j = 0; j < record.get_length(); ++j)
            {
                sum += (sum_t)record.get_data(j) << (8 *
                    ((record.get_address() + j) % width));
            }
        }
        else
        {
            // Big endian
            for (int j = 0; j < record.get_length(); ++j)
            {
                sum += (sum_t)record.get_data(j) << (8 *
                    (width - 1 - ((record.get_address() + j)
                    % width)));
            }
        }
    }
    return true;
}
