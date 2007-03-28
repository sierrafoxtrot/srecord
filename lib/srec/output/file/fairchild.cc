//
//      srecord - manipulate eprom load files
//      Copyright (C) 2003, 2006, 2007 Peter Miller
//
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111, USA.
//
// MANIFEST: functions to impliment the srec_output_file_fairchild class
//


#include <lib/srec/output/file/fairchild.h>
#include <lib/srec/record.h>


srec_output_file_fairchild::~srec_output_file_fairchild()
{
}


srec_output_file_fairchild::srec_output_file_fairchild(
        const string &a_file_name) :
    srec_output_file(a_file_name),
    address(~0uL),
    data_bytes_emitted(0)
{
}


void
srec_output_file_fairchild::put_nibble(unsigned n)
{
    srec_output_file::put_nibble(n);
    checksum_add(n & 15);
}


void
srec_output_file_fairchild::put_byte(unsigned char n)
{
    // This differs from srec_output_file::put_byte only in that it
    // doesn't add to the checksum.
    put_nibble(n >> 4);
    put_nibble(n);
}


void
srec_output_file_fairchild::emit(unsigned long addr, unsigned char data)
{
    if (data_bytes_emitted != 0)
    {
        //
        // Fill out the data record until we reach the address we want,
        // or the data record is full.  There is no way to have a short
        // data record.
        //
        while (address < addr && data_bytes_emitted < 8)
        {
            put_byte(0xFF);
            ++address;
            ++data_bytes_emitted;
        }

        //
        // See if we have reached the end of a data record.
        // (They only ever have 8 bytes.)
        //
        if (data_bytes_emitted >= 8)
        {
            put_nibble(checksum_get());
            put_char('\n');
            data_bytes_emitted = 0;
        }
    }
    if (data_bytes_emitted == 0)
    {
        if (address != addr)
        {
            put_stringf("S%4.4lX\n", addr);
            address = addr;
        }
        put_char('X');
        checksum_reset();
    }

    //
    // Emit the one byte of data we were given.
    //
    put_byte(data);
    ++address;
    ++data_bytes_emitted;

    //
    // See if we have reached the end of a data record.
    // (They only ever have 8 bytes.)
    //
    if (data_bytes_emitted >= 8)
    {
        put_nibble(checksum_get());
        put_char('\n');
        data_bytes_emitted = 0;
    }
}


void
srec_output_file_fairchild::write(const srec_record &record)
{
    switch (record.get_type())
    {
    case srec_record::type_header:
    case srec_record::type_unknown:
    case srec_record::type_data_count:
        // ignore
        break;

    case srec_record::type_data:
        {
            int len = record.get_length();
            unsigned long new_addr = record.get_address();
            for (int j = 0; j < len; ++j, ++new_addr)
                emit(new_addr, record.get_data(j));
        }
        break;

    case srec_record::type_start_address:
        if (data_bytes_emitted != 0)
        {
            //
            // Fill out the data record until it is full.  There is no
            // way to have a short data record.
            //
            for (;;)
            {
                put_byte(0xFF);
                ++address;
                ++data_bytes_emitted;
                if (data_bytes_emitted >= 8)
                    break;
            }
            put_nibble(checksum_get());
            put_char('\n');
            data_bytes_emitted = 0;
        }
        put_string("*\n");
        break;
    }
}


void
srec_output_file_fairchild::line_length_set(int)
{
    // ignore
}


void
srec_output_file_fairchild::address_length_set(int)
{
    // ignore
}


int
srec_output_file_fairchild::preferred_block_size_get()
    const
{
    return 8;
}
