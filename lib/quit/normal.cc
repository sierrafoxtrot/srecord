//
//      srecord - manipulate eprom load files
//      Copyright (C) 2000, 2002, 2006, 2007 Peter Miller
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

#include <cstdarg>
#include <iostream>

#include <lib/progname.h>
#include <lib/quit/normal.h>


quit_normal::quit_normal()
{
}


quit_normal::~quit_normal()
{
}


void
quit_normal::exit(int n)
{
    ::exit(n);
}


void
quit_normal::message_v(const char *fmt, va_list ap)
{
    char buf[2000];
    snprintf(buf, sizeof(buf), "%s: ", progname_get());
    size_t len = strlen(buf);
    vsnprintf(buf + len, sizeof(buf) - len, fmt, ap);

    std::cout.flush();

    int column = 0;
    char *cp = buf;
    for (;;)
    {
        unsigned char c = *cp++;
        if (!c)
            break;
        if (isspace(c) || !isprint(c))
            continue;

        std::string word;
        for (;;)
        {
            word += c;
            c = *cp;
            if (!c)
                break;
            if (isspace(c) || !isprint(c))
                break;
            ++cp;
        }
        if (!column)
        {
            std::cerr << word;
            column = word.size();
        }
        else if (column + 1 + word.size() > 80)
        {
            std::cerr << std::endl << "    ";
            column = 4;
            std::cerr << word;
            column += word.size();
        }
        else
        {
            std::cerr << ' ';
            ++column;
            std::cerr << word;
            column += word.size();
        }
    }
    std::cerr << std::endl;
    std::cerr.flush();
}


static quit_normal quit_normal_default;
quit &quit_default = quit_normal_default;
