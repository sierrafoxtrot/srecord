//
//      srecord - manipulate eprom load files
//      Copyright (C) 1998, 1999, 2002, 2003, 2006, 2007 Peter Miller
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
//      Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111, USA.
//
// MANIFEST: interface definition for lib/srec/input/interval.cc
//

#ifndef INCLUDE_SREC_INPUT_INTERVAL_H
#define INCLUDE_SREC_INPUT_INTERVAL_H

/**
  * The srec_input_interval function is used to read an input stream and
  * build an interval based on the addresses which contain data within
  * the input.
  */
class interval srec_input_interval(class srec_input *);

#endif // INCLUDE_SREC_INPUT_INTERVAL_H
