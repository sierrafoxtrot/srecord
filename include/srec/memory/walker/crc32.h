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
// MANIFEST: interface definition for include/srec/memory/walker/crc32.cc
//

#ifndef INCLUDE_SREC_MEMORY_WALKER_CRC32_H
#define INCLUDE_SREC_MEMORY_WALKER_CRC32_H

#pragma interface "srec_memory_walker_crc32"

#include <crc32.h>
#include <srec/memory/walker.h>

class srec_memory_walker_crc32: public srec_memory_walker
{
public:
	virtual ~srec_memory_walker_crc32();
	srec_memory_walker_crc32();
	void observe(unsigned long, const void *, int);
	unsigned get();

private:
	crc32 checksum;
	srec_memory_walker_crc32(const srec_memory_walker_crc32 &);
	srec_memory_walker_crc32 &operator=(const srec_memory_walker_crc32 &);
};

#endif // INCLUDE_SREC_MEMORY_WALKER_CRC32_H
