/*
 *	srecord - manipulate eprom load files
 *	Copyright (C) 2000 Peter Miller;
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
 * MANIFEST: interface definition for include/srec/input/filter/crc32.cc
 */

#ifndef INCLUDE_SREC_INPUT_FILTER_CRC32_H
#define INCLUDE_SREC_INPUT_FILTER_CRC32_H

#pragma interface "srec_input_filter_crc32"

#include <srec/input/filter.h>

class srec_input_filter_crc32:
	public srec_input_filter
{
public:
	virtual ~srec_input_filter_crc32();
	srec_input_filter_crc32(srec_input *deeper, unsigned long address,
		int order);
	virtual int read(srec_record &);

private:
	srec_input *deeper;
	unsigned long address;
	int order;
	class srec_memory *buffer;
	unsigned long buffer_pos;
	srec_input_filter_crc32();
	srec_input_filter_crc32(const srec_input_filter_crc32 &);
	srec_input_filter_crc32 &operator=(const srec_input_filter_crc32 &);
};

#endif /* INCLUDE_SREC_INPUT_FILTER_CRC32_H */
