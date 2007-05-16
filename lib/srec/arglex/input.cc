//
//      srecord - manipulate eprom load files
//      Copyright (C) 1998-2007 Peter Miller
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
// MANIFEST: functions to impliment the srec_arglex input method
//

#include <iostream>
using namespace std;

#include <lib/srec/arglex.h>
#include <lib/srec/input/file/aomf.h>
#include <lib/srec/input/file/ascii_hex.h>
#include <lib/srec/input/file/atmel_generic.h>
#include <lib/srec/input/file/binary.h>
#include <lib/srec/input/file/brecord.h>
#include <lib/srec/input/file/cosmac.h>
#include <lib/srec/input/file/dec_binary.h>
#include <lib/srec/input/file/emon52.h>
#include <lib/srec/input/file/fairchild.h>
#include <lib/srec/input/file/fastload.h>
#include <lib/srec/input/file/formatted_binary.h>
#include <lib/srec/input/file/four_packed_code.h>
#include <lib/srec/input/file/guess.h>
#include <lib/srec/input/file/intel.h>
#include <lib/srec/input/file/intel16.h>
#include <lib/srec/input/file/mos_tech.h>
#include <lib/srec/input/file/needham.h>
#include <lib/srec/input/file/os65v.h>
#include <lib/srec/input/file/signetics.h>
#include <lib/srec/input/file/spasm.h>
#include <lib/srec/input/file/spectrum.h>
#include <lib/srec/input/file/srecord.h>
#include <lib/srec/input/file/stewie.h>
#include <lib/srec/input/file/tektronix_extended.h>
#include <lib/srec/input/file/tektronix.h>
#include <lib/srec/input/file/ti_tagged.h>
#include <lib/srec/input/file/ti_tagged_16.h>
#include <lib/srec/input/file/ti_txt.h>
#include <lib/srec/input/file/vmem.h>
#include <lib/srec/input/file/wilson.h>
#include <lib/srec/input/filter/and.h>
#include <lib/srec/input/filter/byte_swap.h>
#include <lib/srec/input/filter/checksum/bitnot.h>
#include <lib/srec/input/filter/checksum/negative.h>
#include <lib/srec/input/filter/checksum/positive.h>
#include <lib/srec/input/filter/crc16.h>
#include <lib/srec/input/filter/crc32.h>
#include <lib/srec/input/filter/crop.h>
#include <lib/srec/input/filter/fill.h>
#include <lib/srec/input/filter/length.h>
#include <lib/srec/input/filter/maximum.h>
#include <lib/srec/input/filter/minimum.h>
#include <lib/srec/input/filter/not.h>
#include <lib/srec/input/filter/offset.h>
#include <lib/srec/input/filter/or.h>
#include <lib/srec/input/filter/random_fill.h>
#include <lib/srec/input/filter/sequence.h>
#include <lib/srec/input/filter/split.h>
#include <lib/srec/input/filter/unfill.h>
#include <lib/srec/input/filter/unsplit.h>
#include <lib/srec/input/filter/xor.h>
#include <lib/srec/input/generator.h>
#include <lib/srec/input/interval.h>


