//
//	srecord - manipulate eprom load files
//	Copyright (C) 2000, 2002 Peter Miller;
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
// MANIFEST: functions to impliment the quit class
//

#pragma implementation "quit"

#include <errno.h>
#include <quit.h>
#include <cstdarg>
#include <cstdio>
#include <cstring>


quit::quit()
{
}


quit::quit(const quit &arg)
{
}


quit &
quit::operator=(const quit &arg)
{
	return *this;
}


quit::~quit()
{
}


void
quit::fatal_error(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	fatal_error_v(fmt, ap);
	va_end(ap);
}


void
quit::fatal_error_v(const char *fmt, va_list ap)
{
	message_v(fmt, ap);
	exit(1);
}


void
quit::fatal_error_errno(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	fatal_error_errno_v(fmt, ap);
	va_end(ap);
}


void
quit::fatal_error_errno_v(const char *fmt, va_list ap)
{
	int n = errno;
	char buf[1024];
	vsnprintf(buf, sizeof(buf), fmt, ap);
	fatal_error("%s: %s", buf, strerror(n));
}


void
quit::warning(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	warning_v(fmt, ap);
	va_end(ap);
}


void
quit::warning_v(const char *fmt, va_list ap)
{
        char buf[1024];
	vsnprintf(buf, sizeof(buf), fmt, ap);
	message("warning: %s", buf);
}


void
quit::message(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	message_v(fmt, ap);
	va_end(ap);
}
