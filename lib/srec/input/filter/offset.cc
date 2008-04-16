//
//      srecord - manipulate eprom load files
//      Copyright (C) 1998, 1999, 2001, 2002, 2005-2008 Peter Miller
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

#include <lib/srec/input/filter/offset.h>
#include <lib/srec/record.h>


srec_input_filter_offset::~srec_input_filter_offset()
{
}


srec_input_filter_offset::srec_input_filter_offset(
        const srec_input::pointer &a1, long a2) :
    srec_input_filter(a1),
    nbytes(a2)
{
}


srec_input::pointer
srec_input_filter_offset::create(const srec_input::pointer &a_deeper,
    long nbytes)
{
    return pointer(new srec_input_filter_offset(a_deeper, nbytes));
}


bool
srec_input_filter_offset::read(srec_record &record)
{
    if (!srec_input_filter::read(record))
        return false;
    long addr = record.get_address() + nbytes;

    //
    // Cope with machines with 64-bit longs, since all the code assumes
    // addresses are in the modulo-2**32 range.
    //
    // FIXME: need to cope with data records which span zero.
    //
    addr &= 0xFFFFFFFF;

    record.set_address(addr);
    return true;
}
