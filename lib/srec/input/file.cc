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
 * MANIFEST: functions to impliment the srec_input_file class
 */

#pragma implementation

#include <cstdio>
#include <errno.h>
#include <iostream.h>

#include <srec/input/file.h>


srec_input_file::srec_input_file()
	: file_name("standard input"), line_number(1), prev_was_newline(false),
	  checksum(0)
{
	fp = stdin;
}


srec_input_file::srec_input_file(const srec_input_file &)
{
	fatal_error("bug (%s, %d)", __FILE__, __LINE__);
}


srec_input_file::srec_input_file(const char *file_name)
	: file_name(file_name), line_number(1), prev_was_newline(false),
	  checksum(0)
{
	if (file_name == string("-"))
	{
		file_name = "standard input";
		fp = stdin;
	}
	else
	{
		fp = fopen(file_name, "r");
		if (!fp)
			fatal_error_errno("open");
	}
}


srec_input_file &
srec_input_file::operator=(const srec_input_file &)
{
	fatal_error("bug (%s, %d)", __FILE__, __LINE__);
	return *this;
}


srec_input_file::~srec_input_file()
{
	FILE *fp = (FILE *)this->fp;
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
	FILE *fp = (FILE *)this->fp;
	if (prev_was_newline)
		++line_number;
	int c = getc(fp);
	if (c == EOF)
	{
		if (ferror(fp))
			fatal_error_errno("read");
		c = -1;
	}
	prev_was_newline = (c == '\n');
	return c;
}


int
srec_input_file::peek_char()
{
	FILE *fp = (FILE *)this->fp;
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
	checksum += n;
	return n;
}


int
srec_input_file::checksum_get()
{
	return (checksum & 0xFF);
}


void
srec_input_file::checksum_reset()
{
	checksum = 0;
}


void
srec_input_file::seek_to_end()
{
	FILE *fp = (FILE *)this->fp;
	fseek(fp, 0L, SEEK_END);
}
