//
//	srecord - manipulate eprom load files
//	Copyright (C) 2000-2002 Peter Miller;
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
// MANIFEST: interface definition for include/crc16.cc
//

#ifndef INCLUDE_CRC16_H
#define INCLUDE_CRC16_H

#pragma interface "crc16"

#include <stddef.h>

class crc16
{
public:
	virtual ~crc16();
	crc16();
	crc16(const crc16 &);
	crc16 &operator=(const crc16 &);
	unsigned short get() const;
	void next(unsigned char);
	void nextbuf(const void *, size_t);
private:
	unsigned short state;
};

#endif // INCLUDE_CRC16_H
