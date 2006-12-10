//
//	srecord - manipulate eprom load files
//	Copyright (C) 2001, 2002, 2005, 2006 Peter Miller;
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
// MANIFEST: printf formatting attribute insulation
//

#ifndef INCLUDE_FORMAT_PRINTF_H
#define INCLUDE_FORMAT_PRINTF_H

#ifdef __GNUC__
#define FORMAT_PRINTF(x, y) __attribute__((__format__(printf, x, y)))
#define NORETURN __attribute__((__noreturn__))
#define DEPRECATED __attribute__((__deprecated__))
#else
#define FORMAT_PRINTF(x, y)
#define DEPRECATED
#define NORETURN
#endif

#endif // INCLUDE_FORMAT_PRINTF_H
