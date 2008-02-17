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

#include <lib/srec/input/filter/and.h>
#include <lib/srec/record.h>


srec_input_filter_and::~srec_input_filter_and()
{
}


srec_input_filter_and::srec_input_filter_and(const srec_input::pointer &a1,
        int a2) :
    srec_input_filter(a1),
    value(a2)
{
}


srec_input::pointer
srec_input_filter_and::create(const srec_input::pointer &a_deeper, int a_mask)
{
    return pointer(new srec_input_filter_and(a_deeper, a_mask));
}


int
srec_input_filter_and::read(srec_record &record)
{
    if (!srec_input_filter::read(record))
        return 0;
    if (record.get_type() == srec_record::type_data)
    {
        for (int j = 0; j < record.get_length(); ++j)
            record.set_data(j, record.get_data(j) & value);
    }
    return 1;
}
