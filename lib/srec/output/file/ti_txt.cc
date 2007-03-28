//
//      srecord - manipulate eprom load files
//      Copyright (C) 2007 Peter Miller
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
// MANIFEST: functions to impliment the srec_output_file_ti_txt class
//

#include <lib/srec/output/file/ti_txt.h>
#include <lib/srec/record.h>


srec_output_file_ti_txt::srec_output_file_ti_txt(const char *filename) :
    srec_output_file(filename),
    address(),
    address_set(false),
    address_length(2),
    pref_block_size(16),
    column(0),
    line_length(pref_block_size * 3 - 1)
{
}


srec_output_file_ti_txt::~srec_output_file_ti_txt()
{
    if (address & 1)
    {
        // The format definition says there is always an even number of
        // bytes.  Pad with 0xFF if necessary.
        put_byte_wrap(0xFF);
    }
    if (column > 0)
        put_char('\n');
    put_stringf("Q\n");
    column = 0;
}


void
srec_output_file_ti_txt::put_byte_wrap(unsigned char c)
{
    if (column && column + 3 > line_length)
    {
        put_char('\n');
        column = 0;
    }
    if (column)
    {
        put_char(' ');
        ++column;
    }
    put_byte(c);
    ++address;
    column += 2;
}


void
srec_output_file_ti_txt::write(const srec_record &record)
{
    switch (record.get_type())
    {
    case srec_record::type_header:
        // All header data is discarded
        break;

    case srec_record::type_data:
        if (address_set && (address & 1) && address != record.get_address())
        {
            // assert(address_set);

            // The format definition says there is always an even number
            // of bytes in each section, and sections always start on
            // even addresses.  Pad with 0xFF if necessary.
            put_byte_wrap(0xFF);

            // There is a teeny tiny chance that once the data is
            // padded, the address will one again line up.  We want to
            // do nothing if at all possible, which is why we do the
            // address check all over again.
        }

        //
        // Watch out for the corner case where the end of one section is
        // padded and the beginning of the next section is padded, and
        // they line up perfectly.
        //
        if
        (
            address_set
        &&
            (record.get_address() & 1)
        &&
            address + 1 == record.get_address()
        )
        {
            put_byte_wrap(0xFF);
        }

        if (!address_set || address != record.get_address())
        {
            if (column > 0)
            {
                put_char('\n');
                column = 0;
            }

            address = record.get_address();
            address_set = true;

            bool bogus_byte = false;
            if (address & 1)
            {
                // The format definition requires that all addresses are
                // even.  So we will adjust with a bogus 0xFF byte.
                address &= ~1;
                bogus_byte = true;
            }

            int width = 2;
            if (address >= 0x1000000)
                width = 4;
            if (address >= 0x10000)
                width = 3;
            if (width < address_length)
                width = address_length;
            width *= 2;
            put_stringf("@%0*lX\n", width, address);

            if (bogus_byte)
                put_byte_wrap(0xFF);
        }
        for (int j = 0; j < record.get_length(); ++j)
        {
            put_byte_wrap(record.get_data(j));
        }
        break;

    case srec_record::type_data_count:
        // ignore
        break;

    case srec_record::type_start_address:
        // ignore
        break;

    case srec_record::type_unknown:
        fatal_error("can't write unknown record type");
    }
}


void
srec_output_file_ti_txt::line_length_set(int linlen)
{
    int n = (linlen + 1) / 3;
    if (n < 1)
        n = 1;
    if (n > srec_record::max_data_length)
        n = srec_record::max_data_length;
    pref_block_size = n;
    line_length = pref_block_size * 3 - 1;
}


void
srec_output_file_ti_txt::address_length_set(int n)
{
    if (n < 2)
        n = 2;
    if (n > 4)
        n = 4;
    address_length = n;
}


int
srec_output_file_ti_txt::preferred_block_size_get()
    const
{
    return pref_block_size;
}
