//
//      srecord - manipulate eprom load files
//      Copyright (C) 2004, 2006, 2007 Peter Miller
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
// MANIFEST: functions to impliment the srec_arglex_get_interval class
//

#include <lib/interval.h>
#include <lib/srec/arglex.h>
#include <lib/srec/input.h>
#include <lib/srec/input/interval.h>


interval
srec_arglex::get_interval_inner(const char *name)
{
    switch (token_cur())
    {
    case token_paren_begin:
        {
            token_next();
            interval retval = get_interval(name);
            if (token_cur() != token_paren_end)
            {
                    cerr << "``)'' expected" << endl;
                    exit(1);
            }
            token_next();
            return retval;
        }

    default:
        {
            if (!can_get_number())
            {
                cerr << "the " << name
                    << " range requires two numeric arguments" << endl;
                exit(1);
            }
            unsigned long n1 = get_number("address range minimum");
            unsigned long n2 = 0;
            if (can_get_number())
                    n2 = get_number("address range maximum");
            if (n2 && n1 >= n2)
            {
                    cerr << "the " << name << " range " << n1
                            << ".." << n2 << " is invalid" << endl;
                    exit(1);
            }
            return interval(n1, n2);
        }

    case token_within:
        {
            token_next();
            srec_input *ifp = get_input();
            interval over = srec_input_interval(ifp);
            delete ifp;
            return over;
        }

    case token_over:
        {
            token_next();
            srec_input *ifp = get_input();
            interval over = srec_input_interval(ifp);
            delete ifp;
            return interval(over.get_lowest(), over.get_highest());
        }
    }
}


interval
srec_arglex::get_interval(const char *name)
{
    interval range;
    for (;;)
    {
        range += get_interval_inner(name);
        switch (token_cur())
        {
        case token_number:
        case token_within:
        case token_over:
        case token_paren_begin:
            continue;

        default:
            break;
        }
        break;
    }
    return range;
}
