/*
 *	srecord - manipulate eprom load files
 *	Copyright (C) 1998, 1999, 2000, 2001 Peter Miller;
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
 * MANIFEST: interface definition for lib/srec/memory.cc
 */

#ifndef INCLUDE_SREC_MEMORY_H
#define INCLUDE_SREC_MEMORY_H

#pragma interface "srec_memory"

#include <srec/memory/chunk.h>

class srec_memory
{
public:
	srec_memory();
	srec_memory(const srec_memory &);
	srec_memory &operator=(const srec_memory &);
	~srec_memory();

	void set(unsigned long, int);
	int get(unsigned long) const;
	bool set_p(unsigned long) const;

	void walk(class srec_memory_walker *) const;

	unsigned long reader(class srec_input *, bool = false);

	static bool equal(const srec_memory &, const srec_memory &);
	static bool compare(const srec_memory &, const srec_memory &);

	bool find_next_data(unsigned long &address, void *data,
		size_t &nbytes) const;

	static void allow_overwriting();

private:
	static bool overwrite;
	mutable int nchunks;
	mutable int nchunks_max;
	mutable srec_memory_chunk **chunk;
	mutable srec_memory_chunk *cache;
	mutable int find_next_chunk_index;

	srec_memory_chunk *find(unsigned long) const;
	srec_memory_chunk *find_next_chunk(unsigned long) const;
	void clear();
	void copy(const srec_memory &);
};

bool operator == (const srec_memory &, const srec_memory &);
bool operator != (const srec_memory &, const srec_memory &);

#endif /* INCLUDE_SREC_MEMORY_H */
