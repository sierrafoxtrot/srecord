//
//	srecord - manipulate eprom load files
//	Copyright (C) 1998-2003 Peter Miller;
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
// MANIFEST: functions to impliment the srec_arglex class
//

#pragma implementation "srec_arglex"

#include <iostream>
using namespace std;

#include <srec/arglex.h>
#include <srec/input/file/ascii_hex.h>
#include <srec/input/file/atmel_generic.h>
#include <srec/input/file/binary.h>
#include <srec/input/file/cosmac.h>
#include <srec/input/file/dec_binary.h>
#include <srec/input/file/emon52.h>
#include <srec/input/file/fairchild.h>
#include <srec/input/file/fastload.h>
#include <srec/input/file/formatted_binary.h>
#include <srec/input/file/four_packed_code.h>
#include <srec/input/file/guess.h>
#include <srec/input/file/intel.h>
#include <srec/input/file/intel16.h>
#include <srec/input/file/mos_tech.h>
#include <srec/input/file/needham.h>
#include <srec/input/file/os65v.h>
#include <srec/input/file/signetics.h>
#include <srec/input/file/spasm.h>
#include <srec/input/file/spectrum.h>
#include <srec/input/file/srecord.h>
#include <srec/input/file/tektronix_extended.h>
#include <srec/input/file/tektronix.h>
#include <srec/input/file/ti_tagged.h>
#include <srec/input/file/vmem.h>
#include <srec/input/file/wilson.h>
#include <srec/input/filter/and.h>
#include <srec/input/filter/byte_swap.h>
#include <srec/input/filter/checksum/bitnot.h>
#include <srec/input/filter/checksum/negative.h>
#include <srec/input/filter/checksum/positive.h>
#include <srec/input/filter/crc16.h>
#include <srec/input/filter/crc32.h>
#include <srec/input/filter/crop.h>
#include <srec/input/filter/fill.h>
#include <srec/input/filter/length.h>
#include <srec/input/filter/maximum.h>
#include <srec/input/filter/minimum.h>
#include <srec/input/filter/not.h>
#include <srec/input/filter/offset.h>
#include <srec/input/filter/or.h>
#include <srec/input/filter/split.h>
#include <srec/input/filter/unfill.h>
#include <srec/input/filter/unsplit.h>
#include <srec/input/filter/xor.h>
#include <srec/input/interval.h>


