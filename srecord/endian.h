//
// srecord - Manipulate EPROM load files
// Copyright (C) 2008, 2010, 2011 Peter Miller
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

};

// vim: set ts=8 sw=4 et :
#endif // SRECORD_ENDIAN_H
