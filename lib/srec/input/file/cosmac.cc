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
// MANIFEST: functions to impliment the srec_input_file_cosmac class
//


#include <lib/srec/input/file/cosmac.h>
#include <lib/srec/record.h>


srec_input_file_cosmac::~srec_input_file_cosmac()
{
}


srec_input_file_cosmac::srec_input_file_cosmac(const string &a_file_name) :
    srec_input_file(a_file_name),
    state('\n'),
    address(0),
    seen_some_input(false)
{
}


int
srec_input_file_cosmac::read(srec_record &record)
{
    for (;;)
    {
        int c = get_char();
        switch (state)
        {
        case '\n':
            // want another command
            if (c < 0)
            {
                if (!seen_some_input)
                    fatal_error("file contains no data");
                return 0;
            }
            if (c != '!')
            {
                format_error:
                fatal_error("format error");
                // NOTREACHED
            }
            state = c;
            break;

        case '!':
            // want the 2nd half of "!M"
            if (c != 'M')
                goto format_error;
            state = c;
            break;

        case 'M':
            // either we want an address, or we want a simicolon (an
            // address will follow a semicolon)
            if (c == ';')
            {
                state = c;
                continue;
            }
            if (c == ' ')
                continue;

            address = 0;
            for (int n = 0; ; ++n)
            {
                // Check the length of the address.  The RCA Cosmac only
                // had 16-bit addresses, but we'll allow 32.
                if (n >= 8)
                    goto format_error;

                get_char_undo(c);
                address = (address << 4) + get_nibble();
                c = get_char();
                switch (c)
                {
                default:
                    continue;

                case ' ':
                case ',':
                    state = c;
                    break;
                }
                break;
            }

        case ';':
            if (c < 0)
                goto format_error;
            if (c == '\n')
                state = 'M';
            break;

        case ',':
            if (c < 0)
                goto format_error;
            if (c == '\n')
                state = ' ';
            break;

        case ' ':
            if (c == ';' || c == ',' || c == '\n' || c == ' ')
            {
                state = c;
                continue;
            }
            get_char_undo(c);
            unsigned char data = get_byte();
            record = srec_record(srec_record::type_data, address, &data, 1);
            ++address;
            seen_some_input = true;
            return 1;
        }
    }
}


const char *
srec_input_file_cosmac::get_file_format_name()
    const
{
    return "RCA Cosmac";
}
