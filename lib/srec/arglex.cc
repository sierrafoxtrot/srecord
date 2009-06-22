//
// srecord - manipulate eprom load files
// Copyright (C) 1998-2009 Peter Miller
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

#include <iostream>

#include <lib/srec/arglex.h>
#include <lib/srec/input/file.h>
#include <lib/srec/memory.h>


srec_arglex::srec_arglex(int argc, char **argv) :
    arglex(argc, argv),
    stdin_used(false),
    stdout_used(false),
    issue_sequence_warnings(-1)
{
    static const table_ty table[] =
    {
        { "(", token_paren_begin, },
        { ")", token_paren_end, },
        { "+", token_union, },
        { "-A430", token_a430, },
        { "-Absolute_Object_Module_Format", token_aomf, },
        { "-AND", token_and, },
        { "-Ascii_Hexadecimal", token_ascii_hex, },
        { "-Ascii_Space_Hexadecimal", token_ascii_hex, },
        { "-ASseMbler", token_assembler, },
        { "-Atmel_Generic_BigEndian", token_atmel_generic_be, },
        { "-Atmel_Generic_LittleEndian", token_atmel_generic_le, },
        { "-Atmel_Generic", token_atmel_generic_le, },
        { "-AUGment", token_crc16_augment },
        { "-BAsic", token_basic_data, },
        { "-Big_Endian_Adler_16", token_adler16_be, },
        { "-Big_Endian_Adler_32", token_adler32_be, },
        { "-Big_Endian_Adler_Checksum_16", token_adler16_be, },
        { "-Big_Endian_Adler_Checksum_32", token_adler32_be, },
        { "-Big_Endian_Checksum_BitNot", token_checksum_be_bitnot, },
        { "-Big_Endian_Checksum_Negative", token_checksum_be_negative, },
        { "-Big_Endian_Checksum_Positive", token_checksum_be_positive, },
        { "-Big_Endian_Checksum", token_checksum_be_bitnot, },
        { "-Big_Endian_Cyclic_Redundancy_Check_16", token_crc16_be, },
        { "-Big_Endian_Cyclic_Redundancy_Check_32", token_crc32_be, },
        { "-Big_Endian_Exclusive_Length", token_exclusive_length_be, },
        { "-Big_Endian_Exclusive_MAximum", token_exclusive_maximum_be, },
        { "-Big_Endian_Exclusive_MInimum",token_exclusive_minimum_be, },
        { "-Big_Endian_Fletchers_16", token_fletcher16_be, },
        { "-Big_Endian_Fletchers_32", token_fletcher32_be, },
        { "-Big_Endian_Fletchers_Checksum_16", token_fletcher16_be, },
        { "-Big_Endian_Fletchers_Checksum_32", token_fletcher32_be, },
        { "-Big_Endian_Length", token_length_be, },
        { "-Big_Endian_MAximum", token_maximum_be, },
        { "-Big_Endian_MInimum",token_minimum_be, },
        { "-BIG", token_big, },
        { "-BINary", token_binary, },
        { "-Bit_Reverse", token_bitrev },
        { "-B_Record", token_brecord, },
        { "-BROken", token_crc16_broken, },
        { "-Byte_Swap", token_byte_swap, },
        { "-C_Array", token_c_array, },
        { "-C_COMpressed", token_c_compressed, },
        { "-CL430", token_cl430, },
        { "-Comite_Consultatif_International_Telephonique_et_Telegraphique",
            token_crc16_ccitt,},
        { "-CONSTant", token_constant, },
        { "-COsmac", token_cosmac, },
        { "-CRop", token_crop, },
        { "-Cyclic_Redundancy_Check_16_XMODEM", token_crc16_xmodem,},
        { "-Dec_Binary", token_dec_binary, },
        { "-DECimal_STyle", token_style_hexadecimal_not, },
        { "-DIFference", token_minus, },
        { "-Disable_Sequence_Warnings", token_sequence_warnings_disable, },
        { "-Dot_STyle", token_style_dot, },
        { "-EEPROM", token_eeprom, },
        { "-Elektor_Monitor52", token_emon52, },
        { "-Enable_Sequence_Warnings", token_sequence_warnings_enable, },
        { "-Exclude", token_exclude, },
        { "-Exclusive_Length", token_exclusive_length, },
        { "-Exclusive_Maximum", token_exclusive_maximum, },
        { "-Exclusive_Minimum", token_exclusive_minimum, },
        { "-FAIrchild", token_fairchild, },
        { "-Fast_Load", token_fast_load, },
        { "-Fill", token_fill, },
        { "-Formatted_Binary", token_formatted_binary, },
        { "-FORTH", token_forth, },
        { "-Four_Packed_Code", token_four_packed_code, },
        { "-GENerate", token_generator },
        { "-GENerator", token_generator },
        { "-Gnu_CRypt", token_gcrypt }, // undocumented
        { "-GUess", token_guess, },
        { "-HAVal", token_haval, },
        { "-HEXadecimal_Dump", token_hexdump, },
        { "-HEXadecimal_STyle", token_style_hexadecimal, },
        { "-IGnore_Checksums", token_ignore_checksums, },
        { "-INCLude", token_include, },
        { "-INtel_HeXadecimal_16", token_intel16, },
        { "-Intel", token_intel, },
        { "-International_Telecommunication_Union", token_crc16_ccitt,},
        { "-INTERSection", token_intersection, },
        { "-Least_To_Most", token_crc16_least_to_most },
        { "-Length", token_length, },
        { "-Little_Endian_Adler_16", token_adler16_le, },
        { "-Little_Endian_Adler_32", token_adler32_le, },
        { "-Little_Endian_Checksum_Adler_16", token_adler16_le, },
        { "-Little_Endian_Checksum_Adler_32", token_adler32_le, },
        { "-Little_Endian_Checksum_BitNot", token_checksum_le_bitnot, },
        { "-Little_Endian_Checksum_Fletchers_16", token_fletcher16_le, },
        { "-Little_Endian_Checksum_Fletchers_32", token_fletcher32_le, },
        { "-Little_Endian_Checksum_Negative", token_checksum_le_negative, },
        { "-Little_Endian_Checksum_Positive", token_checksum_le_positive, },
        { "-Little_Endian_Checksum", token_checksum_le_bitnot, },
        { "-Little_Endian_Cyclic_Redundancy_Check_16", token_crc16_le,},
        { "-Little_Endian_Cyclic_Redundancy_Check_32", token_crc32_le,},
        { "-Little_Endian_Exclusive_Length", token_exclusive_length_le, },
        { "-Little_Endian_Exclusive_MAximum", token_exclusive_maximum_le, },
        { "-Little_Endian_Exclusive_MInimum", token_exclusive_minimum_le, },
        { "-Little_Endian_Fletchers_16", token_fletcher16_le, },
        { "-Little_Endian_Fletchers_32", token_fletcher32_le, },
        { "-Little_Endian_Length", token_length_le, },
        { "-Little_Endian_MAximum", token_maximum_le, },
        { "-Little_Endian_MInimum", token_minimum_le, },
        { "-MAximum-Address", token_maximum_address, },
        { "-MAximum", token_maximum_address, },        // deprecated, put second
        { "-Memory_Initialization_File", token_memory_initialization_file },
        { "-Message_Digest_2", token_md2 },
        { "-Message_Digest_5", token_md5 },
        { "-MInimum-Address", token_minimum_address, },
        { "-MInimum", token_minimum_address, },        // deprecated, put second
        { "-MINUs", token_minus, },
        { "-MOS_Technologies", token_mos_tech, },
        { "-Most_To_Least", token_crc16_most_to_least },
        { "-Motorola", token_motorola, },
        { "-MULTiple", token_multiple, },
        { "-Needham_Hexadecimal", token_needham_hex, },
        { "-Not_AUGment", token_crc16_augment_not },
        { "-Not_CONSTant", token_constant_not, },
        { "-Not_INCLude", token_include_not, },
        { "-NOT", token_not, },
        { "-OFfset", token_offset, },
        { "-Ohio_Scientific65v", token_ohio_scientific, },
        { "-Ohio_Scientific", token_ohio_scientific, },
        { "-OR", token_or, },
        { "-Output", token_output, },
        { "-Output_Words", token_output_word, },
        { "-OVer", token_over, },
        { "-RAM", token_ram, },
        { "-Random_Fill", token_random_fill, },
        { "-RANDom", token_random, },
        { "-RAnge_PADding", token_range_padding, },
        { "-RAw", token_binary, },
        { "-REPeat_Data", token_repeat_data, },
        { "-REPeat_String", token_repeat_string, },
        { "-Ripe_Message_Digest_160", token_rmd160 },
        { "-Round_Down", token_round_down, },
        { "-Round_Nearest", token_round_nearest, },
        { "-Round_Off", token_round_nearest, },
        { "-Round", token_round_nearest, },
        { "-Round_Up", token_round_up, },
        { "-Section_STyle", token_style_section, },
        { "-Secure_Hash_Algorithm_1", token_sha1 },
        { "-Secure_Hash_Algorithm_224", token_sha224 },
        { "-Secure_Hash_Algorithm_256", token_sha256 },
        { "-Secure_Hash_Algorithm_384", token_sha384 },
        { "-Secure_Hash_Algorithm_512", token_sha512 },
        { "-SIGnetics", token_signetics, },
        { "-SPAsm_BigEndian", token_spasm_be, },
        { "-SPAsm_LittleEndian", token_spasm_le, },
        { "-SPAsm", token_spasm_be, }, // is this right?
        { "-SPEctrum", token_spectrum, },
        { "-SPlit", token_split, },
        { "-S_record", token_motorola, },
        { "-STewie", token_stewie, },
        { "-SUBtract", token_minus, },
        { "-Tektronix_Extended", token_tektronix_extended, },
        { "-Tektronix", token_tektronix, },
        { "-Texas_Instruments_Tagged_16", token_ti_tagged_16, },
        { "-Texas_Instruments_Tagged", token_ti_tagged, },
        { "-Texas_Instruments_TeXT", token_ti_txt, },
        { "-TIGer", token_tiger },
        { "-Un_Fill", token_unfill, },
        { "-UNIon", token_union, },
        { "-Un_SPlit", token_unsplit, },
        { "-VHdl", token_vhdl, },
        { "-VMem", token_vmem, },
        { "-WHIrlpool", token_whirlpool },
        { "-WILson", token_wilson, },
        { "-Within", token_within, },
        { "-X_MODEM", token_crc16_xmodem,},
        { "-XOR", token_xor, },
        { "[", token_paren_begin, },
        { "]", token_paren_end, },
        { "{", token_paren_begin, },
        { "}", token_paren_end, },
        ARGLEX_END_MARKER
    };

    table_set(table);
    deprecated_option("-MAximum");
    deprecated_option("-MInimum");
}


