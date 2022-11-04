//
// srecord - manipulate eprom load files
// Copyright (C) 2000, 2002, 2003, 2006-2008, 2010, 2011 Peter Miller
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at
// your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser
// General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//

#ifndef SRECORD_QUIT_EXCEPTION_H
#define SRECORD_QUIT_EXCEPTION_H

#include <srecord/quit.h>

namespace srecord
{

/**
  * The quit_exception class is use to represent a quit handler which
  * throws an exception, rather than call global exit.
  */
class quit_exception:
    public quit
{
public:
    /**
      * The destructor.
      */
    ~quit_exception() override;

    /**
      * The default constructor.
      */
    quit_exception();

    /**
      * the vomit class is used for the throw.
      */
    class vomit { };

protected:
    // see base class for documentation
    void exit(int n) override;

    // see base class for documentation
    void message_v(const char *fmt, va_list) override;

private:
    /**
      * The copy constructor.  Do not use.
      */
    quit_exception(const quit_exception &) = delete;

    /**
      * The assignment operator.  Do not use.
      */
    auto operator=(const quit_exception &) -> quit_exception & = delete;
};

};

#endif // SRECORD_QUIT_EXCEPTION_H
