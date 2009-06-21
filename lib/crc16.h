//
// srecord - manipulate eprom load files
// Copyright (C) 2000-2003, 2006-2009 Peter Miller
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at
// your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
//
// See http://www.joegeluso.com/software/articles/ccitt.htm for a great
// write-up on the CRC16 calculation.
//

#ifndef LIB_CRC16_H
#define LIB_CRC16_H


#include <stddef.h>

/**
  * The crc16 class is used to represent the running value of a 16-bit
  * cyclic redundancy check of series of bytes.
  *
  * Note: this CRC16 works for systems that process the leaast
  * significant bit of each byte first, and working to wards the most
  * significant bit.
  *
  * If you were expecting MSB first (you may not even know it) this
  * code will give you "wrong" answers.  Try using the bitrev filter.
  */
class crc16
{
public:
    enum
    {
        polynomial_ccitt = 0x1021 // also xmodem
    };

    enum seed_mode_t
    {
        seed_mode_ccitt,
        seed_mode_xmodem,
        seed_mode_broken
    };

    /**
      * The destructor.
      */
    virtual ~crc16();

    /**
      * The default constructor.
      *
      * @param seed_mode
      *     This selects which CRC16 calculation seed is to be used.
      * @param augment
      *     This is true if the 16-zero-bit augmentation is desired.
      *     This is the default.  False if no augmentation is desired.
      */
    crc16(seed_mode_t seed_mode = seed_mode_ccitt, bool augment = true,
        unsigned short polynomial = polynomial_ccitt);

    /**
      * The copy constructor.
      */
    crc16(const crc16 &);

    /**
      * The assignment operator.
      */
    crc16 &operator=(const crc16 &);

    /**
      * The get method is used to obtain the running value of the cyclic
      * redundancy check.
      */
    unsigned short get() const;

    /**
      * The next method is used to advance the state by one byte.
      */
    void next(unsigned char);

    /**
      * The nextbuf method is used to advance the state by a series of bytes.
      */
    void nextbuf(const void *, size_t);

    /**
      * The print_table method may be used to print the table being used.
      * This is principally for debugging the table generation process.
      */
    void print_table(void) const;

private:
    /**
      * The state instance variable is used to remember the running
      * value of the 16-bit cyclic redundancy check.
      */
    unsigned short state;

    /**
      * The augment instance variable is used to remember whether or
      * not the 16 zero bits of augmentation are to be processed before
      * reporting the result.
      */
    bool augment;

    /**
      * The table instance variable i sused to remember the results of 8
      * shift-and-process operations for each byte value.  Thsi is used
      * to improve efficiency.
      */
    unsigned short table[256];

    void calculate_table(unsigned short polynomial);

    inline unsigned short updcrc(unsigned char c, unsigned short state) const;
};

#endif // LIB_CRC16_H
