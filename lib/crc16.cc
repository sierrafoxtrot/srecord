//
//	srecord - manipulate eprom load files
//	Copyright (C) 2000-2002, 2006 Peter Miller;
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
// MANIFEST: functions to impliment the crc16 class
//
// I got this copy from http://www.gator.net/~garyg/C/CONTRIB/SNIP/
// but knowing the Internet, it's probably gone by now.  Search for
// "crc-16" and you should find it fairly quickly.  The same code
// also appears in the ZModem sources, as well.
//


#include <lib/crc16.h>

//
// The CRC polynomial.  This is used by XMODEM (almost CCITT).
//
#define POLYNOMIAL 0x1021


static unsigned short table[256];
static unsigned short const ccitt_seed = 0xFFFF;
static unsigned short const xmodem_seed = 0;


static void
calculate_table()
{
    for (int b = 0; b < 256; ++b)
    {
	unsigned short v = b << 8;
	for (int i = 8; --i >= 0; )
    	    v = (v & 0x8000) ? ((v << 1) ^ POLYNOMIAL) : (v << 1);
	table[b] = v;
    }
}


crc16::crc16(bool ccitt)
{
    if (ccitt)
	seed = ccitt_seed;
    else
	seed = xmodem_seed;
    
    state = seed;
    if (!table[1])
	calculate_table();
}


crc16::crc16(const crc16 &arg) :
    state(arg.state),
    seed(arg.seed)
{
}


crc16 &
crc16::operator=(const crc16 &arg)
{
    if (this != &arg)
    {
	state = arg.state;
	seed = arg.seed;
    }
    return *this;
}


crc16::~crc16()
{
}

//
// The ``updcrc'' function is derived from the updcrc macro derived from
// article Copyright (C) 1986 Stephen Satchell.
// NOTE: First argument must be in range 0 to 255.
//
// Programmers may incorporate any or all code into their programs,
// giving proper credit within the source.  Publication of the
// source routines is permitted so long as proper credit is given
// to Stephen Satchell, Satchell Evaluations and Chuck Forsberg,
// Omen Technology.
//

static inline unsigned short
updcrc(unsigned char cp, unsigned short crc)
{
    return
    table[((crc >> 8) ^ cp) & 0xFF] ^ (crc << 8);
}

void
crc16::next(unsigned char x)
{
    state = updcrc(x, state);
}

void
crc16::nextbuf(const void *data, size_t nbytes)
{
    unsigned char *dp = (unsigned char *)data;
    while (nbytes > 0)
    {
	state = updcrc(*dp, state);
	++dp;
	--nbytes;
    }
}

unsigned short
crc16::get()
    const
{
    return state;
}
