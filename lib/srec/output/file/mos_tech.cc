//
//      srecord - manipulate eprom load files
//      Copyright (C) 2000-2002, 2006-2008 Peter Miller
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

#include <lib/srec/output/file/mos_tech.h>
#include <lib/srec/record.h>


srec_output_file_mos_tech::~srec_output_file_mos_tech()
{
}


srec_output_file_mos_tech::srec_output_file_mos_tech(
        const std::string &a_filename) :
    srec_output_file(a_filename),
    pref_block_size(24),
    data_record_count(0)
{
    // The block size of 24 comes from the sole example I have seen of a
    // valid MOS Tech file.
}


srec_output::pointer
srec_output_file_mos_tech::create(const std::string &a_file_name)
{
    return pointer(new srec_output_file_mos_tech(a_file_name));
}


void
srec_output_file_mos_tech::write(const srec_record &record)
{
    switch (record.get_type())
    {
    case srec_record::type_header:
        // This format can't do header records
        break;

    case srec_record::type_data:
        if (record.get_length() < 1)
            return;
        if (record.get_address() + record.get_length() > (1UL << 16))
        {
            fatal_error
            (
                "data address (0x%lX..0x%lX) too large",
                record.get_address(),
                record.get_address() + record.get_length() - 1
            );
        }
        put_char(';');
        checksum_reset();
        put_byte(record.get_length());
        put_word(record.get_address());
        for (int j = 0; j < record.get_length(); ++j)
            put_byte(record.get_data(j));
        put_word(checksum_get16());
        put_char('\n');
        ++data_record_count;
        break;

    case srec_record::type_data_count:
        // ignore
        break;

    case srec_record::type_start_address:
        if (data_only_flag)
            break;
        put_char(';');
        checksum_reset();
        put_byte(0);
        put_word(data_record_count);
        // In the only file example I have, the count is repeated
        // in the checksum, which would you make you think that the
        // address field is added as a 16-bit value, except that
        // only the data count line is wrong.  Sheesh.
        put_word(data_record_count);
        //put_word(checksum_get16());
        put_char('\n');
        break;

    case srec_record::type_unknown:
        fatal_error("can't write unknown record type");
    }
}


void
srec_output_file_mos_tech::line_length_set(int linlen)
{
    //
    // Given the number of characters, figure the maximum number of
    // data baytes.
    // ';' <len> <addr> ...data... <cs>
    // 1 +  2 +  4 +    2*n +      2       <= linlen
    //
    int n = (linlen - 9) / 2;

    //
    // Constrain based on the file format.
    //
    if (n < 1)
        n = 1;
    else if (n > 255)
        n = 255;

    //
    // An additional constraint is the size of the srec_record
    // data buffer.
    //
    if (n > srec_record::max_data_length)
        n = srec_record::max_data_length;
    pref_block_size = n;
}


void
srec_output_file_mos_tech::address_length_set(int)
{
    // ignore (this is only a 16-bit format)
}


int
srec_output_file_mos_tech::preferred_block_size_get()
    const
{
    return pref_block_size;
}


const char *
srec_output_file_mos_tech::format_name()
    const
{
    return "MOS-Tech";
}
