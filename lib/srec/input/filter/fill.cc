//
//      srecord - manipulate eprom load files
//      Copyright (C) 1998, 1999, 2001, 2002, 2004, 2006, 2007 Peter Miller
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
// MANIFEST: functions to impliment the srec_input_filter_fill class
//


#include <lib/interval.h>
#include <lib/srec/input/filter/fill.h>
#include <lib/srec/record.h>


srec_input_filter_fill::srec_input_filter_fill(srec_input *a1, int a2,
        const interval &a3) :
    srec_input_filter(a1),
    filler_value(a2),
    filler_block(0),
    range(a3)
{
}


srec_input_filter_fill::~srec_input_filter_fill()
{
    if (filler_block)
        delete [] filler_block;
}


int
srec_input_filter_fill::generate(srec_record &record)
{
    if (range.empty())
        return 0;
    interval chunk(range.get_lowest(), range.get_lowest() + 256);
    chunk *= range;
    chunk.first_interval_only();
    if (!filler_block)
    {
        filler_block = new unsigned char [256];
        memset(filler_block, filler_value, 256);
    }
    record =
        srec_record
        (
            srec_record::type_data,
            chunk.get_lowest(),
            filler_block,
            chunk.get_highest() - chunk.get_lowest()
        );
    range -= chunk;
    return 1;
}


int
srec_input_filter_fill::read(srec_record &record)
{
    if (!srec_input_filter::read(record))
        return generate(record);
    if (record.get_type() == srec_record::type_data)
    {
        range -=
            interval
            (
                record.get_address(),
                record.get_address() + record.get_length()
            );
    }
    return 1;
}
