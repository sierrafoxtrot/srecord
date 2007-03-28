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
// MANIFEST: functions to impliment the srec_output_file_ti_tagged class
//

#include <lib/srec/output/file/ti_tagged_16.h>
#include <lib/srec/record.h>

#include <cctype>


srec_output_file_ti_tagged_16::srec_output_file_ti_tagged_16(
        const string &a_file_name) :
    srec_output_file(a_file_name),
    address(0),
    column(0),
    line_length(74),
    csum(0)
{
}


srec_output_file_ti_tagged_16::~srec_output_file_ti_tagged_16()
{
    if (column)
        put_eoln();
}


void
srec_output_file_ti_tagged_16::put_char(int c)
{
    if (c == '\n')
    {
        csum = 0;
        column = 0;
    }
    else
    {
        csum += (unsigned char)c;
        ++column;
    }
    inherited::put_char(c);
}


void
srec_output_file_ti_tagged_16::put_eoln()
{
    put_char('7');
    put_word(-csum);
    put_char('F');
    put_char('\n');
}


void
srec_output_file_ti_tagged_16::write(const srec_record &record)
{
    int pos = 0;
    switch (record.get_type())
    {
    case srec_record::type_header:
        if (!data_only_flag)
        {
            put_stringf("K%4.4X", 5 + record.get_length());
            const unsigned char *cp = record.get_data();
            const unsigned char *ep = cp + record.get_length();
            while (cp < ep)
            {
                unsigned char c = *cp++;
                if (!isprint(c))
                    c = ' ';
                put_char(c);
            }
        }
        break;

    case srec_record::type_data:
        if (record.get_address() + record.get_length() > (1UL << 17))
        {
            fatal_error
            (
                "data address (0x%lX..0x%lX) too large",
                record.get_address(),
                record.get_address() + record.get_length() - 1
            );
        }
        // assert(record.get_length() > 0);
        if (record.get_length() == 0)
            break;
        pos = 0;
        if (address != record.get_address())
        {
            address = record.get_address();
            if (column + 5 > line_length)
                put_eoln();
            put_char('9');
            put_word(address >> 1);

            //
            // Odd addresses are impossible, so we have to handle them
            // carefully.  We fake an 0xFF byte before the first byte of
            // real data, this will leave the erased EPROM data alone.
            //
            // The srec_cat command only ever writes data in ascending
            // order, and always on even address boundaries, so this
            // will not cause duplicate values for any addresses.
            //
            if (address & 1)
            {
                if (column + 5 > line_length)
                    put_eoln();
                put_char('B');
                put_byte(0xFF);
                put_byte(record.get_data(0));
                ++address;
                pos = 1;
            }
        }
        for (; pos + 2 <= record.get_length(); pos += 2)
        {
            if (column + 5 > line_length)
                put_eoln();
            put_char('B');
            put_byte(record.get_data(pos));
            put_byte(record.get_data(pos + 1));
            address += 2;
        }
        for (; pos < record.get_length(); ++pos)
        {
            if (column + 3 > line_length)
                put_eoln();
            put_char('*');
            put_byte(record.get_data(pos));
            ++address;
        }
        break;

    case srec_record::type_data_count:
        // ignore
        break;

    case srec_record::type_start_address:
        if (column)
            put_eoln();
        if (data_only_flag)
            break;
        put_char(':');
        put_char('\n');
        break;

    case srec_record::type_unknown:
        fatal_error("can't write unknown record type");
    }
}


void
srec_output_file_ti_tagged_16::line_length_set(int linlen)
{
    // reduce the line length by 6 characters, to account for the
    // checksum, so we don't keep subtracting it later.
    line_length = linlen - 6;

    // make sure the line is long enough to do anything useful
    if (line_length < 5)
        line_length = 5;
}


void
srec_output_file_ti_tagged_16::address_length_set(int)
{
    // ignore (addresses are always 16 bits)
}


int
srec_output_file_ti_tagged_16::preferred_block_size_get()
    const
{
    int n = (line_length / 5) * 2;
    if (n < 1)
        n = 1;
    if (n > srec_record::max_data_length)
        n = srec_record::max_data_length;
    return n;
}
