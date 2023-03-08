//
// srecord - Manipulate EPROM load files
// Copyright (C) 2009-2011 Peter Miller
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at
// your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//

#ifndef SRECORD_BITREV_H
#define SRECORD_BITREV_H

#include <cstdint>

namespace srecord
{

/**
  * The bitrev8 function is used to reverse the order of the bits in an
  * 8-bit number.  This function is its own inverse.
  *
  * @param value
  *     The value to be reversed.
  */
uint8_t bitrev8(uint8_t value);

/**
  * The bitrev16 function is used to reverse the order of the bits in an
  * 16-bit number.  This function is its own inverse.
  *
  * @param value
  *     The value to be reversed.
  */
uint16_t bitrev16(uint16_t value);

/**
  * The bitrev24 function is used to reverse the order of the bits in an
  * 24-bit number.  This function is its own inverse.
  *
  * @param value
  *     The value to be reversed.
  */
uint32_t bitrev24(uint32_t value);

/**
  * The bitrev32 function is used to reverse the order of the bits in an
  * 32-bit number.  This function is its own inverse.
  *
  * @param value
  *     The value to be reversed.
  */
uint32_t bitrev32(uint32_t value);

/**
  * The bitrev40 function is used to reverse the order of the bits in an
  * 40-bit number.  This function is its own inverse.
  *
  * @param value
  *     The value to be reversed.
  */
uint64_t bitrev40(uint64_t value);

/**
  * The bitrev48 function is used to reverse the order of the bits in an
  * 48-bit number.  This function is its own inverse.
  *
  * @param value
  *     The value to be reversed.
  */
uint64_t bitrev48(uint64_t value);

/**
  * The bitrev56 function is used to reverse the order of the bits in an
  * 56-bit number.  This function is its own inverse.
  *
  * @param value
  *     The value to be reversed.
  */
uint64_t bitrev56(uint64_t value);

/**
  * The bitrev64 function is used to reverse the order of the bits in an
  * 64-bit number.  This function is its own inverse.
  *
  * @param value
  *     The value to be reversed.
  */
uint64_t bitrev64(uint64_t value);

};

#endif // SRECORD_BITREV_H
