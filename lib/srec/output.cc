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
 * MANIFEST: functions to impliment the srec_output class
 */

#pragma implementation "srec_output"

#include <errno.h>
#include <iostream.h>

#include <srec/output.h>
#include <srec/record.h>


srec_output::srec_output()
{
}


srec_output::srec_output(const srec_output &)
{
	fatal_error("bug (%s, %d)", __FILE__, __LINE__);
}


srec_output &
srec_output::operator=(const srec_output &)
{
	fatal_error("bug (%s, %d)", __FILE__, __LINE__);
	return *this;
}


srec_output::~srec_output()
{
}


void
srec_output::fatal_error(const char *fmt, ...)
	const
{
	va_list ap;
	va_start(ap, fmt);
	fatal_error_v(fmt, ap);
	va_end(ap);
}


void
srec_output::fatal_error_v(const char *fmt, va_list ap)
	const
{
	cout.flush();
	cerr << filename() << ": ";
	cerr.vform(fmt, ap);
	cerr << endl;
	cerr.flush();
	exit(1);
}


void
srec_output::fatal_error_errno(const char *fmt, ...)
	const
{
	va_list ap;
	va_start(ap, fmt);
	fatal_error_errno_v(fmt, ap);
	va_end(ap);
}


void
srec_output::fatal_error_errno_v(const char *fmt, va_list ap)
	const
{
	int n = errno;
	cout.flush();
	cerr << filename() << ": ";
	cerr.vform(fmt, ap);
	cerr << ": " << strerror(n) << " [" << n << "]" << endl;
	cerr.flush();
	exit(1);
}


void
srec_output::warning(const char *fmt, ...)
	const
{
	va_list ap;
	va_start(ap, fmt);
	warning_v(fmt, ap);
	va_end(ap);
}


void
srec_output::warning_v(const char *fmt, va_list ap)
	const
{
	cout.flush();
	cerr << filename() << ": warning: ";
	cerr.vform(fmt, ap);
	cerr << endl;
	cerr.flush();
}


void
srec_output::write_header()
{
	srec_record record(srec_record::type_header, (srec_record::address_t)0,
		(const srec_record::data_t *)"HDR", 3);
	write(record);
}


void
srec_output::write_data(unsigned long address, const void *data, size_t length)
{
	const srec_record::data_t *data_p = (const srec_record::data_t *)data;
	while (length > 0)
	{
		int nbytes = (length > 32 ? 32 : length);
		srec_record record(srec_record::type_data, address, data_p,
			nbytes);
		write(record);
		address += nbytes;
		data_p += nbytes;
		length -= nbytes;
	}
}


void
srec_output::write_termination(unsigned long address)
{
	srec_record record(srec_record::type_termination, address);
	write(record);
}
