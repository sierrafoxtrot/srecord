//
//      srecord - manipulate eprom load files
//      Copyright (C) 2003, 2005-2007 Peter Miller
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
// MANIFEST: functions to impliment the srec_input_file_fairchild class
//


#include <lib/srec/input/file/fairchild.h>
#include <lib/srec/record.h>


srec_input_file_fairchild::~srec_input_file_fairchild()
{
}


srec_input_file_fairchild::srec_input_file_fairchild(const string &a_filename) :
    srec_input_file(a_filename),
    header_seen(false),
    address(0),
    file_contains_data(false)
{
}


int
srec_input_file_fairchild::get_nibble()
{
    int n = srec_input_file::get_nibble();
    checksum_add(n);
    return n;
}


int
srec_input_file_fairchild::get_byte()
{
    // this differs from the srec_input_file method only in that we
    // don't add to the checksum.
    int c1 = get_nibble();
    int c2 = get_nibble();
    return ((c1 << 4) | c2);
}


int
srec_input_file_fairchild::read(srec_record &record)
{
    if (!header_seen)
    {
        bool garbage_warning = false;
        for (;;)
        {
            int c = get_char();
            if (c < 0)
                fatal_error("format error");
            if (c == 'S')
            {
                get_char_undo(c);
                break;
            }
            if (!garbage_warning)
            {
                fatal_error("garbage lines ignored");
                garbage_warning = true;
            }
        }
        header_seen = true;
    }
    for (;;)
    {
        int c = get_char();
        switch (c)
        {
        case -1:
            fatal_error("no * end record");

        case '*':
            if (!file_contains_data)
                fatal_error("file contains no data");
            get_char_undo(c);
            return 0;

        case 'S':
            address = get_word();
            break;

        case 'X':
            {
                checksum_reset();
                unsigned char data[8];
                for (int j = 0; j < 8; ++j)
                    data[j] = get_byte();
                record = srec_record(srec_record::type_data, address, data, 8);
                address += 8;
                file_contains_data = true;

                //
                // Verify the checksum.
                //
                int sum1 = checksum_get() & 0xF;
                int sum2 = get_nibble();
                if (use_checksums() && sum1 != sum2)
                    fatal_error("checksum error (%d != %d)", sum1, sum2);
            }
            return 1;

        default:
            //
            // The documentation says to ignore all the other characters
            // in the file.
            //
            break;
        }

    }
}


const char *
srec_input_file_fairchild::get_file_format_name()
    const
{
    return "Fairchild Fairbug";
}
