/*
 *	srecord - manipulate eprom load files
 *	Copyright (C) 1998 Peter Miller;
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
 *	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * MANIFEST: functions to impliment the srec_arglex class
 */

#pragma implementation

#include <iostream.h>

#include <srec/arglex.h>
#include <srec/input/file/binary.h>
#include <srec/input/file/intel.h>
#include <srec/input/file/srecord.h>
#include <srec/input/filter/checksum.h>
#include <srec/input/filter/crop.h>
#include <srec/input/filter/fill.h>
#include <srec/input/filter/length.h>
#include <srec/input/filter/offset.h>
#include <srec/input/interval.h>
#include <srec/output/file/binary.h>
#include <srec/output/file/c.h>
#include <srec/output/file/intel.h>
#include <srec/output/file/srecord.h>


srec_arglex::srec_arglex()
{
	/* bug */
}


srec_arglex::srec_arglex(int argc, char **argv)
	: arglex(argc, argv), stdin_used(false), stdout_used(false)
{
	static table_ty table[] =
	{
		{ "(",		token_paren_begin,	},
		{ ")",		token_paren_end,	},
		{ "-Big_Endian_Checksum", token_checksum_be, },
		{ "-Big_Endian_Length",	token_length_be, },
		{ "-BINary",	token_binary,	},
		{ "-C_Array",	token_c_array,	},
		{ "-CRop",	token_crop,	},
		{ "-Exclude",	token_exclude,	},
		{ "-Fill",	token_fill,	},
		{ "-Intel",	token_intel,	},
		{ "-Little_Endian_Checksum", token_checksum_le, },
		{ "-Little_Endian_Length", token_length_le, },
		{ "-Motorola",	token_motorola,	},
		{ "-OFfset",	token_offset,	},
		{ "-Output",	token_output,	},
		{ "-OVer",	token_over,	},
		{ "-RAw",	token_binary,	},
		{ "-S_record",	token_motorola,	},
		{ "-Within",	token_within,	},
		ARGLEX_END_MARKER
	};

	table_set(table);
}


srec_arglex::srec_arglex(const srec_arglex &)
{
	/* bug */
}


srec_arglex &
srec_arglex::operator=(const srec_arglex &)
{
	/* bug */
	return *this;
}


srec_arglex::~srec_arglex()
{
	/* bug */
}


interval
srec_arglex::get_interval_inner(const char *name)
{
	switch (token_cur())
	{
	default:
		cerr << "the " << name
			<< " range requires two numeric arguments" << endl;
		exit(1);

	case token_paren_begin:
		{
			token_next();
			interval retval = get_interval(name);
			if (token_cur() != token_paren_end)
			{
				cerr << "``)'' expected" << endl;
				exit(1);
			}
			token_next();
			return retval;
		}

	case token_number:
		{
			unsigned long n1 = value_number();
			unsigned long n2 = 0;
			if (token_next() == token_number)
			{
				n2 = value_number();
				token_next();
			}
			if (n2 && n1 >= n2)
			{
				cerr << "the " << name << " range " << n1
					<< ".." << n2 << " is invalid" << endl;
				exit(1);
			}
			return interval(n1, n2);
		}

	case token_within:
		{
			token_next();
			srec_input *ifp = get_input();
			interval over = srec_input_interval(ifp);
			delete ifp;
			return over;
		}

	case token_over:
		{
			token_next();
			srec_input *ifp = get_input();
			interval over = srec_input_interval(ifp);
			delete ifp;
			return interval(over.get_lowest(), over.get_highest());
		}
	}
}


interval
srec_arglex::get_interval(const char *name)
{
	interval range;
	for (;;)
	{
		range += get_interval_inner(name);
		switch (token_cur())
		{
		case token_number:
		case token_within:
		case token_over:
		case token_paren_begin:
			continue;

		default:
			break;
		}
		break;
	}
	return range;
}


void
srec_arglex::get_address_and_nbytes(const char *name, unsigned long &address,
	int &nbytes)
{
	if (token_next() != token_number)
	{
		cerr << "the " << name
			<< " filter requires an address and a byte count"
			<< endl;
		exit(1);
	}
	address = value_number();
	nbytes = 4;
	if (token_next() == token_number)
	{
		nbytes = value_number(); 
		token_next();
		if (nbytes < 1 || nbytes > 8)
		{
			cerr << "the " << name << " byte count " << nbytes
				<< " is out of range (1..8)"
				<< endl;
			exit(1);
		}
	}
	if ((long long)address + nbytes > (1LL << 32))
	{
		cerr << "the " << name << " address (" << address
			<< ") and byte count (" << nbytes
			<< ") may not span the top of memory" << endl;
		exit(1);
	}
}


