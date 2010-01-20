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

#include <srecord/arglex.h>
#include <srecord/input/filter/message/crc16.h>
#include <srecord/memory.h>
#include <srecord/memory/walker/crc16.h>
#include <srecord/record.h>


srec_input_filter_message_crc16::~srec_input_filter_message_crc16()
{
}


srec_input_filter_message_crc16::srec_input_filter_message_crc16(
        const srec_input::pointer &deeper_arg, unsigned long address_arg,
        endian_t a_end) :
    srec_input_filter_message(deeper_arg),
    address(address_arg),
    end(a_end),
    seed_mode(crc16::seed_mode_ccitt),
    augment_flag(true),
    polynomial(crc16::polynomial_ccitt),
    bitdir(crc16::bit_direction_most_to_least)
{
}


srec_input::pointer
srec_input_filter_message_crc16::create(const srec_input::pointer &a_deeper,
    unsigned long a_address, endian_t a_end)
{
    return
        pointer
        (
            new srec_input_filter_message_crc16(a_deeper, a_address, a_end)
        );
}


void
srec_input_filter_message_crc16::command_line(srec_arglex *cmdln)
{
    for (;;)
    {
        switch (cmdln->token_cur())
        {
        case arglex::token_number:
            polynomial = cmdln->value_number();
            cmdln->token_next();
            break;

        case srec_arglex::token_crc16_most_to_least:
            bitdir = crc16::bit_direction_most_to_least;
            cmdln->token_next();
            break;

        case srec_arglex::token_crc16_least_to_most:
            bitdir = crc16::bit_direction_least_to_most;
            cmdln->token_next();
            break;

        case srec_arglex::token_crc16_xmodem:
            seed_mode = crc16::seed_mode_xmodem;
            cmdln->token_next();
            break;

        case srec_arglex::token_crc16_ccitt:
            seed_mode = crc16::seed_mode_ccitt;
            polynomial = crc16::polynomial_ccitt;
            cmdln->token_next();
            break;

        case srec_arglex::token_crc16_broken:
            seed_mode = crc16::seed_mode_broken;
            cmdln->token_next();
            break;

        case srec_arglex::token_crc16_augment:
            augment_flag = true;
            cmdln->token_next();
            break;

        case srec_arglex::token_crc16_augment_not:
            augment_flag = false;
            cmdln->token_next();
            break;

        default:
            return;
        }
    }
}


void
srec_input_filter_message_crc16::process(const srec_memory &buffer,
    srec_record &record)
{
    //
    // Now CRC16 the bytes in order from lowest address to highest.
    // (Holes are ignored, not filled, warning already issued.)
    //
    srec_memory_walker_crc16::pointer w =
        srec_memory_walker_crc16::create
        (
            seed_mode,
            augment_flag,
            polynomial,
            bitdir
        );
    buffer.walk(w);
    unsigned crc = w->get();

    //
    // Turn the CRC into the first data record.
    //
    unsigned char chunk[2];
    srec_record::encode(chunk, crc, sizeof(chunk), end);
    record = srec_record(srec_record::type_data, address, chunk, sizeof(chunk));
}


const char *
srec_input_filter_message_crc16::get_algorithm_name()
    const
{
    return "CRC16";
}