srec_arglex::srec_arglex(int argc, char **argv) :
    arglex(argc, argv),
    stdin_used(false),
    stdout_used(false)
{
    static const table_ty table[] =
    {
	{ "(",		token_paren_begin,	},
	{ ")",		token_paren_end,	},
	{ "-AND",	token_and,		},
	{ "-Ascii_Hexadecimal",	token_ascii_hex, },
	{ "-Ascii_Space_Hexadecimal", token_ascii_hex, },
	{ "-ASM",	token_asm_db,		},
	{ "-Atmel_Generic",	token_atmel_generic_le,	 },
	{ "-Atmel_Generic_BigEndian", token_atmel_generic_be, },
	{ "-Atmel_Generic_LittleEndian", token_atmel_generic_le, },
	{ "-BAsic",	token_basic_data,	},
	{ "-Big_Endian_Checksum", token_checksum_be_bitnot, },
	{ "-Big_Endian_Checksum_BitNot", token_checksum_be_bitnot, },
	{ "-Big_Endian_Checksum_Negative", token_checksum_be_negative, },
	{ "-Big_Endian_Checksum_Positive", token_checksum_be_positive, },
	{ "-Big_Endian_Length",	token_length_be, },
	{ "-Big_Endian_MAximum", token_maximum_be, },
	{ "-Big_Endian_MInimum",token_minimum_be, },
	{ "-BINary",	token_binary,		},
	{ "-Byte_Swap",	token_byte_swap,	},
	{ "-C_Array",	token_c_array,		},
	{ "-COsmac",	token_cosmac,		},
	{ "-CRop",	token_crop,		},
	{ "-Big_Endian_Cyclic_Redundancy_Check_16", token_crc16_be, },
	{ "-Little_Endian_Cyclic_Redundancy_Check_16", token_crc16_le,},
	{ "-Big_Endian_Cyclic_Redundancy_Check_32", token_crc32_be, },
	{ "-Little_Endian_Cyclic_Redundancy_Check_32", token_crc32_le,},
	{ "-Dec_Binary",	token_dec_binary, },
	{ "-Elektor_Monitor52",	token_emon52,	},
	{ "-Exclude",	token_exclude,		},
	{ "-FAIrchild",	token_fairchild,	},
	{ "-Fast_Load",	token_fast_load,	},
	{ "-Formatted_Binary",	token_formatted_binary,	},
	{ "-Fill",	token_fill,		},
	{ "-Four_Packed_Code", token_four_packed_code, },
	{ "-GUess",	token_guess,		},
	{ "-Intel",	token_intel,		},
	{ "-INtel_HeXadecimal_16", token_intel16, },
	{ "-Little_Endian_Checksum", token_checksum_le_bitnot, },
	{ "-Little_Endian_Checksum_BitNot", token_checksum_le_bitnot, },
	{ "-Little_Endian_Checksum_Negative", token_checksum_le_negative, },
	{ "-Little_Endian_Checksum_Positive", token_checksum_le_positive, },
	{ "-Little_Endian_Length", token_length_le, },
	{ "-Little_Endian_MAximum", token_maximum_le, },
	{ "-Little_Endian_MInimum", token_minimum_le, },
	{ "-Length",    token_length, },
	{ "-MAximum",   token_maximum, },
	{ "-MInimum",   token_minimum, },
	{ "-MOS_Technologies", token_mos_tech,	},
	{ "-Motorola",	token_motorola,		},
	{ "-MULTiple",	token_multiple,		},
	{ "-Needham_Hexadecimal", token_needham_hex, },
	{ "-NOT",	token_not,		},
	{ "-OFfset",	token_offset,		},
	{ "-Ohio_Scientific", token_ohio_scientific, },
	{ "-Ohio_Scientific65v", token_ohio_scientific, },
	{ "-OR",	token_or,		},
	{ "-Output",	token_output,		},
	{ "-OVer",	token_over,		},
	{ "-RAw",	token_binary,		},
	{ "-Round_Up",	token_round_up,		},
	{ "-Round_Down", token_round_down,	},
	{ "-Round",	token_round_nearest,	},
	{ "-Round_Off",	token_round_nearest,	},
	{ "-Round_Nearest", token_round_nearest, },
	{ "-SIGnetics",	token_signetics,	},
	{ "-SPAsm",	token_spasm_be,		}, // is this right?
	{ "-SPAsm_BigEndian", token_spasm_be,	},
	{ "-SPAsm_LittleEndian", token_spasm_le, },
	{ "-SPEctrum",  token_spectrum,		},
	{ "-SPlit",	token_split,		},
	{ "-S_record",	token_motorola,		},
	{ "-Tektronix",	token_tektronix,	},
	{ "-Tektronix_Extended", token_tektronix_extended, },
	{ "-Texas_Instruments_Tagged", token_ti_tagged, },
	{ "-Un_Fill",	token_unfill,		},
	{ "-Un_SPlit",	token_unsplit,		},
	{ "-VHdl",	token_vhdl,		},
	{ "-VMem",	token_vmem,		},
	{ "-WILson",	token_wilson,		},
	{ "-Within",	token_within,		},
	{ "-XOR",	token_xor,		},
	ARGLEX_END_MARKER
    };

    table_set(table);
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
    case token_number:
    case token_minimum:
    case token_maximum:
    case token_length:
	return true;

    default:
	return false;
    }
}


unsigned long
srec_arglex::get_number(const char *caption)
{
    unsigned long value;
    unsigned long multiple;
    srec_input *ifp;
    interval over;

    switch (token_cur())
    {
    case token_number:
	value = value_number();
	token_next();
	return value;

    case token_minimum:
	token_next();
	ifp = get_input();
	over = srec_input_interval(ifp);
	delete ifp;
	value = over.get_lowest();
	break;

    case token_maximum:
	token_next();
	ifp = get_input();
	over = srec_input_interval(ifp);
	delete ifp;
	value = over.get_highest();
	break;

    case token_length:
	token_next();
	ifp = get_input();
	over = srec_input_interval(ifp);
	delete ifp;
	value = (over.get_highest() - over.get_lowest());
	break;

    default:
	cerr << "number expected for " << caption << endl;
	exit(1);
	return 0;
    }
    switch (token_cur())
    {
    case token_round_down:
	token_next();
	multiple = get_number("-round-down");
	value /= multiple;
	value *= multiple;
	break;

    case token_round_up:
	token_next();
	multiple = get_number("-round-up");
	value = (value + multiple - 1) / multiple;
	value *= multiple;
	break;

    case token_round_nearest:
	token_next();
	multiple = get_number("-round-nearest");
	value = (value + multiple / 2) / multiple;
	value *= multiple;
	break;
    }
    return value;
}


