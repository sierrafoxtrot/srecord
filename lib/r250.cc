//
//      srecord - manipulate eprom load files
//      Copyright (C) 2004, 2006, 2007 Peter Miller
//
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
//      02110-1301 USA.
//
// MANIFEST: functions to impliment the common_r250 class
//

#include <cstdlib>
#include <time.h>
#include <unistd.h>

#include <lib/r250.h>

static  unsigned long   buf[250];
static  unsigned long   *pos;


static inline int
rand8(void)
{
    return ((rand() >> 7) & 255);
}


static inline long
rand32(void)
{
    return
        (
            ((unsigned long)rand8() << 24)
        |
            ((unsigned long)rand8() << 16)
        |
            ((unsigned long)rand8() << 8)
        |
            (unsigned long)rand8()
        );
}


#define SIZEOF(a) (sizeof(a) / sizeof(a[0]))
#define ENDOF(a) ((a) + SIZEOF(a))

static bool ready;


static void
r250_init(void)
{
    ready = true;

    /*
     * initialize crummy linear congruential
     */
    time_t now;
    time(&now);
    srand(now + getpid());

    /*
     * position to start of array
     */
    pos = buf;

    /*
     * initialise contents of array
     */
    unsigned long *bp;
    for (bp = buf; bp < ENDOF(buf); ++bp)
        *bp = rand32();

    /*
     * make sure the bits are linearly independent
     */
    unsigned long bit;
    for (bit = 1, bp = buf + 3; bit; bp += 11, bit <<= 1)
    {
        if (bp >= ENDOF(buf))
            bp -= SIZEOF(buf);
        *bp = (*bp & ~(bit - 1)) | bit;
    }
}


unsigned long
r250(void)
{
    if (!ready)
        r250_init();
    unsigned long *other = pos + 103;
    if (other >= ENDOF(buf))
        other -= SIZEOF(buf);
    *pos ^= *other;
    unsigned long result = *pos++;
    if (pos >= ENDOF(buf))
        pos = buf;
    return result;
}