srec_input *
srec_arglex::get_input()
{
	/*
	 * determine the file name
	 */
	const char *fn = "-";
	switch (token_cur())
	{
	default:
		assert(0);
		break;

	case token_string:
		fn = value_string();
		token_next();
		break;

	case token_stdio:
		if (stdin_used)
		{
			cerr << "the standard input may only be named once on the command line" << endl;
			exit(1);
		}
		stdin_used = true;
		token_next();
		break;
	}

	/*
	 * determine the file format
	 * and open the input file
	 */
	srec_input *ifp;
	switch (token_cur())
	{
	case token_motorola:
		token_next();
		/* fall through... */

	default:
		ifp = new srec_input_file_srecord(fn);
		break;

	case token_intel:
		token_next();
		ifp = new srec_input_file_intel(fn);
		break;

	case token_binary:
		token_next();
		ifp = new srec_input_file_binary(fn);
		break;
	}

	/*
	 * apply any filters specified
	 */
	for (;;)
	{
		switch (token_cur())
		{
		case token_crop:
			token_next();
			ifp = new srec_input_filter_crop(ifp,
				get_interval("-Crop"));
			continue;

		case token_exclude:
			token_next();
			ifp = new srec_input_filter_crop(ifp,
				-get_interval("-Exclude"));
			continue;

		case token_fill:
			{
				if (token_next() != token_number) 
				{
					cerr <<
					"the -fill filter requires a fill value"
						<< endl;
					exit(1);
				}
				int filler = value_number();
				if (filler < 0 || filler >= 256)
				{
					cerr << "fill value " << filler
						<< " out of range (0..255)"
						<< endl;
					exit(1);
				}
				token_next();
				interval range = get_interval("-Fill");
				ifp = new srec_input_filter_fill(ifp, filler, range);
			}
			continue;

		case token_length_be:
			{
				unsigned long address;
				int nbytes;
				get_address_and_nbytes
				(
					"-Big_Endian_Length",
					address,
					nbytes
				);
				ifp =
					new srec_input_filter_length
					(
						ifp,
						address,
						nbytes,
						0
					);
			}
			continue;

		case token_length_le:
			{
				unsigned long address;
				int nbytes;
				get_address_and_nbytes
				(
					"-Little_Endian_Length",
					address,
					nbytes
				);
				ifp =
					new srec_input_filter_length
					(
						ifp,
						address,
						nbytes,
						1
					);
			}
			continue;

		case token_checksum_be:
			{
				unsigned long address;
				int nbytes;
				get_address_and_nbytes
				(
					"-Big_Endian_Checksum",
					address,
					nbytes
				);
				ifp =
					new srec_input_filter_checksum
					(
						ifp,
						address,
						nbytes,
						0
					);
			}
			continue;

		case token_checksum_le:
			{
				unsigned long address;
				int nbytes;
				get_address_and_nbytes
				(
					"-Little_Endian_Checksum",
					address,
					nbytes
				);
				ifp =
					new srec_input_filter_checksum
					(
						ifp,
						address,
						nbytes,
						1
					);
			}
			continue;

		case token_offset:
			if (token_next() != token_number)
			{
				cerr <<
				"the -offset filter requires a numeric argument"
					<< endl;
				exit(1);
			}
			ifp = new srec_input_filter_offset(ifp, value_number());
			token_next();
			continue;

		default:
			break;
		}
		break;
	}

	/*
	 * return the input stream determined
	 */
	return ifp;
}


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
	default:
		break;

	case token_stdio:
		if (stdout_used)
		{
			cerr << "the standard output may only be named once on the command line" << endl;
			exit(1);
		}
		stdout_used = true;
		token_next();
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

	case token_intel:
		token_next();
		ofp = new srec_output_file_intel(fn);
		break;

	case token_binary:
		token_next();
		ofp = new srec_output_file_binary(fn);
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
	}

	/*
	 * return the stream determined
	 */
	return ofp;
}
