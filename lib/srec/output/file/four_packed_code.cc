//
//	srecord - manipulate eprom load files
//	Copyright (C) 2001, 2002, 2005 Peter Miller;
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
// MANIFEST: functions to impliment the srec_output_file_four_packed_code class
//

#pragma implementation "srec_output_file_four_packed_code"

#include <srec/output/file/four_packed_code.h>
#include <srec/record.h>


static unsigned char digit[] =
{
	'%',	//  0
	'&',	//  1
	'\'',	//  2
	'(',	//  3
	')',	//  4
	'+',	//  5
	',',	//  6
	'-',	//  7
	'.',	//  8
	'/',	//  9
	'0',	// 10
	'1',	// 11
	'2',	// 12
	'3',	// 13
	'4',	// 14
	'5',	// 15
	'6',	// 16
	'7',	// 17
	'8',	// 18
	'9',	// 19
	':',	// 20
	';',	// 21
	'<',	// 22
	'=',	// 23
	'>',	// 24
	'?',	// 25
	'@',	// 26
	'A',	// 27
	'B',	// 28
	'C',	// 29
	'D',	// 30
	'E',	// 31
	'F',	// 32
	'G',	// 33
	'H',	// 34
	'I',	// 35
	'J',	// 36
	'K',	// 37
	'L',	// 38
	'M',	// 39
	'N',	// 40
	'O',	// 41
	'P',	// 42
	'Q',	// 43
	'R',	// 44
	'S',	// 45
	'T',	// 46
	'U',	// 47
	'V',	// 48
	'W',	// 49
	'X',	// 50
	'Y',	// 51
	'Z',	// 52
	'[',	// 53
	'\\',	// 54
	']',	// 55
	'^',	// 56
	'_',	// 57
	'`',	// 58
	'a',	// 59
	'b',	// 60
	'c',	// 61
	'd',	// 62
	'e',	// 63
	'f',	// 64
	'g',	// 65
	'h',	// 66
	'i',	// 67
	'j',	// 68
	'k',	// 69
	'l',	// 70
	'm',	// 71
	'n',	// 72
	'o',	// 73
	'p',	// 74
	'q',	// 75
	'r',	// 76
	's',	// 77
	't',	// 78
	'u',	// 79
	'v',	// 80
	'w',	// 81
	'x',	// 82
	'y',	// 83
	'z',	// 84
};


srec_output_file_four_packed_code::srec_output_file_four_packed_code() :
	srec_output_file(),
	pref_block_size(32),
	put_byte_pos(0),
	put_byte_value(0)
{
}


srec_output_file_four_packed_code::srec_output_file_four_packed_code(const char *filename) :
	srec_output_file(filename),
	pref_block_size(32),
	put_byte_pos(0),
	put_byte_value(0)
{
}


srec_output_file_four_packed_code::srec_output_file_four_packed_code(const srec_output_file_four_packed_code &) :
	srec_output_file(),
	pref_block_size(32),
	put_byte_pos(0),
	put_byte_value(0)
{
	fatal_error("bug (%s, %d)", __FILE__, __LINE__);
}


srec_output_file_four_packed_code &
srec_output_file_four_packed_code::operator=(const srec_output_file_four_packed_code &)
{
	fatal_error("bug (%s, %d)", __FILE__, __LINE__);
	return *this;
}


srec_output_file_four_packed_code::~srec_output_file_four_packed_code()
{
	// check for data count record
	// check for termination record
}


void
srec_output_file_four_packed_code::put_byte(unsigned char n)
{
    put_byte_value |= (unsigned long)n << ((3 - put_byte_pos) << 3);
    ++put_byte_pos;
    if (put_byte_pos >= 4)
    {
    	int n5 = put_byte_value % 85; put_byte_value /= 85;
    	int n4 = put_byte_value % 85; put_byte_value /= 85;
    	int n3 = put_byte_value % 85; put_byte_value /= 85;
    	int n2 = put_byte_value % 85; put_byte_value /= 85;
    	int n1 = put_byte_value;
	put_char(digit[n1]);
	put_char(digit[n2]);
	put_char(digit[n3]);
	put_char(digit[n4]);
	put_char(digit[n5]);
	put_byte_pos = 0;
	put_byte_value = 0;
    }
}


void
srec_output_file_four_packed_code::write_inner(unsigned long address,
	const void *data, int data_nbytes)
{
	//
	// Make sure the line is not too long.
	//
	if (data_nbytes > 252)
		fatal_error("data length (%d) too long", data_nbytes);

	//
	// Assemble the data for this line.
	//
	unsigned char buffer[256];
	buffer[0] = 0;
	buffer[1] = 4 + data_nbytes;
	buffer[2] = 0;
	buffer[3] = 0;
	srec_record::encode_big_endian(buffer + 4, address, 4);
	if (data_nbytes)
	{
		memcpy(buffer + 8, data, data_nbytes);
		while (data_nbytes & 3)
			buffer[8 + data_nbytes++] = 0;
	}
	int nbytes = 8 + data_nbytes;

	//
	// Calculate the checksum.
	//
	int checksum = 0;
	for (int j = 0; j < nbytes; ++j)
		checksum += buffer[j];
	buffer[0] = -checksum;

	//
	// Emit the line as base85 text.
	//
	put_char('$');
	for (int j = 0; j < nbytes; ++j)
		put_byte(buffer[j]);
	put_char('\n');
}


void
srec_output_file_four_packed_code::write(const srec_record &record)
{
	switch (record.get_type())
	{
	case srec_record::type_header:
		// This format can't do header records
		break;

	case srec_record::type_data:
		if (record.get_address() + record.get_length() > (1UL << 16))
		{
			fatal_error
			(
				"data address (0x%lX..0x%lX) too large",
				record.get_address(),
				record.get_address() + record.get_length() - 1
			);
		}
		write_inner
		(
			record.get_address(),
			record.get_data(),
			record.get_length()
		);
		break;

	case srec_record::type_data_count:
		// ignore
		break;

	case srec_record::type_start_address:
		if (data_only_flag)
			break;
		if (record.get_address() == 0)
			put_string("$%%%%%\n");
		else
			write_inner(record.get_address(), 0, 0);
		break;

	case srec_record::type_unknown:
		fatal_error("can't write unknown record type");
	}
}


void
srec_output_file_four_packed_code::line_length_set(int linlen)
{
	//
	// Given the number of characters, figure the maximum number of
	// data bytes.
	//
	int n = (linlen - 11) / 5 * 4;

	//
	// Constrain based on the file format.
	//
	if (n < 1)
		n = 1;
	else if (n > 252)
		n = 252;

	//
	// An additional constraint is the size of the srec_record
	// data buffer.
	//
	if (n > srec_record::max_data_length)
		n = srec_record::max_data_length;
	pref_block_size = n;
}


int
srec_output_file_four_packed_code::preferred_block_size_get()
	const
{
	return pref_block_size;
}


void
srec_output_file_four_packed_code::address_length_set(int)
{
	// Ignore.  We always emit a 32-bit address.
}
