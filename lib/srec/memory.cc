/*
 *	srecord - manipulate eprom load files
 *	Copyright (C) 1998-2002 Peter Miller;
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
 * MANIFEST: functions to impliment the srec_memory class
 */

#pragma implementation "srec_memory"

#include <srec/input.h>
#include <srec/memory.h>
#include <srec/memory/walker/compare.h>
#include <srec/record.h>


bool srec_memory::overwrite = false;


srec_memory::srec_memory() :
	nchunks(0),
	nchunks_max(0),
	chunk(0),
	cache(0),
	find_next_chunk_index(0),
	hdr(0)
{
}


srec_memory::srec_memory(const srec_memory &arg) :
	nchunks(0),
	nchunks_max(0),
	chunk(0),
	cache(0),
	find_next_chunk_index(0)
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
	delete hdr;
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
	const
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
	srec_memory_chunk *mcp = 0;
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
	const
{
	unsigned long address_hi = address / srec_memory_chunk::size;
	unsigned long address_lo = address % srec_memory_chunk::size;
	srec_memory_chunk *mcp = find(address_hi);
	return mcp->get(address_lo);
}


bool
srec_memory::set_p(unsigned long address)
	const
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



bool
srec_memory::compare(const srec_memory &lhs, const srec_memory &rhs)
{
	srec_memory_walker_compare wlhs(rhs, true);
	lhs.walk(&wlhs);
	wlhs.print("Left");
	srec_memory_walker_compare wrhs(lhs, false);
	rhs.walk(&wrhs);
	wrhs.print("Right");
	return (!wlhs.same() || !wrhs.same());
}


void
srec_memory::walk(srec_memory_walker *w)
	const
{
	for (int j = 0; j < nchunks; ++j)
		chunk[j]->walk(w);
}


unsigned long
srec_memory::reader(srec_input *ifp, bool barf)
{
	unsigned long result = 0;
	srec_record record;
	while (ifp->read(record))
	{
		switch (record.get_type())
		{
		case srec_record::type_header:
			if (!hdr)
			{
				hdr = new srec_record(record);
			}
			break;

		case srec_record::type_unknown:
		case srec_record::type_data_count:
			break;

		case srec_record::type_data:
			for (int j = 0; j < record.get_length(); ++j)
			{
				srec_record::address_t address =
					record.get_address() + j;
				int n = record.get_data(j);
				if (barf && set_p(address))
				{
					int old = get(address);
					if (n == old)
					{
						ifp->warning
						(
							"redundant %08lX value",
							(long)address
						);
					}
					else if (overwrite)
					{
						ifp->warning
						(
		     "multiple %08lX values (previous = %02X, this one = %02X)",
							(long)address,
							old,
							n
						);
					}
					else
					{
						ifp->fatal_error
						(
		 "contradictory %08lX value (previous = %02X, this one = %02X)",
							(long)address,
							old,
							n
						);
					}
				}
				set(address, n);
			}
			break;

		case srec_record::type_start_address:
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


srec_memory_chunk *
srec_memory::find_next_chunk(unsigned long address)
	const
{
	//
	// This method is generally called sequentially, to visit each
	// and every byte, in cases where walk() is not appropriate.
	// As such, a binary chop isn't necessary.
	//
	if (find_next_chunk_index < nchunks)
	{
		srec_memory_chunk *mcp = chunk[find_next_chunk_index];
		if (mcp->get_address() > address)
			find_next_chunk_index = 0;
	}
	while (find_next_chunk_index < nchunks)
	{
		srec_memory_chunk *mcp = chunk[find_next_chunk_index];
		if (mcp->get_address() >= address)
			return mcp;
		find_next_chunk_index++;
	}
	return 0;
}


bool
srec_memory::find_next_data(unsigned long &address, void *data, size_t &nbytes)
	const
{
	unsigned long address_hi = address / srec_memory_chunk::size;
	for (;;)
	{
		srec_memory_chunk *mcp = find_next_chunk(address_hi);
		if (!mcp)
			return false;
		if (mcp->find_next_data(address, data, nbytes))
			return true;
		address_hi = mcp->get_address() + 1;
		address = address_hi * srec_memory_chunk::size;
	}
}


void
srec_memory::allow_overwriting()
{
    overwrite = true;
}


srec_record *
srec_memory::get_header()
    const
{
    return hdr;
}


void
srec_memory::set_header(const char *s)
{
    delete hdr;
    size_t len = strlen(s);
    if (len > srec_record::max_data_length)
    	len = srec_record::max_data_length;
    hdr =
    	new srec_record
	(
	    srec_record::type_header,
	    0,
	    (srec_record::data_t *)s,
	    len
	);
}
