//
// srecord - manipulate eprom load files
// Copyright (C) 2012 Peter Miller
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at your
// option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
// for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
//
//      From: Hendrik Beijeman <hbeyeman@gmail.com>
//      To: pmiller@opensource.org.au
//      Subject: Re: patch for srecord 1.59
//      Date: Sat, 05 May 2012 06:26:02 +0200 (05/05/12 14:26:02)
//
//      I hereby formally assign all copyright to the author of srecord,
//      Peter Miller.
//

#ifndef SRECORD_STM32_H
#define SRECORD_STM32_H

#include <cstddef>
#include <cstdint>

namespace srecord
{

/**
  * The stm32 class is used to represent the running value of a 32-bit
  * cyclic redundancy check of series of bytes, for the STM32 series of
  * microcontrollers.
  * http://www.st.com/
  */
class stm32
{
public:
    /**
      * The destructor.
      */
    virtual ~stm32() = default;

    /**
      * The default constructor.
      */
    stm32() = default;

    /**
      * The copy constructor.
      */
    stm32(const stm32 &);

    /**
      * The assignment operator.
      */
    stm32 &operator=(const stm32 &);

    /**
      * The get method is used to obtain the running value of the cyclic
      * redundancy check.
      */
    uint32_t get() const;

    /**
      * The next method is used to advance the state by one byte.
      */
    void next(uint8_t c);

    /**
      * The nextbuf method is used to advance the state by a series of bytes.
      */
    void nextbuf(const void *data, size_t data_size);

    /**
      * Word size on the STM32
      */
    static const size_t wordsize = 4;

private:
    /**
      * Run the generator on the filled buffer.  The generator assumes
      * the incoming byte stream is in little-endian order, which is safe
      * considering this is for the STM32F series MPUs.
      */
    void generator();

    /**
      * The state instance variable is used to remember the running
      * value of the 32-bit cyclic redundancy check.
      */
    uint32_t state{0xFFFFFFFF};

    /**
      * Current counter of the byte feeding
      */
    size_t cnt{0};

    /**
      * Buffer the incoming stream to build a word to feed to the
      * CRC generator.
      */
    uint8_t buf[wordsize]{};
};

};

#endif // SRECORD_CRC32_H
