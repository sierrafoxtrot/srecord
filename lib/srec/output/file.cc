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
 * MANIFEST: functions to impliment the srec_output_file class
 */

#pragma implementation "srec_output_file"

#include <cstdio>
#include <errno.h>
#include <iostream.h>

#include <srec/output/file.h>


srec_output_file::srec_output_file()
	: file_name("standard output"), line_number(1), checksum(0)
{
	fp = stdout;
}


srec_output_file::srec_output_file(const srec_output_file &)
{
	fatal_error("bug (%s, %d)", __FILE__, __LINE__);
}


srec_output_file::srec_output_file(const char *file_name)
	: file_name(file_name), line_number(1), checksum(0)
{
	if (file_name == string("-"))
	{
		file_name = "standard output";
		fp = stdout;
	}
	else
	{
		fp = fopen(file_name, "w");
		if (!fp)
			fatal_error_errno("open");
	}
}


srec_output_file &
srec_output_file::operator=(const srec_output_file &)
{
	fatal_error("bug (%s, %d)", __FILE__, __LINE__);
	return *this;
}


srec_output_file::~srec_output_file()
{
	FILE *fp = (FILE *)this->fp;
	if (fflush(fp))
		fatal_error_errno("write");
	if (fp != stdout && fclose(fp))
		fatal_error_errno("close");
}


const string
srec_output_file::filename()
	const
{
	char buffer[20];
	sprintf(buffer, ": %d", line_number);
	return (file_name + buffer);
}


void
srec_output_file::put_char(int c)
{
	FILE *fp = (FILE *)this->fp;
	putc(c, fp);
	if (ferror(fp))
		fatal_error_errno("write");
	if (c == '\n')
		++line_number;
}


void
srec_output_file::put_nibble(int n)
{
	put_char("0123456789ABCDEF"[n & 15]);
}


void
srec_output_file::put_byte(int n)
{
	put_nibble(n >> 4);
	put_nibble(n);
	checksum += (unsigned char)n;
}


int
srec_output_file::checksum_get()
{
	return (unsigned char)checksum;
}


void
srec_output_file::checksum_reset()
{
	checksum = 0;
}


void
srec_output_file::seek_to(unsigned long address)
{
	FILE *fp = (FILE *)this->fp;
	if (fseek(fp, address, 0) < 0)
		fatal_error_errno("seek %ld", address);
}


void
srec_output_file::put_string(const char *s)
{
	while (*s)
		put_char(*s++);
}


void
srec_output_file::put_stringf(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	char buffer[1000];
	vsprintf(buffer, fmt, ap);
	va_end(ap);
	put_string(buffer);
}
