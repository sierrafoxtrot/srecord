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
 * MANIFEST: functions to impliment the srec_record class
 */

#pragma implementation

#include <srec/record.h>


srec_record::srec_record()
	: itype(type_unknown), address(0), length(0)
{
}


srec_record::srec_record(const srec_record &arg)
	: itype(arg.itype), address(arg.address), length(arg.length)
{
	if (arg.length > 0)
		memcpy(data, arg.data, arg.length);
}


srec_record::srec_record(type arg)
	: itype(arg), address(0), length(0)
{
}


srec_record::srec_record(type a1, address_t a2)
	: itype(a1), address(a2), length(0)
{
}


srec_record::srec_record(type a1, address_t a2, const data_t *a3, int a4)
	: itype(a1), address(a2), length(a4 > 0 ? a4 : 0)
{
	if (length > 0)
		memcpy(data, a3, length);
}


srec_record &
srec_record::operator=(const srec_record &arg)
{
	itype = arg.itype;
	address = arg.address;
	length = arg.length;
	if (arg.length > 0)
		memcpy(data, arg.data, arg.length);
	return *this;
}


srec_record::~srec_record()
{
}


int
srec_record::maximum_data_length(address_t address)
{
	if (address < (1UL << 16))
		return 252;
	if (address < (1UL << 24))
		return 251;
	return 250;
}


srec_record::address_t
srec_record::decode_big_endian(data_t *buffer, int length)
{
	address_t result = 0;
	while (length-- > 0)
		result = (result << 8) | *buffer++;
	return result;
}


void
srec_record::encode_big_endian(data_t *buffer, address_t value, int length)
{
	while (length > 0)
	{
		buffer[--length] = value;
		value >>= 8;
	}
}