srec_arglex::~srec_arglex()
{
}


bool
srec_arglex::can_get_number()
    const
{
    switch (token_cur())
    {
    case token_paren_begin:
    case token_number:
    case token_minimum_address:
    case token_maximum_address:
    case token_length:
        return true;

    default:
        return false;
    }
}


void
srec_arglex::get_address(const char *name, unsigned long &address)
{
    if (!can_get_number())
    {
        fatal_error("the %s filter requires an address", name);
        // NOTREACHED
    }
    address = get_number("address");
}


void
srec_arglex::get_address_and_nbytes(const char *name, unsigned long &address,
    int &nbytes)
{
    if (!can_get_number())
    {
        fatal_error("the %s filter requires an address and a byte count", name);
        // NOTREACHED
    }
    address = get_number("address");
    nbytes = 4;
    if (can_get_number())
    {
        nbytes = get_number("byte count", 1, 8);
    }
    if ((long long)address + nbytes > (1LL << 32))
    {
        fatal_error
        (
            "the %s address (0x%8.8lX) and byte count (%d) may not span the "
                "top of memory",
            name,
            address,
            nbytes
        );
        // NOTREACHED
    }
}


void
srec_arglex::get_address_nbytes_width(const char *name, unsigned long &address,
    int &nbytes, int &width)
{
    address = get_number("address");
    nbytes = 4;
    width = 1;
    if (can_get_number())
    {
        nbytes = get_number("byte count", 1, 8);
        if (can_get_number())
        {
            width = get_number("width", 1, nbytes);
        }
    }
    if ((long long)address + nbytes > (1LL << 32))
    {
        fatal_error
        (
            "the %s address (0x%8.8lX) and byte count (%d) may not span the "
                "top of memory",
            name,
            address,
            nbytes
        );
        // NOTREACHED
    }
}


void
srec_arglex::default_command_line_processing()
{
    switch (token_cur())
    {
    default:
        arglex::default_command_line_processing();
        break;

    case srec_arglex::token_ignore_checksums:
        srec_input_file::ignore_all_checksums();
        token_next();
        break;

    case srec_arglex::token_sequence_warnings_enable:
        issue_sequence_warnings = 1;
        token_next();
        break;

    case srec_arglex::token_sequence_warnings_disable:
        issue_sequence_warnings = 0;
        token_next();
        break;

    case srec_arglex::token_multiple:
        srec_memory::allow_overwriting();
        token_next();
        break;
    }
}
