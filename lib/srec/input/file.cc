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
 * MANIFEST: functions to impliment the srec_input_file class
 */

#pragma implementation "srec_input_file"

#include <cstdio>
#include <errno.h>
#include <iostream.h>
#include <string.h>

#include <srec/input/file.h>


srec_input_file::srec_input_file()
	: file_name("standard input"), line_number(1), prev_was_newline(false),
	  checksum(0), is_text(0)
{
	vfp = stdin;
}


srec_input_file::srec_input_file(const srec_input_file &)
{
	fatal_error("bug (%s, %d)", __FILE__, __LINE__);
}


const char *
srec_input_file::mode()
	const
{
	return "r";
}


srec_input_file::srec_input_file(const char *file_name)
	: file_name(file_name), line_number(1), prev_was_newline(false),
	  vfp(0), checksum(0)
{
	if (file_name == string("-"))
	{
		file_name = "standard input";
		vfp = stdin;
	}
	else
	{
		/*
		 * The call to fopen is deferred until the constructor has
		 * completed.  This is so that the virtual mode() method
		 * is available (it isn't in the base class constructor).
		 */
	}
}


srec_input_file &
srec_input_file::operator=(const srec_input_file &)
{
	fatal_error("bug (%s, %d)", __FILE__, __LINE__);
	return *this;
}


void *
srec_input_file::get_fp()
{
	if (!vfp)
	{
		/*
		 * The call to fopen is deferred until the constructor has
		 * completed.  This is so that the virtual mode() method
		 * is available (it isn't in the base class constructor).
		 */
		const char *the_mode = mode();
		vfp = fopen(file_name.c_str(), the_mode);
		if (!vfp)
			fatal_error_errno("open");
		is_text = !strchr(the_mode, 'b');
	}
	return vfp;
}


srec_input_file::~srec_input_file()
{
	FILE *fp = (FILE *)get_fp();
	if (fp != stdin && fclose(fp))
		fatal_error_errno("close");
}


const string
srec_input_file::filename()
	const
{
	return file_name;
}


const string
srec_input_file::filename_and_line()
	const
{
	char buffer[20];
	sprintf(buffer, ": %d", line_number);
	return (file_name + buffer);
}


int
srec_input_file::get_char()
{
	FILE *fp = (FILE *)get_fp();
	if (prev_was_newline)
		++line_number;
	int c = getc(fp);
	if (c == EOF)
	{
		if (ferror(fp))
			fatal_error_errno("read");
		/*
		 * If this is a text file, but the last character wasn't
		 * a newline, insert one.
		 */
		c = ((is_text && !prev_was_newline) ? '\n' : -1);
	}
	else if (c == '\r' && is_text)
	{
		/*
		 * If this is a text file, turn CRLF into LF.
		 * Leave all other sequences containing CR alone.
		 */
		c = getc(fp);
		if (c == EOF)
		{
			if (ferror(fp))
				fatal_error_errno("read");
			c = '\r';
		}
		else if (c != '\n')
		{
			ungetc(c, fp);
			c = '\r';
		}
	}
	prev_was_newline = (c == '\n');
	return c;
}


int
srec_input_file::peek_char()
{
	FILE *fp = (FILE *)get_fp();
	int c = getc(fp);
	if (c == EOF)
	{
		if (ferror(fp))
			fatal_error_errno("read");
		c = -1;
	}
	else
		ungetc(c, fp);
	return c;
}


int
srec_input_file::get_nibble()
{
	int c = get_char();
	switch (c)
	{
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
		return (c - '0');

	case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
		return (c - 'a' + 10);

	case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': 
		return (c - 'A' + 10);
	}
	fatal_error("hexadecimal digit expected");
	/* NOTREACHED */
	return -1;
}


int
srec_input_file::get_byte()
{
	int c1 = get_nibble();
	int c2 = get_nibble();
	int n = ((c1 << 4) | c2);
	checksum_add(n);
	return n;
}


int
srec_input_file::get_word()
{
	int b1 = get_byte();
	int b2 = get_byte();
	return ((b1 << 8) | b2);
}


unsigned long
srec_input_file::get_3bytes()
{
	unsigned long b1 = get_byte();
	unsigned long b2 = get_byte();
	unsigned long b3 = get_byte();
	return ((((b1 << 8) | b2) << 8) | b3);
}


unsigned long
srec_input_file::get_4bytes()
{
	unsigned long b1 = get_byte();
	unsigned long b2 = get_byte();
	unsigned long b3 = get_byte();
	unsigned long b4 = get_byte();
	return ((((((b1 << 8) | b2) << 8) | b3) << 8) | b4);
}


int
srec_input_file::checksum_get()
	const
{
	return (checksum & 0xFF);
}


int
srec_input_file::checksum_get16()
	const
{
	return (checksum & 0xFFFF);
}


void
srec_input_file::checksum_reset()
{
	checksum = 0;
}


void
srec_input_file::seek_to_end()
{
	FILE *fp = (FILE *)get_fp();
	fseek(fp, 0L, SEEK_END);
}
