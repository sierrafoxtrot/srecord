//
//	srecord - manipulate eprom load files
//	Copyright (C) 2000, 2002, 2003 Peter Miller;
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
// MANIFEST: interface definition for include/quit/normal.cc
//

#ifndef INCLUDE_QUIT_NORMAL_H
#define INCLUDE_QUIT_NORMAL_H

#pragma interface "quit_normal"

#include <quit.h>

/**
  * The quit_normal class is used to represent a quit handler which
  * behaves in the "normal" way: print error message son the standard
  * error, and exit via the gloibal ::exit function.
  */
class quit_normal:
    public quit
{
public:
    /**
      * The destructor.
      */
    virtual ~quit_normal();

    /**
      * The default constructor.
      */
    quit_normal();

    /**
      * The copy constructor.
      */
    quit_normal(const quit_normal &);

    /**
      * The assignment operator.
      */
    quit_normal &operator=(const quit_normal &);

    // see base class for documentation
    virtual void exit(int);

    // see base class for documentation
    virtual void message_v(const char *, va_list);
};

#endif // INCLUDE_QUIT_NORMAL_H
