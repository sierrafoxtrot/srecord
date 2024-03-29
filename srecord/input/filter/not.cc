//
// srecord - manipulate eprom load files
// Copyright (C) 2001, 2002, 2006-2010 Peter Miller
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see
// <http://www.gnu.org/licenses/>.
//


#include <srecord/input/filter/not.h>
#include <srecord/record.h>


srecord::input_filter_not::input_filter_not(const input::pointer &arg) :
    srecord::input_filter(arg)
{
}


srecord::input::pointer
srecord::input_filter_not::create(const srecord::input::pointer &a_deeper)
{
    return pointer(new srecord::input_filter_not(a_deeper));
}


bool
srecord::input_filter_not::read(srecord::record &record)
{
    if (!srecord::input_filter::read(record))
        return false;
    if (record.get_type() == srecord::record::type_data)
    {
        for (size_t j = 0; j < record.get_length(); ++j)
            record.set_data(j, ~record.get_data(j));
    }
    return true;
}
