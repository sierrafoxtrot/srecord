/*
 *	srecord - manipulate eprom load files
 *	Copyright (C) 2001 Peter Miller;
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
 * MANIFEST: functions to impliment the srec_arglex_output class
 */

#include <srec/arglex.h>


#include <srec/output/file/ascii_hex.h>
#include <srec/output/file/atmel_generic.h>
#include <srec/output/file/binary.h>
#include <srec/output/file/c.h>
#include <srec/output/file/vhdl.h>
#include <srec/output/file/intel.h>
#include <srec/output/file/mos_tech.h>
#include <srec/output/file/spasm.h>
#include <srec/output/file/srecord.h>
#include <srec/output/file/tektronix.h>
#include <srec/output/file/tektronix_extended.h>
#include <srec/output/file/ti_tagged.h>
#include <srec/output/file/wilson.h>


srec_output *
srec_arglex::get_output()
{
	/*
	 * skip the -output token
	 */
	if (token_cur() == token_output)
		token_next();

	/*
	 * determine the file name
	 */
	const char *fn = "-";
	switch (token_cur())
	{
	case token_stdio:
		token_next();
		/* fall through... */

	default:
		if (stdout_used)
		{
			cerr << "the standard output may only be named once on the command line" << endl;
			exit(1);
		}
		stdout_used = true;
		break;

	case token_string:
		fn = value_string();
		token_next();
		break;
	}

	/*
	 * determine the file format
	 */
	srec_output *ofp;
	switch (token_cur())
	{
	case token_motorola:
		token_next();
		/* fall through... */

	default:
		ofp = new srec_output_file_srecord(fn);
		break;

	case token_ascii_hex:
		token_next();
		ofp = new srec_output_file_ascii_hex(fn);
		break;

	case token_atmel_generic_be:
		token_next();
		ofp = new srec_output_file_atmel_generic(fn, true);
		break;

	case token_atmel_generic_le:
		token_next();
		ofp = new srec_output_file_atmel_generic(fn, false);
		break;

	case token_intel:
		token_next();
		ofp = new srec_output_file_intel(fn);
		break;

	case token_mos_tech:
		token_next();
		ofp = new srec_output_file_mos_tech(fn);
		break;

	case token_spasm_be:
		token_next();
		ofp = new srec_output_file_spasm(fn, true);
		break;

	case token_spasm_le:
		token_next();
		ofp = new srec_output_file_spasm(fn, false);
		break;

	case token_tektronix:
		token_next();
		ofp = new srec_output_file_tektronix(fn);
		break;

	case token_tektronix_extended:
		token_next();
		ofp = new srec_output_file_tektronix_extended(fn);
		break;

	case token_ti_tagged:
		token_next();
		ofp = new srec_output_file_ti_tagged(fn);
		break;

	case token_binary:
		token_next();
		ofp = new srec_output_file_binary(fn);
		break;

	case token_vhdl:
		{
			const char *prefix = "eprom";
			int bytes_per_word = 1;
			if (token_next() == token_number)
			{
				bytes_per_word = value_number();
			}
			if (token_next() == token_string)
			{
				prefix = value_string();
				token_next();
			}
			ofp = new srec_output_file_vhdl(fn, bytes_per_word, prefix);
		}
		break;

	case token_c_array:
		{
			const char *prefix = "eprom";
			if (token_next() == token_string)
			{
				prefix = value_string();
				token_next();
			}
			ofp = new srec_output_file_c(fn, prefix);
		}
		break;

	case token_wilson:
		token_next();
		ofp = new srec_output_file_wilson(fn);
		break;
	}

	/*
	 * return the stream determined
	 */
	return ofp;
}
