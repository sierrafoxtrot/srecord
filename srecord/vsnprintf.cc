//
// srecord - manipulate eprom load files
// Copyright (C) 2002, 2006, 2007, 2010 Peter Miller
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General
// Public License along with this program. If not, see
// <http://www.gnu.org/licenses/>.
//


#include <config.h>


#ifndef HAVE_SNPRINTF

extern "C" int
snprintf(char *str, size_t size, const char *format, ...)
{
    va_start(ap, format);
    vsnprintf(str, size, format, ap);
    va_end(ap);
}

#endif


#ifndef HAVE_VSNPRINTF

extern "C" int
vsnprintf(char *str, size_t size, const char *format, va_list ap)
{
    // We'll just have to hope for the best...
    vsprintf(str, format, ap);
}

#endif
