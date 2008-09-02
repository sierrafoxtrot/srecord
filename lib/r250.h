//
//      srecord - manipulate eprom load files
//      Copyright (C) 2004, 2006-2008 Peter Miller
//
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 3 of the License, or
//      (at your option) any later version.
//
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with this program. If not, see
//      <http://www.gnu.org/licenses/>.
//

#ifndef LIB_R250_H
#define LIB_R250_H

/**
  * The r250 function is used to produce a 32-bit random number.
  * It repeats every 2**256 samples, long enough for most purposes.
  */
unsigned long r250(void);

#endif // LIB_R250_H
