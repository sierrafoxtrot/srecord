//
// srecord - Manipulate EPROM load files
// Copyright (C) 2009, 2010 Peter Miller
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at
// your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//

#include <srecord/bitrev.h>
#include <srecord/input/filter/bitrev.h>
#include <srecord/record.h>


srec_input_filter_bitrev::~srec_input_filter_bitrev()
{
}


srec_input_filter_bitrev::srec_input_filter_bitrev(
    const srec_input::pointer &arg
) :
    srec_input_filter(arg)
{
}


srec_input::pointer
srec_input_filter_bitrev::create(const srec_input::pointer &a_deeper)
{
    return pointer(new srec_input_filter_bitrev(a_deeper));
}


bool
srec_input_filter_bitrev::read(srec_record &record)
{
    if (!srec_input_filter::read(record))
        return false;
    if (record.get_type() == srec_record::type_data)
    {
        for (size_t j = 0; j < record.get_length(); ++j)
            record.set_data(j, bitrev8(record.get_data(j)));
    }
    return true;
}


// vim:ts=8:sw=4:et
