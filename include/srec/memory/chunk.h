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
 * MANIFEST: interface definition for lib/srec/memory/chunk.cc
 */

#ifndef INCLUDE_SREC_MEMORY_CHUNK_H
#define INCLUDE_SREC_MEMORY_CHUNK_H

#pragma interface "srec_memory_chunk"

class srec_memory_chunk
{
public:
	enum { size = 7 * 256 };

	srec_memory_chunk(unsigned long);
	srec_memory_chunk(const srec_memory_chunk &);
	srec_memory_chunk &operator=(const srec_memory_chunk &);
	~srec_memory_chunk();

	void set(unsigned long address, int data);
	int get(unsigned long);
	bool set_p(unsigned long) const;

	void walk(class srec_memory_walker *) const;

	unsigned long get_address() const { return address; }

	static bool equal(const srec_memory_chunk &, const srec_memory_chunk &);

private:
	unsigned long address;
	unsigned char data[size];
	unsigned char mask[(size + 7) / 8];

	srec_memory_chunk();
};

bool operator == (const srec_memory_chunk &, const srec_memory_chunk &);
bool operator != (const srec_memory_chunk &, const srec_memory_chunk &);

#endif /* INCLUDE_SREC_MEMORY_CHUNK_H */
