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
 * MANIFEST: functions to impliment the srec_memory class
 */

#pragma implementation

#include <srec/input.h>
#include <srec/memory.h>
#include <srec/memory/walker.h>
#include <srec/record.h>


srec_memory::srec_memory()
	: nchunks(0), nchunks_max(0), chunk(0), cache(0)
{
}


srec_memory::srec_memory(const srec_memory &arg)
	: nchunks(0), nchunks_max(0), chunk(0), cache(0)
{
	copy(arg);
}


srec_memory &
srec_memory::operator=(const srec_memory &arg)
{
	clear();
	copy(arg);
	return *this;
}


srec_memory::~srec_memory()
{
	clear();
}


void
srec_memory::clear()
{
	for (int j = 0; j < nchunks; ++j)
		delete chunk[j];
	if (chunk)
		delete chunk;
	nchunks = 0;
	nchunks_max = 0;
	chunk = 0;
	cache = 0;
}


void
srec_memory::copy(const srec_memory &arg)
{
	nchunks = arg.nchunks;
	nchunks_max = arg.nchunks_max;
	chunk = new (srec_memory_chunk *)[nchunks_max];
	for (int j = 0; j < nchunks; ++j)
		chunk[j] = new srec_memory_chunk(arg.chunk[j][0]);
}


srec_memory_chunk *
srec_memory::find(unsigned long address)
{
	/*
	 * Speed things up if we've been there recently.
	 */
	if (cache && cache->get_address() == address)
		return cache;

	/*
	 * Binary chop to find the appropriate chunk.
	 */
	int min = 0;
	int max = nchunks - 1;
	srec_memory_chunk *mcp;
	while (min <= max)
	{
		int mid = (min + max) / 2;
		mcp = chunk[mid];
		if (mcp->get_address() == address)
		{
			cache = mcp;
			return cache;
		}
		if (address < mcp->get_address())
			max = mid - 1;
		else
			min = mid + 1;
	}

	/*
	 * We need a new row.  Make sure there is enough room.
	 */
	if (nchunks >= nchunks_max)
	{
		nchunks_max = nchunks_max * 2 + 4;
		srec_memory_chunk **tmp =
			new (srec_memory_chunk *)[nchunks_max];
		for (int j = 0; j < nchunks; ++j)
			tmp[j] = chunk[j];
		delete chunk;
		chunk = tmp;
	}

	/*
	 * Insert the new chunk.
	 */
	mcp = new srec_memory_chunk(address);
	for (int up = nchunks; up > min; --up)
		chunk[up] = chunk[up - 1];
	++nchunks;
	chunk[min] = mcp;

	cache = mcp;
	return mcp;
}


void
srec_memory::set(unsigned long address, int datum)
{
	unsigned long address_hi = address / srec_memory_chunk::size;
	unsigned long address_lo = address % srec_memory_chunk::size;
	srec_memory_chunk *mcp = find(address_hi);
	mcp->set(address_lo, datum);
}


int
srec_memory::get(unsigned long address)
{
	unsigned long address_hi = address / srec_memory_chunk::size;
	unsigned long address_lo = address % srec_memory_chunk::size;
	srec_memory_chunk *mcp = find(address_hi);
	return mcp->get(address_lo);
}


bool
srec_memory::set_p(unsigned long address)
{
	unsigned long address_hi = address / srec_memory_chunk::size;
	unsigned long address_lo = address % srec_memory_chunk::size;
	srec_memory_chunk *mcp = find(address_hi);
	return mcp->set_p(address_lo);
}


bool
srec_memory::equal(const srec_memory &lhs, const srec_memory &rhs)
{
	if (lhs.nchunks != rhs.nchunks)
		return false;
	for (int j = 0; j < lhs.nchunks; ++j)
		if (lhs.chunk[j][0] != rhs.chunk[j][0])
			return false;
	return true;
}


void
srec_memory::walk(srec_memory_walker *w)
	const
{
	for (int j = 0; j < nchunks; ++j)
		chunk[j]->walk(w);
}


unsigned long
srec_memory::reader(srec_input *ifp)
{
	unsigned long result = 0;
	srec_record record;
	while (ifp->read(record))
	{
		switch (record.get_type())
		{
		case srec_record::type_unknown:
		case srec_record::type_header:
		case srec_record::type_data_count:
			break;

		case srec_record::type_data:
			for (int j = 0; j < record.get_length(); ++j)
			{
				srec_record::address_t address =
					record.get_address() + j;
				if (set_p(address))
				{
					ifp->warning
					(
						"duplicate %08lX value",
						(long)address
					);
				}
				set(address, record.get_data(j));
			}
			break;

		case srec_record::type_termination:
			result = record.get_address();
			break;
		}
	}
	return result;
}


bool
operator == (const srec_memory &lhs, const srec_memory &rhs)
{
	return srec_memory::equal(lhs, rhs);
}


bool
operator != (const srec_memory &lhs, const srec_memory &rhs)
{
	return !srec_memory::equal(lhs, rhs);
}
