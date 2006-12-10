//
//	srecord - manipulate eprom load files
//	Copyright (C) 1998-2006 Peter Miller;
//	All rights reserved.
//
//	This program is free software; you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published by
//	the Free Software Foundation; either version 2 of the License, or
//	(at your option) any later version.
//
//	This program is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.
//
//	You should have received a copy of the GNU General Public License
//	along with this program; if not, write to the Free Software
//	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111, USA.
//
// MANIFEST: interface definition for lib/srec/arglex.cc
//

#ifndef INCLUDE_SREC_ARGLEX_H
#define INCLUDE_SREC_ARGLEX_H


#include <lib/arglex.h>

/**
  * The srec_arglex is used to parse command line with srec-specific
  * arguments.
  */
class srec_arglex:
    public arglex
{
public:
    enum
    {
	token_and = arglex::token_MAX,
	token_aomf,
	token_ascii_hex,
	token_assembler,
	token_atmel_generic_be,
	token_atmel_generic_le,
	token_basic_data,
	token_binary,
	token_byte_swap,
	token_c_array,
	token_checksum_be_bitnot,
	token_checksum_be_negative,
	token_checksum_be_positive,
	token_checksum_le_bitnot,
	token_checksum_le_negative,
	token_checksum_le_positive,
	token_constant,
	token_constant_not,
	token_cosmac,
	token_crc16_be,
	token_crc16_le,
	token_crc16_xmodem,
	token_crc32_be,
	token_crc32_le,
	token_crop,
	token_dec_binary,
	token_emon52,
	token_exclude,
	token_fairchild,
	token_fast_load,
	token_fill,
	token_formatted_binary,
	token_four_packed_code,
	token_guess,
	token_ignore_checksums,
	token_include,
	token_include_not,
	token_intel,
	token_intel16,
	token_length,
	token_length_be,
	token_length_le,
	token_maximum,
	token_maximum_be,
	token_maximum_le,
	token_minimum,
	token_minimum_be,
	token_minimum_le,
	token_mos_tech,
	token_motorola,
	token_multiple,
	token_needham_hex,
	token_not,
	token_offset,
	token_ohio_scientific,
	token_or,
	token_output,
	token_over,
	token_paren_begin,
	token_paren_end,
	token_random_fill,
	token_round_down,
	token_round_nearest,
	token_round_up,
	token_signetics,
	token_spasm_be,
	token_spasm_le,
	token_spectrum,
	token_split,
	token_stewie,
	token_tektronix,
	token_tektronix_extended,
	token_ti_tagged,
	token_unfill,
	token_unsplit,
	token_vhdl,
	token_vmem,
	token_wilson,
	token_within,
	token_xor,
	token_MAX
    };

    /**
      * The destructor.
      */
    virtual ~srec_arglex();

    /**
      * The constructor.  Pass the argc and argv as given to main;
      * there is not need to change the values at all.
      */
    srec_arglex(int argc, char **argv);

    /**
      * The get_input method is used to parse an input specification
      * (filename, file format, filters, everything) from the
      * command line.
      *
      * If the parse is unsuccessful (is not present on command
      * line) a fatal error will be issued and the method call will
      * not return.
      */
    class srec_input *get_input();

    /**
      * The get_output method is used to parse an output specification
      * (filename and file format) from the command line.
      *
      * If the parse is unsuccessful (is not present on command
      * line) a fatal error will be issued and the method call will
      * not return.
      */
    class srec_output *get_output();

    /**
      * The get_number method is used to parse a numeric value fromthe
      * command line.
      */
    unsigned long get_number(const char *caption);

    /**
      * The can_get_number method is used to determine if it is possible
      * to parse a number from the next token on the command line.
      */
    bool can_get_number() const;

private:
    /**
      * The get_interval_inner method is used to parse a single
      * interval from the command line (usually, a pair of number
      * representing the [lower, upper) bounds, but it could be
      * -over or -within, too).
      *
      * This method should only every be called by the get_interval
      * method.
      *
      * If the parse is unsuccessful (is not present on command
      * line) a fatal error will be issued and the method call will
      * not return.
      */
    class interval get_interval_inner(const char *err_msg_caption);

    /**
      * The get_interval method is used to parse an interval
      * set form the command line.	Iyt consists of as many
      * get_interval_inner()s as possible.
      *
      * Used by the get_input method to parse the address intervals
      * used by various filters.
      *
      * If the parse is unsuccessful (is not present on command
      * line) a fatal error will be issued and the method call will
      * not return.
      */
    class interval get_interval(const char *err_msg_caption);

    /**
      * The get_address method is used to parse an address from the
      * command line.
      *
      * If the parse is unsuccessful (is not present on command
      * line) a fatal error will be issued and the method call will
      * not return.
      */
    void get_address(const char *err_msg_caption, unsigned long &addr);

    /**
      * The get_address_and_nbytes method is used to parse an address
      * and a byte count from the command line.
      *
      * If the parse is unsuccessful (is not present on command
      * line) a fatal error will be issued and the method call will
      * not return.
      */
    void get_address_and_nbytes(const char *err_msg_caption,
	    unsigned long &addr, int &nbytes);

    /**
      * The get_address_nbytes_width method is used to parse an address
      * a byte count and a width from the command line.
      *
      * If the parse is unsuccessful (is not present on command
      * line) a fatal error will be issued and the method call will
      * not return.
      */
    void get_address_nbytes_width(const char *err_msg_caption,
	unsigned long &addr, int &nbytes, int &width);

    /**
      * The stdin_used instance variable is used to remember whether
      * or not the standard input has been used by a filter, yet.
      * Only one use of the standard input may be made; the second
      * use will result in a fatal error.
      */
    bool stdin_used;

    /**
      * The stdout_used instance variable is used to remember whether
      * or not the standard output has been used by a filter, yet.
      * Only one use of the standard output may be made; the second
      * use will result in a fatal error.
      */
    bool stdout_used;

    /**
      * The default constructor.  Do not use.
      */
    srec_arglex();

    /**
      * The copy constructor.  Do not use.
      */
    srec_arglex(const srec_arglex &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_arglex &operator=(const srec_arglex &);
};

#endif // INCLUDE_SREC_ARGLEX_H
