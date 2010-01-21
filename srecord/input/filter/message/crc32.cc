//
//      srecord - manipulate eprom load files
//      Copyright (C) 2000-2003, 2006-2010 Peter Miller
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

#include <srecord/arglex/tool.h>
#include <srecord/input/filter/message/crc32.h>
#include <srecord/memory/walker/crc32.h>
#include <srecord/record.h>


srec_input_filter_message_crc32::~srec_input_filter_message_crc32()
{
}


srec_input_filter_message_crc32::srec_input_filter_message_crc32(
    const srec_input::pointer &a_deeper,
    unsigned long a_address,
    endian_t a_end
) :
    srec_input_filter_message(a_deeper),
    address(a_address),
    end(a_end),
    seed_mode(crc32::seed_mode_ccitt)
{
}


srec_input::pointer
srec_input_filter_message_crc32::create(const srec_input::pointer &a_deeper,
    unsigned long a_address, endian_t a_end)
{
    return
        pointer
        (
            new srec_input_filter_message_crc32(a_deeper, a_address, a_end)
        );
}


void
srec_input_filter_message_crc32::command_line(srec_arglex_tool *cmdln)
{
    for (;;)
    {
        switch (cmdln->token_cur())
        {
        case srec_arglex_tool::token_crc16_xmodem:
            seed_mode = crc32::seed_mode_xmodem;
            cmdln->token_next();
            break;

        case srec_arglex_tool::token_crc16_ccitt:
            seed_mode = crc32::seed_mode_ccitt;
            cmdln->token_next();
            break;

        default:
            return;
        }
    }
}


void
srec_input_filter_message_crc32::process(const srec_memory &input,
    srec_record &output)
{
    //
    // Now CRC32 the bytes in order from lowest address to highest.
    // (Holes are ignored, not filled, warning already issued.)
    //
    srec_memory_walker_crc32::pointer w =
        srec_memory_walker_crc32::create(seed_mode);
    input.walk(w);
    unsigned long crc = w->get();

    //
    // Turn the CRC into the first data record.
    //
    unsigned char chunk[4];
    srec_record::encode(chunk, crc, sizeof(chunk), end);
    output = srec_record(srec_record::type_data, address, chunk, sizeof(chunk));
}


const char *
srec_input_filter_message_crc32::get_algorithm_name()
    const
{
    return "CRC32";
}
