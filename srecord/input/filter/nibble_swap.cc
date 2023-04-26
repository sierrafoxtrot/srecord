//
// Copyright (c) 2023 fenugrec
//
// srecord filter: exchanges upper and lower nibble of each byte,
// e.g. "0xA6" => "0x6A"
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at
// your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//

#include <srecord/input/filter/nibble_swap.h>
#include <srecord/record.h>


srecord::input_filter_nibble_swap::input_filter_nibble_swap(
    const srecord::input::pointer &arg
) :
    srecord::input_filter(arg)
{
}


srecord::input::pointer
srecord::input_filter_nibble_swap::create(const input::pointer &a_deeper)

{
    return pointer(new srecord::input_filter_nibble_swap(a_deeper));
}


bool
srecord::input_filter_nibble_swap::read(srecord::record &record)
{
    if (!srecord::input_filter::read(record))
        return false;
    if (record.get_type() == srecord::record::type_data)
    {
        for (size_t j = 0; j < record.get_length(); ++j) {
            uint8_t tmp = record.get_data(j);
            record.set_data(j, ((tmp & 0x0F) << 4) | ((tmp & 0xF0) >> 4));
        }
    }
    return true;
}
