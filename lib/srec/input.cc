/*
 *	srecord - manipulate eprom load files
 *	Copyright (C) 1998, 1999, 2000 Peter Miller;
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
 * MANIFEST: functions to impliment the srec_input class
 */

#pragma implementation "srec_input"

#include <errno.h>
#include <iostream.h>
#include <cstring>

#include <srec/input.h>
#include <srec/record.h>
#include <quit/prefix.h>


srec_input::srec_input()
	: quitter(&quit_default)
{
}


srec_input::srec_input(const srec_input &)
	: quitter(&quit_default)
{
	fatal_error("bug (%s, %d)", __FILE__, __LINE__);
}


srec_input &
srec_input::operator=(const srec_input &)
{
	fatal_error("bug (%s, %d)", __FILE__, __LINE__);
	return *this;
}


srec_input::~srec_input()
{
}


void
srec_input::fatal_error(const char *fmt, ...)
	const
{
	va_list ap;
	va_start(ap, fmt);
	quit_prefix blab(*quitter, filename_and_line());
	blab.fatal_error_v(fmt, ap);
	va_end(ap);
}


void
srec_input::fatal_error_errno(const char *fmt, ...)
	const
{
	va_list ap;
	va_start(ap, fmt);
	quit_prefix blab(*quitter, filename_and_line());
	blab.fatal_error_errno_v(fmt, ap);
	va_end(ap);
}


void
srec_input::warning(const char *fmt, ...)
	const
{
	va_list ap;
	va_start(ap, fmt);
	quit_prefix blab(*quitter, filename_and_line());
	blab.warning_v(fmt, ap);
	va_end(ap);
}


const string
srec_input::filename_and_line()
	const
{
	return filename();
}


void
srec_input::set_quit(quit &arg)
{
	quitter = &arg;
}


void
srec_input::reset_quit()
{
	quitter = &quit_default;
}
