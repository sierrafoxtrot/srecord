/*
 *	srecord - manipulate eprom load files
 *	Copyright (C) 1998, 1999 Peter Miller;
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
 * MANIFEST: interface definition for lib/srec/memory/walker.cc
 */

#ifndef INCLUDE_SREC_MEMORY_WALKER_H
#define INCLUDE_SREC_MEMORY_WALKER_H

#pragma interface "srec_memory_walker"

class srec_memory_walker
{
public:
	virtual ~srec_memory_walker();
	virtual void observe(unsigned long, const void *, int) = 0;

protected:
	srec_memory_walker();
	srec_memory_walker(const srec_memory_walker &);
	srec_memory_walker &operator=(const srec_memory_walker &);
};

#endif /* INCLUDE_SREC_MEMORY_WALKER_H */
