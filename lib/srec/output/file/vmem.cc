//
//	srecord - manipulate eprom load files
//	Copyright (C) 2000-2004 Peter Miller;
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
// MANIFEST: functions to impliment the srec_output_file_vmem class
//

#pragma implementation "srec_output_file_vmem"

#include <cctype>
#include <srec/arglex.h>
#include <srec/output/file/vmem.h>
#include <srec/record.h>


//
// The default number of bits is 32.
// If you change this, you must also change the following files:
//     lib/srec/output/file/vmem.cc
//     man/man1/srec_cat.1
//
#define DEFAULT_MEM_WIDTH 32


//
// Calculate log2(bytes_per_value)
//
// For example...
// Return  Num    Num
// Value   Bytes  Bits
//   0      1       8
//   1      2      16
//   2      4      32
//   3      8      64
//   4     16     128
//
static unsigned
calc_width_shift(int x)
{
    //
    // The user could be giving a number of bytes.
    // (But the range is limited to those values not easily confused
    // with a number of bits.)
    //
    if (x == 1)
	return 0;
    if (x == 2)
	return 1;
    if (x == 4)
	return 2;

    //
    // The documented interface is a number of bits.
    //
    for (int j = 0; j < 28; ++j)
    {
	int nbits = 8 << j;
	if (x <= nbits)
	    return j;
    }

    //
    // The default number of bits is 32.
    // If you change this, you must also change the following files:
    //     man/man1/srec_cat.1
    //     lib/srec/arglex_output.cc
    //
    return 2;
}


static unsigned
calc_width_mask(int x)
{
    return ((1u << calc_width_shift(x)) - 1u);
}


srec_output_file_vmem::srec_output_file_vmem(const char *filename) :
    srec_output_file(filename),
    address(0),
    column(0),
    pref_block_size(16),
    width_shift(calc_width_shift(DEFAULT_MEM_WIDTH)),
    width_mask(calc_width_mask(DEFAULT_MEM_WIDTH))
{
}


void
srec_output_file_vmem::command_line(srec_arglex *cmdln)
{
    if (cmdln->token_cur() == arglex::token_number)
    {
	int n = cmdln->value_number();
	cmdln->token_next();

	width_shift = calc_width_shift(n);
	width_mask = calc_width_mask(n);
    }
}


srec_output_file_vmem::~srec_output_file_vmem()
{
    while (address & width_mask)
	write_byte(0xFF);
    if (column)
	put_char('\n');
}


void
srec_output_file_vmem::write_byte(unsigned value)
{
    //
    // Each line starts with an address.
    // The addresses are actually divided by the memory width,
    // rather than being byte adddresses.
    //
    // (The presence of the @ character would seem to imply this is optional,
    // because it is easy to parse that it is not the start of a hex byte.)
    //
    if (column == 0)
	put_stringf("@%08lX", address >> width_shift);

    //
    // Put a space between each memory-width chunk of bytes.
    //
    if ((column & width_mask) == 0)
	put_char(' ');

    //
    // Write the byte and crank the address.
    //
    put_byte(value);
    ++address;

    //
    // Crank the column.
    // If the line is too long, finish it.
    //
    ++column;
    if (column >= pref_block_size)
    {
	put_char('\n');
	column = 0;
    }
}


void
srec_output_file_vmem::write_byte_at(unsigned long new_addr, unsigned value)
{
    //
    // We have to seek to the new address, but there are several cases.
    //
    // The easiest (and commonest) case is that the address is already
    // exactly where we need it.
    //
    // The next commonest is that the current address is nicely aligned
    // on a memory width boundary, and the new address is also nicely
    // aligned.  No padding is required in this case; the loop simply
    // sets the new address.
    //
    // There are three cases where we need padding
    //       nn nn nn FF         on the end of a memory width chunk
    //       FF nn nn nn         on the beginning of a memory width chunk
    //       nn FF nn nn         in the middle of a memory width chunk
    // sometimes end AND beginning padding will be required for the one
    // change of address.  The loop takes care of all these cases,
    // padding with an 0xFF value.
    //
    while (address != new_addr)
    {
	if (address & width_mask)
	    write_byte(0xFF);
	else
	{
	    // Round down, which provokes the beginning padding, when required.
	    unsigned long zero = new_addr & ~(unsigned long)width_mask;
	    if (address == zero)
		write_byte(0xFF);
	    else
		address = zero;
	}
    }

    //
    // Now that the address is correct, write out the byte.
    //
    write_byte(value);
}


void
srec_output_file_vmem::write(const srec_record &record)
{
    switch (record.get_type())
    {
    case srec_record::type_header:
	// emit header records as comments in the file
	{
	    put_string("/* ");
	    if (record.get_address() != 0)
		put_stringf("%08lX: ", record.get_address());
	    const unsigned char *cp = record.get_data();
	    const unsigned char *ep = cp + record.get_length();
	    while (cp < ep)
	    {
		int c = *cp++;
		if (isprint(c) || isspace(c))
		    put_char(c);
		else
		    put_stringf("\\%o", c);
		// make sure we don't end the comment
		if (c == '*' && cp < ep && *cp == '/')
		    put_char(' ');
	    }
	    put_string(" */\n");
	}
	break;

    case srec_record::type_data:
	for (int j = 0; j < record.get_length(); ++j)
	{
	    write_byte_at(record.get_address() + j, record.get_data(j));
	}
	break;

    case srec_record::type_data_count:
    case srec_record::type_start_address:
	// ignore
	break;

    case srec_record::type_unknown:
	fatal_error("can't write unknown record type");
    }
}


void
srec_output_file_vmem::line_length_set(int linlen)
{
    int nchunks = (linlen - 9) / ((2 << width_shift) + 1);
    int max_chunks = srec_record::max_data_length >> width_shift;
    if (nchunks > max_chunks)
	nchunks = max_chunks;
    if (nchunks < 1)
	nchunks = 1;
    int nbytes = nchunks << width_shift;
    pref_block_size = nbytes;
}


void
srec_output_file_vmem::address_length_set(int n)
{
    // ignore
}


int
srec_output_file_vmem::preferred_block_size_get()
    const
{
    return pref_block_size;
}
