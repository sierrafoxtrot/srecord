//
// srecord - Manipulate EPROM load files
// Copyright (C) 2009 Peter Miller
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

#include <lib/srec/arglex.h>
#include <lib/srec/input/filter/fletcher16.h>
#include <lib/srec/memory.h>
#include <lib/srec/memory/walker/fletcher16.h>
#include <lib/srec/record.h>


srec_input_filter_fletcher16::~srec_input_filter_fletcher16()
{
}


srec_input_filter_fletcher16::srec_input_filter_fletcher16(
        const srec_input::pointer &deeper_arg, unsigned long address_arg,
        endian_t a_end) :
    srec_input_filter(deeper_arg),
    address(address_arg),
    end(a_end),
    buffer_pos(0),
    have_forwarded_header(false),
    have_given_checksum(false),
    have_forwarded_start_address(false)
{
}


srec_input::pointer
srec_input_filter_fletcher16::create(const srec_input::pointer &a_deeper,
    unsigned long a_address, endian_t a_end)
{
    return
        pointer(new srec_input_filter_fletcher16(a_deeper, a_address, a_end));
}


bool
srec_input_filter_fletcher16::read(srec_record &record)
{
    //
    // If we haven't read the deeper input yet, read all of it into
    // a memory buffer, then fletcher16 the bytes.
    //
    if (buffer.empty())
    {
        buffer.reader(ifp, true);

        if (buffer.has_holes())
        {
            warning
            (
                "The data presented for Fletcher-16 checksum has at "
                "least one hole in it.  This is bad.  It means that "
                "the in-memory calculation performed by your embedded "
                "system will be different than the calculation "
                "performed here.  You are strongly advised to use the "
                "--fill 0xFF filter *before* this Fletcher-16 filter to "
                "ensure both calculations are using the same byte "
                "values.  See srec_info(1) for how to see the holes."
            );
        }
    }

    //
    // Pass on the header of the deeper file.
    //
    if (!have_forwarded_header)
    {
        have_forwarded_header = true;
        srec_record *rp = buffer.get_header();
        if (rp)
        {
            record = *rp;
            return true;
        }
    }

    if (!have_given_checksum)
    {
        have_given_checksum = true;

        //
        // Now calculate the Fletcher 16 checksum the bytes in order from
        // lowest address to highest.  (Holes are ignored, not filled.)
        //
        srec_memory_walker_fletcher16::pointer w =
            srec_memory_walker_fletcher16::create();
        buffer.walk(w);
        unsigned short fletcher = w->get();

        //
        // Turn the Fletcher-16 checksum into the first data record.
        //
        unsigned char chunk[2];
        srec_record::encode(chunk, fletcher, sizeof(chunk), end);
        record =
            srec_record(srec_record::type_data, address, chunk, sizeof(chunk));
        return true;
    }

    //
    // Now resend the rest of the data.
    //
    unsigned long ret_address = buffer_pos;
    unsigned char data[64];
    size_t nbytes = sizeof(data);
    if (buffer.find_next_data(ret_address, data, nbytes))
    {
        record = srec_record(srec_record::type_data, ret_address, data, nbytes);
        buffer_pos = ret_address + nbytes;
        return true;
    }

    //
    // Pass on the header of the deeper file.
    //
    if (!have_forwarded_start_address)
    {
        have_forwarded_start_address = true;
        srec_record *rp = buffer.get_execution_start_address();
        if (rp)
        {
            record = *rp;
            return true;
        }
    }

    //
    // All done.
    //
    return false;
}

// vim:ts=8:sw=4:et
