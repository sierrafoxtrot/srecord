//
//      srecord - manipulate eprom load files
//      Copyright (C) 1998-2008 Peter Miller
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
#include <lib/srec/input/filter/interval/length.h>
#include <lib/srec/input/filter/interval/maximum.h>
#include <lib/srec/input/filter/interval/minimum.h>
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


srec_input::pointer
srec_arglex::get_simple_input()
{
    std::string fn = "-";
    switch (token_cur())
    {
    case token_paren_begin:
        {
            token_next();
            srec_input::pointer ifp = get_input();
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
    srec_input::pointer ifp;
    switch (token_cur())
    {
    case token_motorola:
        token_next();
        // fall through...

    default:
        ifp = srec_input_file_srecord::create(fn);
        break;

    case token_aomf:
        token_next();
        ifp = srec_input_file_aomf::create(fn);
        break;

    case token_ascii_hex:
        token_next();
        ifp = srec_input_file_ascii_hex::create(fn);
        break;

    case token_atmel_generic_be:
        token_next();
        ifp = srec_input_file_atmel_generic::create(fn, endian_big);
        break;

    case token_atmel_generic_le:
        token_next();
        ifp = srec_input_file_atmel_generic::create(fn, endian_little);
        break;

    case token_binary:
        token_next();
        ifp = srec_input_file_binary::create(fn);
        break;

    case token_brecord:
        token_next();
        ifp = srec_input_file_brecord::create(fn);
        break;

    case token_cosmac:
        token_next();
        ifp = srec_input_file_cosmac::create(fn);
        break;

    case token_dec_binary:
        token_next();
        ifp = srec_input_file_dec_binary::create(fn);
        break;

    case token_emon52:
        token_next();
        ifp = srec_input_file_emon52::create(fn);
        break;

    case token_fairchild:
        token_next();
        ifp = srec_input_file_fairchild::create(fn);
        break;

    case token_fast_load:
        token_next();
        ifp = srec_input_file_fastload::create(fn);
        break;

    case token_formatted_binary:
        token_next();
        ifp = srec_input_file_formatted_binary::create(fn);
        break;

    case token_four_packed_code:
        token_next();
        ifp = srec_input_file_four_packed_code::create(fn);
        break;

    case token_generator:
        token_next();
        ifp = srec_input_generator::create(this);
        break;

    case token_guess:
        token_next();
        ifp = srec_input_file::guess(fn);
        break;

    case token_intel:
        token_next();
        ifp = srec_input_file_intel::create(fn);
        break;

    case token_intel16:
        token_next();
        ifp = srec_input_file_intel16::create(fn);
        break;

    case token_mos_tech:
        token_next();
        ifp = srec_input_file_mos_tech::create(fn);
        break;

    case token_ohio_scientific:
        token_next();
        ifp = srec_input_file_os65v::create(fn);
        break;

    case token_needham_hex:
        token_next();
        ifp = srec_input_file_needham::create(fn);
        break;

    case token_signetics:
        token_next();
        ifp = srec_input_file_signetics::create(fn);
        break;

    case token_spasm_be:
        token_next();
        ifp = srec_input_file_spasm::create(fn, endian_big);
        break;

    case token_spasm_le:
        token_next();
        ifp = srec_input_file_spasm::create(fn, endian_little);
        break;

    case token_spectrum:
        token_next();
        ifp = srec_input_file_spectrum::create(fn);
        break;

    case token_stewie:
        token_next();
        ifp = srec_input_file_stewie::create(fn);
        break;

    case token_tektronix:
        token_next();
        ifp = srec_input_file_tektronix::create(fn);
        break;

    case token_tektronix_extended:
        token_next();
        ifp = srec_input_file_tektronix_extended::create(fn);
        break;

    case token_ti_tagged:
        token_next();
        ifp = srec_input_file_ti_tagged::create(fn);
        break;

    case token_ti_tagged_16:
        token_next();
        ifp = srec_input_file_ti_tagged_16::create(fn);
        break;

    case token_ti_txt:
        token_next();
        ifp = srec_input_file_ti_txt::create(fn);
        break;

    case token_vmem:
        token_next();
        ifp = srec_input_file_vmem::create(fn);
        break;

    case token_wilson:
        token_next();
        ifp = srec_input_file_wilson::create(fn);
        break;
    }
    // assert(ifp);

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
        ifp = srec_input_filter_sequence::create(ifp);

    //
    // report success
    //
    return ifp;
}


srec_input::pointer
srec_arglex::get_input()
{
    srec_input::pointer ifp = get_simple_input();

    //
    // apply any filters specified
    //
    for (;;)
    {
        switch (token_cur())
        {
        case token_byte_swap:
            token_next();
            ifp = srec_input_filter_byte_swap::create(ifp);
            break;

        case token_not:
            token_next();
            ifp = srec_input_filter_not::create(ifp);
            break;

        case token_crc16_be:
            {
                token_next();
                unsigned long address;
                get_address("-Big_Endian_CRC16", address);
                ifp = srec_input_filter_crc16::create(ifp, address, endian_big);
            }
            break;

        case token_crc16_le:
            {
                token_next();
                unsigned long address;
                get_address("-Little_Endian_CRC16", address);
                ifp =
                    srec_input_filter_crc16::create
                    (
                        ifp,
                        address,
                        endian_little
                    );
            }
            break;

        case token_crc32_be:
            {
                token_next();
                unsigned long address;
                get_address("-Big_Endian_CRC32", address);
                ifp = srec_input_filter_crc32::create(ifp, address, endian_big);
            }
            break;

        case token_crc32_le:
            {
                token_next();
                unsigned long address;
                get_address("-Little_Endian_CRC32", address);
                ifp =
                    srec_input_filter_crc32::create
                    (
                        ifp,
                        address,
                        endian_little
                    );
            }
            break;

        case token_crop:
            token_next();
            ifp = srec_input_filter_crop::create(ifp, get_interval("-Crop"));
            break;

        case token_exclude:
            token_next();
            ifp =
                srec_input_filter_crop::create(ifp, -get_interval("-Exclude"));
            break;

        case token_fill:
            {
                token_next();
                int filler = get_number("--Fill", 0, 255);
                interval range = get_interval("--Fill");
                ifp = srec_input_filter_fill::create(ifp, filler, range);
            }
            break;

        case token_random_fill:
            {
                token_next();
                interval range = get_interval("-Random_Fill");
                ifp = srec_input_filter_random_fill::create(ifp, range);
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
                ifp = srec_input_filter_and::create(ifp, filler);
            }
            break;

        case token_xor:
            {
                token_next();
                int filler = get_number("--xor", 0, 255);
                ifp = srec_input_filter_xor::create(ifp, filler);
            }
            break;

        case token_or:
            {
                token_next();
                int filler = get_number("--or value", 0, 255);
                ifp = srec_input_filter_or::create(ifp, filler);
            }
            break;

        case token_length:
            fatal_error("Use --big-endian-length or --little-endian-length");
            // NOTREACHED

        case token_exclusive_length:
            fatal_error
            (
                "Use --big-endian-exclusive-length or "
                    "--little-endian-exclusive-length"
            );
            // NOTREACHED

        case token_length_be:
            {
                token_next();
                unsigned long address;
                int nbytes;
                get_address_and_nbytes("-Big_Endian_Length", address, nbytes);
                ifp =
                    srec_input_filter_length::create
                    (
                        ifp,
                        address,
                        nbytes,
                        endian_big
                    );
            }
            break;

        case token_exclusive_length_be:
            {
                token_next();
                unsigned long address;
                int nbytes;
                get_address_and_nbytes
                (
                    "-Big_Endian_Exclusive_Length",
                    address,
                    nbytes
                );
                ifp =
                    srec_input_filter_interval_length::create
                    (
                        ifp,
                        address,
                        nbytes,
                        0
                    );
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
                ifp =
                    srec_input_filter_length::create
                    (
                        ifp,
                        address,
                        nbytes,
                        endian_little
                    );
            }
            break;

        case token_exclusive_length_le:
            {
                token_next();
                unsigned long address;
                int nbytes;
                get_address_and_nbytes
                (
                    "-Little_Endian_Exclusive_Length",
                    address,
                    nbytes
                );
                ifp =
                    srec_input_filter_interval_length::create
                    (
                        ifp,
                        address,
                        nbytes,
                        1
                    );
            }
            break;

        case token_maximum:
            fatal_error("Use --big-endian-maximum or --little-endian-maximum");
            // NOTREACHED

        case token_exclusive_maximum:
            fatal_error
            (
                "Use --big-endian-exclusive-maximum or "
                    "--little-endian-exclusive-maximum"
            );
            // NOTREACHED

        case token_maximum_be:
            {
                token_next();
                unsigned long address;
                int nbytes;
                get_address_and_nbytes("-Big_Endian_MAximum", address, nbytes);
                ifp =
                    srec_input_filter_maximum::create
                    (
                        ifp,
                        address,
                        nbytes,
                        endian_big
                    );
            }
            break;

        case token_exclusive_maximum_be:
            {
                token_next();
                unsigned long address;
                int nbytes;
                get_address_and_nbytes
                (
                    "-Big_Endian_Exclusive_MAximum",
                    address,
                    nbytes
                );
                ifp =
                    srec_input_filter_interval_maximum::create
                    (
                        ifp,
                        address,
                        nbytes,
                        0
                    );
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
                ifp =
                    srec_input_filter_maximum::create
                    (
                        ifp,
                        address,
                        nbytes,
                        endian_little
                    );
            }
            break;

        case token_exclusive_maximum_le:
            {
                token_next();
                unsigned long address;
                int nbytes;
                get_address_and_nbytes
                (
                    "-Little_Endian_Exclusive_MAximum",
                    address,
                    nbytes
                );
                ifp =
                    srec_input_filter_interval_maximum::create
                    (
                        ifp,
                        address,
                        nbytes,
                        1
                    );
            }
            break;

        case token_minimum:
            fatal_error("Use --big-endian-minimum or --little-endian-minimum");
            // NOTREACHED

        case token_exclusive_minimum:
            fatal_error
            (
                "Use --big-endian-exclusive-minimum or "
                    "--little-endian-exclusive-minimum"
            );
            // NOTREACHED

        case token_minimum_be:
            {
                token_next();
                unsigned long address;
                int nbytes;
                get_address_and_nbytes("-Big_Endian_MInimum", address, nbytes);
                ifp =
                    srec_input_filter_minimum::create
                    (
                        ifp,
                        address,
                        nbytes,
                        endian_big
                    );
            }
            break;

        case token_exclusive_minimum_be:
            {
                token_next();
                unsigned long address;
                int nbytes;
                get_address_and_nbytes
                (
                    "-Big_Endian_Exclusive_MInimum",
                    address,
                    nbytes
                );
                ifp =
                    srec_input_filter_interval_minimum::create
                    (
                        ifp,
                        address,
                        nbytes,
                        0
                    );
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
                ifp =
                    srec_input_filter_minimum::create
                    (
                        ifp,
                        address,
                        nbytes,
                        endian_little
                    );
            }
            break;

        case token_exclusive_minimum_le:
            {
                token_next();
                unsigned long address;
                int nbytes;
                get_address_and_nbytes
                (
                    "-Little_Endian_Exclusive_MInimum",
                    address,
                    nbytes
                );
                ifp =
                    srec_input_filter_interval_minimum::create
                    (
                        ifp,
                        address,
                        nbytes,
                        1
                    );
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
                    srec_input_filter_checksum_bitnot::create
                    (
                        ifp,
                        address,
                        nbytes,
                        endian_big,
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
                    srec_input_filter_checksum_bitnot::create
                    (
                        ifp,
                        address,
                        nbytes,
                        endian_little,
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
                    srec_input_filter_checksum_negative::create
                    (
                        ifp,
                        address,
                        nbytes,
                        endian_big,
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
                    srec_input_filter_checksum_negative::create
                    (
                        ifp,
                        address,
                        nbytes,
                        endian_little,
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
                    srec_input_filter_checksum_positive::create
                    (
                        ifp,
                        address,
                        nbytes,
                        endian_big,
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
                    srec_input_filter_checksum_positive::create
                    (
                        ifp,
                        address,
                        nbytes,
                        endian_little,
                        width
                    );
            }
            break;

        case token_offset:
            {
                token_next();
                unsigned long amount = get_number("--offset");
                ifp = srec_input_filter_offset::create(ifp, amount);
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
                    srec_input_filter_split::create
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
                    srec_input_filter_unfill::create
                    (
                        ifp,
                        fill_value,
                        fill_minimum
                    );
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
                    srec_input_filter_unsplit::create
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
