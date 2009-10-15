//
// srecord - manipulate eprom load files
// Copyright (C) 2008, 2009 Peter Miller
// Copyright (C) 2008 Torsten Sadowski
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

#include <lib/srec/output/file/forth.h>
#include <lib/srec/record.h>
#include <lib/srec/arglex.h>


srec_output_file_forth::~srec_output_file_forth()
{
    emit_end_of_file();
}


srec_output_file_forth::srec_output_file_forth(
        const std::string &a_file_name) :
    srec_output_file(a_file_name),
    address(0),
    store_cmd("C!")
{
}


srec_output::pointer
srec_output_file_forth::create(const std::string &a_file_name)
{
    return pointer(new srec_output_file_forth(a_file_name));
}


void
srec_output_file_forth::emit_end_of_file()
{
    put_char('\n');

    if (enable_footer_flag)
    {
        put_stringf("( THE END )\n");
    }
}


void
srec_output_file_forth::write(const srec_record &record)
{
    switch (record.get_type())
    {
    case srec_record::type_header:
        put_string("HEX\n");
        break;

    case srec_record::type_data:
        address = record.get_address();
        //
        // Now write out the new address.  It is important not to
        // disturb the checksum, so don't use the put_byte method.
        //
        for (int j = 0; j < record.get_length(); ++j)
        {
            put_byte(record.get_data(j));
            put_char(' ');
            put_word(address);
            put_char(' ');
            put_string(store_cmd);
            ++address;
            put_char('\n');
        }
        break;

    case srec_record::type_data_count:
        // ignore
        break;

    case srec_record::type_execution_start_address:
        // Ignore.
        // The destructor sill call emit_end_of_file();
        break;

    case srec_record::type_unknown:
        fatal_error("can't write unknown record type");
    }
}


const char *
srec_output_file_forth::format_name()
    const
{
    return "FORTH";
}

void
srec_output_file_forth::command_line(srec_arglex *cmdln)
{
    for (;;)
    {
        switch (cmdln->token_cur())
        {
        case srec_arglex::token_ram:
            cmdln->token_next();
            // Store into RAM
            store_cmd = "C!";
            break;

        case srec_arglex::token_eeprom:
            cmdln->token_next();
            // Store into EEPROM
            store_cmd = "EEC!";
            break;

        default:
            return;
        }
    }
}

void srec_output_file_forth::line_length_set(int)
{
}

void srec_output_file_forth::address_length_set(int)
{
}

int srec_output_file_forth::preferred_block_size_get() const
{
  return 16;
}