srec_input *
srec_arglex::get_input()
{
    //
    // determine the file name
    //
    std::string fn = "-";
    switch (token_cur())
    {
    case token_paren_begin:
        {
            token_next();
            srec_input *ifp = get_input();
            if (token_cur() != token_paren_end)
            {
                fatal_error
                (
                    "closing parenthesis expected before %s",
                    token_name(token_cur())
                );
                // NOTREACHED
            }
            token_next();
            return ifp;
        }

    case token_generator:
        // Don't need a file name,
        // but do NOT discard this token, yet.
        break;

    case token_string:
        fn = value_string();
        token_next();
        break;

    case token_stdio:
        token_next();
        // fall through...

    default:
        if (stdin_used)
        {
            fatal_error
            (
                "the standard input may only be named once on the command line"
            );
            // NOTREACHED
        }
        stdin_used = true;
        break;
    }

    //
    // determine the file format
    // and open the input file
    //
    srec_input *ifp;
    switch (token_cur())
    {
    case token_motorola:
        token_next();
        // fall through...

    default:
        ifp = new srec_input_file_srecord(fn);
        break;

    case token_aomf:
        token_next();
        ifp = new srec_input_file_aomf(fn);
        break;

    case token_ascii_hex:
        token_next();
        ifp = new srec_input_file_ascii_hex(fn);
        break;

    case token_atmel_generic_be:
        token_next();
        ifp = new srec_input_file_atmel_generic(fn, true);
        break;

    case token_atmel_generic_le:
        token_next();
        ifp = new srec_input_file_atmel_generic(fn, false);
        break;

    case token_binary:
        token_next();
        ifp = new srec_input_file_binary(fn);
        break;

    case token_brecord:
        token_next();
        ifp = new srec_input_file_brecord(fn);
        break;

    case token_cosmac:
        token_next();
        ifp = new srec_input_file_cosmac(fn);
        break;

    case token_dec_binary:
        token_next();
        ifp = new srec_input_file_dec_binary(fn);
        break;

    case token_emon52:
        token_next();
        ifp = new srec_input_file_emon52(fn);
        break;

    case token_fairchild:
        token_next();
        ifp = new srec_input_file_fairchild(fn);
        break;

    case token_fast_load:
        token_next();
        ifp = new srec_input_file_fastload(fn);
        break;

    case token_formatted_binary:
        token_next();
        ifp = new srec_input_file_formatted_binary(fn);
        break;

    case token_four_packed_code:
        token_next();
        ifp = new srec_input_file_four_packed_code(fn);
        break;

    case token_generator:
        token_next();
        ifp = srec_input_generator::create(this);
        break;

    case token_guess:
        token_next();
        ifp = srec_input_file_guess(fn);
        break;

    case token_intel:
        token_next();
        ifp = new srec_input_file_intel(fn);
        break;

    case token_intel16:
        token_next();
        ifp = new srec_input_file_intel16(fn);
        break;

    case token_mos_tech:
        token_next();
        ifp = new srec_input_file_mos_tech(fn);
        break;

    case token_ohio_scientific:
        token_next();
        ifp = new srec_input_file_os65v(fn);
        break;

    case token_needham_hex:
        token_next();
        ifp = new srec_input_file_needham(fn);
        break;

    case token_signetics:
        token_next();
        ifp = new srec_input_file_signetics(fn);
        break;

    case token_spasm_be:
        token_next();
        ifp = new srec_input_file_spasm(fn, true);
        break;

    case token_spasm_le:
        token_next();
        ifp = new srec_input_file_spasm(fn, false);
        break;

    case token_spectrum:
        token_next();
        ifp = new srec_input_file_spectrum(fn);
        break;

    case token_stewie:
        token_next();
        ifp = new srec_input_file_stewie(fn);
        break;

    case token_tektronix:
        token_next();
        ifp = new srec_input_file_tektronix(fn);
        break;

    case token_tektronix_extended:
        token_next();
        ifp = new srec_input_file_tektronix_extended(fn);
        break;

    case token_ti_tagged:
        token_next();
        ifp = new srec_input_file_ti_tagged(fn);
        break;

    case token_ti_tagged_16:
        token_next();
        ifp = new srec_input_file_ti_tagged_16(fn);
        break;

    case token_ti_txt:
        token_next();
        ifp = new srec_input_file_ti_txt(fn);
        break;

    case token_vmem:
        token_next();
        ifp = new srec_input_file_vmem(fn);
        break;

    case token_wilson:
        token_next();
        ifp = new srec_input_file_wilson(fn);
        break;
    }

    //
    // Process any additional format-specfic command line options.
    //
    ifp->command_line(this);

    //
    // Ignore checksums if asked to.
    //
    if (token_cur() == token_ignore_checksums)
    {
        ifp->disable_checksum_validation();
        token_next();
    }

    //
    // warn about data record sequences, if asked to
    //
    if (issue_sequence_warnings != 0)
        ifp = new srec_input_filter_sequence(ifp);

    //
    // apply any filters specified
    //
    for (;;)
    {
        switch (token_cur())
        {
        case token_byte_swap:
            token_next();
            ifp = new srec_input_filter_byte_swap(ifp);
            break;

        case token_not:
            token_next();
            ifp = new srec_input_filter_not(ifp);
            break;

        case token_crc16_be:
            {
                token_next();
                unsigned long address;
                get_address("-Big_Endian_CRC16", address);
                bool ccitt_seed = true;
                if (token_cur() == token_crc16_xmodem)
                {
                    token_next();
                    ccitt_seed = false;
                }
                ifp = new srec_input_filter_crc16(ifp, address, 0, ccitt_seed);
            }
            break;

        case token_crc16_le:
            {
                token_next();
                unsigned long address;
                get_address("-Little_Endian_CRC16", address);
                bool ccitt_seed = true;
                if (token_cur() == token_crc16_xmodem)
                {
                    token_next();
                    ccitt_seed = false;
                }
                ifp = new srec_input_filter_crc16(ifp, address, 1, ccitt_seed);
            }
            break;

        case token_crc32_be:
            {
                token_next();
                unsigned long address;
                get_address("-Big_Endian_CRC32", address);
                ifp = new srec_input_filter_crc32(ifp, address, 0);
            }
            break;

        case token_crc32_le:
            {
                token_next();
                unsigned long address;
                get_address("-Little_Endian_CRC32", address);
                ifp = new srec_input_filter_crc32(ifp, address, 1);
            }
            break;

        case token_crop:
            token_next();
            ifp = new srec_input_filter_crop(ifp, get_interval("-Crop"));
            break;

        case token_exclude:
            token_next();
            ifp = new srec_input_filter_crop(ifp, -get_interval("-Exclude"));
            break;

        case token_fill:
            {
                token_next();
                int filler = get_number("--Fill", 0, 255);
                interval range = get_interval("--Fill");
                ifp = new srec_input_filter_fill(ifp, filler, range);
            }
            break;

        case token_random_fill:
            {
                token_next();
                interval range = get_interval("-Random_Fill");
                ifp = new srec_input_filter_random_fill(ifp, range);
            }
            break;

        case token_and:
            {
                token_next();
                int filler = get_number("--and", 0, 255);
                if (filler < 0 || filler >= 256)
                {
                    fatal_error("-and value %d out of range (0..255)", filler);
                    // NOTREACHED
                }
                ifp = new srec_input_filter_and(ifp, filler);
            }
            break;

        case token_xor:
            {
                token_next();
                int filler = get_number("--xor", 0, 255);
                ifp = new srec_input_filter_xor(ifp, filler);
            }
            break;

        case token_or:
            {
                token_next();
                int filler = get_number("--or value", 0, 255);
                ifp = new srec_input_filter_or(ifp, filler);
            }
            break;

        case token_length:
            fatal_error("Use --big-endian-length or --little-endian-length");
            // NOTREACHED

        case token_length_be:
            {
                token_next();
                unsigned long address;
                int nbytes;
                get_address_and_nbytes("-Big_Endian_Length", address, nbytes);
                ifp = new srec_input_filter_length(ifp, address, nbytes, 0);
            }
            break;

        case token_length_le:
            {
                token_next();
                unsigned long address;
                int nbytes;
                get_address_and_nbytes
                (
                    "-Little_Endian_Length",
                    address,
                    nbytes
                );
                ifp = new srec_input_filter_length(ifp, address, nbytes, 1);
            }
            break;

        case token_maximum:
            fatal_error("Use --big-endian-maximum or --little-endian-maximum");
            // NOTREACHED

        case token_maximum_be:
            {
                token_next();
                unsigned long address;
                int nbytes;
                get_address_and_nbytes("-Big_Endian_MAximum", address, nbytes);
                ifp = new srec_input_filter_maximum(ifp, address, nbytes, 0);
            }
            break;

        case token_maximum_le:
            {
                token_next();
                unsigned long address;
                int nbytes;
                get_address_and_nbytes
                (
                    "-Little_Endian_MAximum",
                    address,
                    nbytes
                );
                ifp = new srec_input_filter_maximum(ifp, address, nbytes, 1);
            }
            break;

        case token_minimum:
            fatal_error("Use --big-endian-minimum or --little-endian-minimum");
            // NOTREACHED

        case token_minimum_be:
            {
                token_next();
                unsigned long address;
                int nbytes;
                get_address_and_nbytes("-Big_Endian_MInimum", address, nbytes);
                ifp = new srec_input_filter_minimum(ifp, address, nbytes, 0);
            }
            break;

        case token_minimum_le:
            {
                token_next();
                unsigned long address;
                int nbytes;
                get_address_and_nbytes
                (
                    "-Little_Endian_MInimum",
                    address,
                    nbytes
                );
                ifp = new srec_input_filter_minimum(ifp, address, nbytes, 1);
            }
            break;

        case token_checksum_be_bitnot:
            {
                token_next();
                unsigned long address;
                int nbytes, width;
                get_address_nbytes_width
                (
                    "-Big_Endian_Checksum_BitNot",
                    address,
                    nbytes,
                    width
                );
                ifp =
                    new srec_input_filter_checksum_bitnot
                    (
                        ifp,
                        address,
                        nbytes,
                        0,
                        width
                    );
            }
            break;

        case token_checksum_le_bitnot:
            {
                token_next();
                unsigned long address;
                int nbytes, width;
                get_address_nbytes_width
                (
                    "-Little_Endian_Checksum_BitNot",
                    address,
                    nbytes,
                    width
                );
                ifp =
                    new srec_input_filter_checksum_bitnot
                    (
                        ifp,
                        address,
                        nbytes,
                        1,
                        width
                    );
            }
            break;

        case token_checksum_be_negative:
            {
                token_next();
                unsigned long address;
                int nbytes, width;
                get_address_nbytes_width
                (
                    "-Big_Endian_Checksum_Negative",
                    address,
                    nbytes,
                    width
                );
                ifp =
                    new srec_input_filter_checksum_negative
                    (
                        ifp,
                        address,
                        nbytes,
                        0,
                        width
                    );
            }
            break;

        case token_checksum_le_negative:
            {
                token_next();
                unsigned long address;
                int nbytes, width;
                get_address_nbytes_width
                (
                    "-Little_Endian_Checksum_Negative",
                    address,
                    nbytes,
                    width
                );
                ifp =
                    new srec_input_filter_checksum_negative
                    (
                        ifp,
                        address,
                        nbytes,
                        1,
                        width
                    );
            }
            break;

        case token_checksum_be_positive:
            {
                token_next();
                unsigned long address;
                int nbytes, width;
                get_address_nbytes_width
                (
                    "-Big_Endian_Checksum_Positive",
                    address,
                    nbytes,
                    width
                );
                ifp =
                    new srec_input_filter_checksum_positive
                    (
                        ifp,
                        address,
                        nbytes,
                        0,
                        width
                    );
            }
            break;

        case token_checksum_le_positive:
            {
                token_next();
                unsigned long address;
                int nbytes, width;
                get_address_nbytes_width
                (
                    "-Little_Endian_Checksum_Positive",
                    address,
                    nbytes,
                    width
                );
                ifp =
                    new srec_input_filter_checksum_positive
                    (
                        ifp,
                        address,
                        nbytes,
                        1,
                        width
                    );
            }
            break;

        case token_offset:
            {
                token_next();
                unsigned long amount = get_number("--offset");
                ifp = new srec_input_filter_offset(ifp, amount);
            }
            break;

        case token_split:
            {
                token_next();
                int split_modulus = get_number("--split modulus");
                if (split_modulus < 2)
                {
                    fatal_error("the -split modulus must be two or more");
                    // NOTREACHED
                }
                int split_offset = 0;
                if (can_get_number())
                {
                    split_offset =
                        get_number("split offset", 0, split_modulus - 1);
                }
                int split_width = 1;
                if (can_get_number())
                {
                    split_width =
                        get_number("split width", 1, split_modulus - 1);
                }
                ifp =
                    new srec_input_filter_split
                    (
                        ifp,
                        split_modulus,
                        split_offset,
                        split_width
                    );
            }
            break;

        case token_unfill:
            {
                token_next();
                int fill_value = get_number("--unfill value", 0, 255);
                int fill_minimum = 1;
                if (can_get_number())
                {
                    fill_minimum = get_number("--unfill minimum", 1, 16);
                }
                ifp =
                    new srec_input_filter_unfill(ifp, fill_value, fill_minimum);
            }
            break;

        case token_unsplit:
            {
                token_next();
                int split_modulus = get_number("--unsplit modulus");
                if (split_modulus < 2)
                {
                    fatal_error("the -unsplit modulus must be two or more");
                    // NOTREACHED
                }
                int split_offset = 0;
                if (can_get_number())
                {
                    split_offset =
                        get_number("--unsplit offset", 0, split_modulus - 1);
                }
                int split_width = 1;
                if (can_get_number())
                {
                    split_width =
                        get_number("--unsplit width", 1, split_modulus - 1);
                }
                ifp =
                    new srec_input_filter_unsplit
                    (
                        ifp,
                        split_modulus,
                        split_offset,
                        split_width
                    );
            }
            break;

        default:
            //
            // return the input stream determined
            //
            return ifp;
        }

        //
        // Process any filter-specific command line options.
        //
        ifp->command_line(this);
    }
}
