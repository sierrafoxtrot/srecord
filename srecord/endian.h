//
// srecord - Manipulate EPROM load files
// Copyright (C) 2008, 2010-2012 Peter Miller
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see
// <http://www.gnu.org/licenses/>.
//

#ifndef SRECORD_ENDIAN_H
#define SRECORD_ENDIAN_H

#include <cstdint>

namespace srecord
{

enum endian_t
{
    endian_big,
    endian_little
};

/**
  * The endian_to_string function may be used to convert an endian value
  * into its string equivalent.
  *
  * @param x
  *     The value to convert
  * @returns
  *     a C string
  */
const char *endian_to_string(endian_t x);

/**
  * The decode_word_be function is used to decode a big-endian 2-byte
  * data buffer into an uint16_t value.
  *
  * @param data
  *     The data to be decoded
  */
uint16_t decode_word_be(const uint8_t *data);

/**
  * The decode_word_le function is used to decode a little-endian 2-byte
  * data buffer into an uint16_t value.
  *
  * @param data
  *     The data to be decoded
  */
uint16_t decode_word_le(const uint8_t *data);

/**
  * The decode_word_le function is used to decode a little-endian 2-byte
  * data buffer into an uint16_t value.
  *
  * @param data
  *     The data to be decoded
  * @param order
  *     The order of the bytes to be decoded.
  */
uint16_t endian_decode_word(const uint8_t *data, endian_t order);

};

#endif // SRECORD_ENDIAN_H
