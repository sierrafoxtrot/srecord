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
 * MANIFEST: interface definition for lib/srec/memory.cc
 */

#ifndef INCLUDE_SREC_MEMORY_H
#define INCLUDE_SREC_MEMORY_H

#include <srec/memory/chunk.h>

#pragma interface

class srec_memory
{
public:
	srec_memory();
	srec_memory(const srec_memory &);
	srec_memory &operator=(const srec_memory &);
	~srec_memory();

	void set(unsigned long, int);
	int get(unsigned long);
	bool set_p(unsigned long);

	void walk(class srec_memory_walker *) const;

	unsigned long reader(class srec_input *, bool = false);

	static bool equal(const srec_memory &, const srec_memory &);

private:
	int nchunks;
	int nchunks_max;
	srec_memory_chunk **chunk;
	srec_memory_chunk *cache;

	srec_memory_chunk *find(unsigned long);
	void clear();
	void copy(const srec_memory &);
};

bool operator == (const srec_memory &, const srec_memory &);
bool operator != (const srec_memory &, const srec_memory &);

#endif /* INCLUDE_SREC_MEMORY_H */
