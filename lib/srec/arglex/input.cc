//
//	srecord - manipulate eprom load files
//	Copyright (C) 1998-2004 Peter Miller;
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
// MANIFEST: functions to impliment the srec_arglex input method
//

#include <iostream>
using namespace std;

#include <srec/arglex.h>
#include <srec/input/file/aomf.h>
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
#include <srec/input/filter/random_fill.h>
#include <srec/input/filter/split.h>
#include <srec/input/filter/unfill.h>
#include <srec/input/filter/unsplit.h>
#include <srec/input/filter/xor.h>
#include <srec/input/interval.h>


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

	case token_random_fill:
	    {
		token_next();
		interval range = get_interval("-Random_Fill");
		ifp = new srec_input_filter_random_fill(ifp, range);
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
