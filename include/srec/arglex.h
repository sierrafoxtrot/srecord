/*
 *	srecord - manipulate eprom load files
 *	Copyright (C) 1998-2001 Peter Miller;
 *	All rights reserved.
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program; if not, write to the Free Software
 *	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111, USA.
 *
 * MANIFEST: interface definition for lib/srec/arglex.cc
 */

#ifndef INCLUDE_SREC_ARGLEX_H
#define INCLUDE_SREC_ARGLEX_H

#pragma interface "srec_arglex"

#include <arglex.h>

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
		token_ascii_hex,
		token_atmel_generic_be,
		token_atmel_generic_le,
		token_binary,
		token_byte_swap,
		token_c_array,
		token_crc16_be,
		token_crc16_le,
		token_crc32_be,
		token_crc32_le,
		token_checksum_be_bitnot,
		token_checksum_be_negative,
		token_checksum_be_positive,
		token_checksum_le_bitnot,
		token_checksum_le_negative,
		token_checksum_le_positive,
		token_crop,
		token_wilson,
		token_exclude,
		token_fill,
		token_guess,
		token_intel,
		token_length_be,
		token_length_le,
		token_maximum_be,
		token_maximum_le,
		token_minimum_be,
		token_minimum_le,
		token_mos_tech,
		token_motorola,
		token_multiple,
		token_not,
		token_offset,
		token_or,
		token_output,
		token_over,
		token_four_packed_code,
		token_paren_begin,
		token_paren_end,
		token_signetics,
		token_spasm_be,
		token_spasm_le,
		token_split,
		token_tektronix,
		token_tektronix_extended,
		token_ti_tagged,
		token_unfill,
		token_unsplit,
		token_vhdl,
		token_within,
		token_xor,
		token_MAX
	};

	/**
	  * The destructor.
	  */
	~srec_arglex();

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

#endif /* INCLUDE_SREC_ARGLEX_H */