interval
srec_arglex::get_interval_inner(const char *name)
{
    switch (token_cur())
    {
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

    default:
	{
	    if (!can_get_number())
	    {
		cerr << "the " << name
		    << " range requires two numeric arguments" << endl;
		exit(1);
	    }
	    unsigned long n1 = get_number("address range minimum");
	    unsigned long n2 = 0;
	    if (can_get_number())
		    n2 = get_number("address range maximum");
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
srec_arglex::get_address(const char *name, unsigned long &address)
{
    if (!can_get_number())
    {
	cerr << "the " << name << " filter requires an address" << endl;
	exit(1);
    }
    address = get_number("address");
}


void
srec_arglex::get_address_and_nbytes(const char *name, unsigned long &address,
    int &nbytes)
{
    if (!can_get_number())
    {
	cerr << "the " << name
    	    << " filter requires an address and a byte count"
    	    << endl;
	exit(1);
    }
    address = get_number("address");
    nbytes = 4;
    if (can_get_number())
    {
	nbytes = get_number("byte count");
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


void
srec_arglex::get_address_nbytes_width(const char *name, unsigned long &address,
    int &nbytes, int &width)
{
    if (!can_get_number())
    {
	cerr << "the " << name
    	    << " filter requires an address and a byte count"
    	    << endl;
	exit(1);
    }
    address = get_number("address");
    nbytes = 4;
    width = 1;
    if (can_get_number())
    {
	nbytes = get_number("byte count");
	if (nbytes < 1 || nbytes > 8)
	{
	    cerr << "the " << name << " byte count " << nbytes
	       	<< " is out of range (1..8)"
	       	<< endl;
	    exit(1);
	}
	if (can_get_number())
	{
	    width = get_number("width");
	    if (width < 1 || width > nbytes)
	    {
	       	cerr << "the " << name << " sum width "
		    << width << " is out of range (1.."
		    << nbytes << ")" << endl;
	       	exit(1);
	    }
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
    //
    // determine the file name
    //
    const char *fn = "-";
    switch (token_cur())
    {
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
	    cerr <<
		"the standard input may only be named once on the command line"
		<< endl;
	    exit(1);
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
    // apply any filters specified
    //
    for (;;)
    {
	switch (token_cur())
	{
	case token_byte_swap:
	    token_next();
	    ifp = new srec_input_filter_byte_swap(ifp);
	    continue;

	case token_not:
	    token_next();
	    ifp = new srec_input_filter_not(ifp);
	    continue;

	case token_crc16_be:
	    {
		token_next();
		unsigned long address;
		get_address("-Big_Endian_CRC16", address);
		ifp = new srec_input_filter_crc16(ifp, address, 0);
	    }
	    continue;

	case token_crc16_le:
	    {
		token_next();
		unsigned long address;
		get_address("-Little_Endian_CRC16", address);
		ifp = new srec_input_filter_crc16(ifp, address, 1);
	    }
	    continue;

	case token_crc32_be:
	    {
		token_next();
		unsigned long address;
		get_address("-Big_Endian_CRC32", address);
		ifp = new srec_input_filter_crc32(ifp, address, 0);
	    }
	    continue;

	case token_crc32_le:
	    {
		token_next();
		unsigned long address;
		get_address("-Little_Endian_CRC32", address);
		ifp = new srec_input_filter_crc32(ifp, address, 1);
	    }
	    continue;

	case token_crop:
	    token_next();
	    ifp = new srec_input_filter_crop(ifp,
		    get_interval("-Crop"));
	    continue;

	case token_exclude:
	    token_next();
	    ifp = new srec_input_filter_crop(ifp, -get_interval("-Exclude"));
	    continue;

	case token_fill:
	    {
		token_next();
		if (!can_get_number())
		{
		    cerr << "the -fill filter requires a fill value" << endl;
		    exit(1);
		}
		int filler = get_number("fill value");
		if (filler < 0 || filler >= 256)
		{
		    cerr << "fill value " << filler << " out of range (0..255)"
			<< endl;
		    exit(1);
		}
		interval range = get_interval("-Fill");
		ifp = new srec_input_filter_fill(ifp, filler, range);
	    }
	    continue;

	case token_and:
	    {
		token_next();
		if (!can_get_number())
		{
		    cerr << "the -and filter requires a fill value" << endl;
		    exit(1);
		}
		int filler = get_number("and value");
		if (filler < 0 || filler >= 256)
		{
		    cerr << "-and value " << filler << " out of range (0..255)"
			<< endl;
		    exit(1);
		}
		ifp = new srec_input_filter_and(ifp, filler);
	    }
	    continue;

	case token_xor:
	    {
		token_next();
		if (!can_get_number())
		{
		    cerr << "the -xor filter requires a fill value" << endl;
		    exit(1);
		}
		int filler = get_number("xor value");
		if (filler < 0 || filler >= 256)
		{
		    cerr << "-xor value " << filler << " out of range (0..255)"
			<< endl;
		    exit(1);
		}
		ifp = new srec_input_filter_xor(ifp, filler);
	    }
	    continue;

	case token_or:
	    {
		token_next();
		if (!can_get_number())
		{
		    cerr << "the -or filter requires a fill value" << endl;
		    exit(1);
		}
		int filler = get_number("or value");
		if (filler < 0 || filler >= 256)
		{
		    cerr << "-or value " << filler << " out of range (0..255)"
			<< endl;
		    exit(1);
		}
		ifp = new srec_input_filter_or(ifp, filler);
	    }
	    continue;

	case token_length:
	    cerr << "Use --big-endian-length or --little-endian-length" << endl;
	    exit(1);

	case token_length_be:
	    {
		token_next();
		unsigned long address;
		int nbytes;
		get_address_and_nbytes("-Big_Endian_Length", address, nbytes);
		ifp = new srec_input_filter_length(ifp, address, nbytes, 0);
	    }
	    continue;

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
	    continue;

	case token_maximum:
	    cerr << "Use --big-endian-maximum or --little-endian-maximum"
		<< endl;
	    exit(1);

	case token_maximum_be:
	    {
		token_next();
		unsigned long address;
		int nbytes;
		get_address_and_nbytes("-Big_Endian_MAximum", address, nbytes);
		ifp = new srec_input_filter_maximum(ifp, address, nbytes, 0);
	    }
	    continue;

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
	    continue;

	case token_minimum:
	    cerr << "Use --big-endian-minimum or --little-endian-minimum"
		<< endl;
	    exit(1);

	case token_minimum_be:
	    {
		token_next();
		unsigned long address;
		int nbytes;
		get_address_and_nbytes("-Big_Endian_MInimum", address, nbytes);
		ifp = new srec_input_filter_minimum(ifp, address, nbytes, 0);
	    }
	    continue;

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
	    continue;

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
	    continue;

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
	    continue;

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
	    continue;

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
	    continue;

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
	    continue;

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
	    continue;

	case token_offset:
	    {
		token_next();
		if (!can_get_number())
		{
		    cerr << "the -offset filter requires a numeric argument"
			<< endl;
		    exit(1);
		}
		unsigned long amount = get_number("address offset");
		ifp = new srec_input_filter_offset(ifp, amount);
	    }
	    continue;

	case token_split:
	    {
		token_next();
		if (!can_get_number())
		{
		    cerr << "the -split filter requires three numeric arguments"
			<< endl;
		    exit(1);
		}
		int split_modulus = get_number("split modulus");
		if (split_modulus < 2)
		{
		    cerr << "the -split modulus must be two or more" << endl;
		    exit(1);
		}
		int split_offset = 0;
		if (can_get_number())
		{
		    split_offset = get_number("split offset");
		    if (split_offset < 0 || split_offset >= split_modulus)
		    {
			cerr << "the -split offset must be 0.."
			    << (split_modulus - 1) << endl;
			exit(1);
		    }
		}
		int split_width = 1;
		if (can_get_number())
		{
		    split_width = get_number("split width");
		    if (split_width < 1 || split_width >= split_modulus)
		    {
			cerr << "the -split width must be 1.."
			    << (split_modulus - 1) << endl;
			exit(1);
		    }
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
	    continue;

	case token_unfill:
	    {
		token_next();
		if (!can_get_number())
		{
		    cerr << "the -unfill filter requires two numeric arguments"
			<< endl;
		    exit(1);
		}
		int fill_value = get_number("unfill value");
		if (fill_value < 0 || fill_value >= 256)
		{
		    cerr << "the -unfill value must be 0..255" << endl;
		    exit(1);
		}
		int fill_minimum = 1;
		if (can_get_number())
		{
		    fill_minimum = get_number("unfill minimum");
		    if (fill_minimum < 1 || fill_minimum > 16)
		    {
			cerr << "the -unfill run length must be 1..16" << endl;
			exit(1);
		    }
		}
		ifp =
		    new srec_input_filter_unfill(ifp, fill_value, fill_minimum);
	    }
	    continue;

	case token_unsplit:
	    {
		token_next();
		if (!can_get_number())
		{
		    cerr <<
			"the -unsplit filter requires three numeric arguments"
			<< endl;
		    exit(1);
		}
		int split_modulus = get_number("unsplit modulus");
		if (split_modulus < 2)
		{
		    cerr << "the -unsplit modulus must be two or more" << endl;
		    exit(1);
		}
		int split_offset = 0;
		if (can_get_number())
		{
		    split_offset = get_number("unsplit offset");
		    if (split_offset < 0 || split_offset >= split_modulus)
		    {
			cerr << "the -unsplit offset must be 0.."
			    << (split_modulus - 1) << endl;
			exit(1);
		    }
		}
		int split_width = 1;
		if (can_get_number())
		{
		    split_width = get_number("unsplit width");
		    if (split_width < 1 || split_width >= split_modulus)
		    {
			cerr << "the -unsplit width must be 1.."
			    << (split_modulus - 1) << endl;
			exit(1);
		    }
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
	    continue;

	default:
	    break;
	}
	break;
    }

    //
    // return the input stream determined
    //
    return ifp;
}
