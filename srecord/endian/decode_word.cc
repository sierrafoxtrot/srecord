//
// srecord - Manipulate EPROM load files
// Copyright (C) 2012 Peter Miller
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
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

#include <srecord/endian.h>


uint16_t
srecord::decode_word_le(const uint8_t *data)
{
    return (data[0] | (data[1] << 8));
}


uint16_t
srecord::decode_word_be(const uint8_t *data)
{
    return ((data[0] << 8) | data[1]);
}


uint16_t
srecord::endian_decode_word(const uint8_t *data, endian_t order)
{
    return
        (
            order == endian_little
        ?
            decode_word_le(data)
        :
            decode_word_be(data)
        );
}
