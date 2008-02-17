//
//      srecord - manipulate eprom load files
//      Copyright (C) 2006-2008 Peter Miller
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

#include <lib/srec/input/filter/sequence.h>
#include <lib/srec/record.h>


srec_input_filter_sequence::~srec_input_filter_sequence()
{
}


srec_input_filter_sequence::srec_input_filter_sequence(
        srec_input::pointer arg) :
    srec_input_filter(arg),
    last_address(0),
    warned(false)
{
}


srec_input::pointer
srec_input_filter_sequence::create(srec_input::pointer arg)
{
    return pointer(new srec_input_filter_sequence(arg));
}


int
srec_input_filter_sequence::read(srec_record &record)
{
    if (!srec_input_filter::read(record))
        return 0;
    if (record.get_type() == srec_record::type_data)
    {
#if 0
        fprintf(stderr, "%s: %d: address = %08lX, length = %08X\n",
            __FILE__, __LINE__,
            record.get_address(),
            record.get_length()
           );
#endif
        if (!warned)
        {
            //
            // Issue a warning if the data records are not in strictly
            // ascending address order.
            //
            if (record.get_address() < last_address)
            {
                warning
                (
                    "data records not in strictly ascending order "
                        "(expected >= 0x%04lX, got 0x%04lX)",
                    last_address,
                    long(record.get_address())
                );
                warned = true;
            }
        }
        last_address = record.get_address() + record.get_length();
    }
    return 1;
}
