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
 * MANIFEST: functions to impliment the srec_memory_chunk class
 */

#pragma implementation

#include <cstring>
#include <srec/memory/chunk.h>
#include <srec/memory/walker.h>


srec_memory_chunk::srec_memory_chunk()
	: address(0)
{
	address = 0;
	memset(data, 0, sizeof(data));
	memset(mask, 0, sizeof(mask));
}


srec_memory_chunk::srec_memory_chunk(unsigned long arg)
	: address(arg)
{
	memset(data, 0, sizeof(data));
	memset(mask, 0, sizeof(mask));
}


srec_memory_chunk::srec_memory_chunk(const srec_memory_chunk &arg)
	: address(arg.address)
{
	memcpy(data, arg.data, sizeof(data));
	memcpy(mask, arg.mask, sizeof(mask));
}


srec_memory_chunk &
srec_memory_chunk::operator=(const srec_memory_chunk &arg)
{
	address = arg.address;
	memcpy(data, arg.data, sizeof(data));
	memcpy(mask, arg.mask, sizeof(mask));
	return *this;
}


srec_memory_chunk::~srec_memory_chunk()
{
}


void
srec_memory_chunk::set(unsigned long offset, int datum)
{
	data[offset] = datum;
	mask[offset >> 3] |= (1 << (offset & 7));
}


void
srec_memory_chunk::walk(srec_memory_walker *w)
	const
{
	for (int j = 0; j < size; ++j)
	{
		if (!set_p(j))
			continue;
		int k;
		for (k = j + 1; k < size && set_p(k); ++k)
			;
		w->observe(address * size + j, data + j, k - j);
		j = k;
	}
}


int
srec_memory_chunk::get(unsigned long offset)
{
	return data[offset];
}


bool
srec_memory_chunk::set_p(unsigned long offset)
	const
{
	return (0 != (mask[offset >> 3] & (1 << (offset & 7))));
}


bool
srec_memory_chunk::equal(const srec_memory_chunk &lhs,
	const srec_memory_chunk &rhs)
{
	return
	(
		lhs.address == rhs.address
	&&
		0 == memcmp(lhs.data, rhs.data, sizeof(lhs.data))
	&&
		0 == memcmp(lhs.mask, rhs.mask, sizeof(lhs.mask))
	);
}


bool
operator == (const srec_memory_chunk &lhs, const srec_memory_chunk &rhs)
{
	return srec_memory_chunk::equal(lhs, rhs);
}


bool
operator != (const srec_memory_chunk &lhs, const srec_memory_chunk &rhs)
{
	return !srec_memory_chunk::equal(lhs, rhs);
}
