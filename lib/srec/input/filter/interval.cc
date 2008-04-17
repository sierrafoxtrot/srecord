//
//      srecord - Manipulate EPROM load files
//      Copyright (C) 2008 Peter Miller
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

#include <lib/srec/input/filter/interval.h>
#include <lib/srec/record.h>


srec_input_filter_interval::~srec_input_filter_interval()
{
}


srec_input_filter_interval::srec_input_filter_interval(
        const srec_input::pointer &a_deeper, long a_address, int a_length,
        endian_t a_end, bool inclusive) :
    srec_input_filter(a_deeper),
    address(a_address),
    length(a_length <= 1 ? 1 : a_length >= 8 ? 8 : a_length),
    end(a_end)
{
    if (inclusive)
        range = interval(address, address + length);
}


bool
srec_input_filter_interval::generate(srec_record &record)
{
    if (length <= 0)
        return false;
    long value = calculate_result();
    unsigned char chunk[8];
    srec_record::encode(chunk, value, length, end);
    record = srec_record(srec_record::type_data, address, chunk, length);
    length = 0;
    return true;
}


bool
srec_input_filter_interval::read(srec_record &record)
{
    if (!srec_input_filter::read(record))
        return generate(record);
    if (record.get_type() == srec_record::type_data)
    {
        interval i(record.get_address(), record.get_address_end());
        range += i;
    }
    return true;
}

// vim:ts=8:sw=4:et
