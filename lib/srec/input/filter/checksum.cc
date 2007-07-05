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
// MANIFEST: functions to impliment the srec_input_filter_checksum class
//


#include <lib/interval.h>
#include <lib/srec/input/filter/checksum.h>
#include <lib/srec/record.h>


srec_input_filter_checksum::srec_input_filter_checksum(srec_input *a1, int a2,
                int a3, int a4, int a5) :
        srec_input_filter(a1),
        checksum_address(a2),
        length(a3),
        checksum_order(!!a4),
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


int
srec_input_filter_checksum::generate(srec_record &record)
{
        if (length <= 0)
                return 0;
        unsigned char chunk[sizeof(sum_t)];
        sum_t value = calculate();
        if (checksum_order)
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
        return 1;
}


int
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
                else if (checksum_order)
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
        return 1;
}
