/*
 *	srecord - manipulate eprom load files
 *	Copyright (C) 1998, 1999, 2000, 2001 Peter Miller;
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

class srec_arglex: public arglex
{
public:
	enum
	{
		token_and = arglex::token_MAX,
		token_ascii_hex,
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
		token_paren_begin,
		token_paren_end,
		token_split,
		token_tektronix,
		token_tektronix_extended,
		token_ti_tagged,
		token_unsplit,
		token_vhdl,
		token_within,
		token_xor,
		token_MAX
	};

	~srec_arglex();
	srec_arglex(int, char **);

	class srec_input *get_input();
	class srec_output *get_output();

private:
	srec_arglex();
	srec_arglex(const srec_arglex &);
	srec_arglex &operator=(const srec_arglex &);
	class interval get_interval_inner(const char *);
	class interval get_interval(const char *);
	void get_address(const char *, unsigned long &);
	void get_address_and_nbytes(const char *, unsigned long &, int &);
	void get_address_nbytes_width(const char *, unsigned long &, int &, int &);
	bool stdin_used;
	bool stdout_used;
};

#endif /* INCLUDE_SREC_ARGLEX_H */
