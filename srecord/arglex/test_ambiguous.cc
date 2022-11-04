//
// srecord - Manipulate EPROM load files
// Copyright (C) 2013 Peter Miller
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

#include <cstdio>
#include <cstdlib>

#include <srecord/arglex.h>


void
srecord::arglex::test_ambiguous()
    const
{
    int number_of_errors = 0;
    for
    (
        auto it1 = tables.begin();
        it1 != tables.end();
        ++it1
    )
    {
        for (const table_ty *tp1 = *it1; tp1->name; ++tp1)
        {
            std::string abbrev = abbreviate(tp1->name);

            for
            (auto tp2 : tables)
            {
                for (; tp2->name; ++tp2)
                {
                    if
                    (
                        tp1->token != tp2->token
                    &&
                        compare(tp2->name, abbrev.c_str())
                    )
                    {
                        fprintf
                        (
                            stderr,
                            "option %s is ambiguous (%s vs %s)\n",
                            abbrev.c_str(),
                            tp1->name,
                            tp2->name
                        );
                        ++number_of_errors;
                    }
                }
            }
        }
    }
    if (number_of_errors > 0)
    {
        fprintf
        (
            stderr,
            "found %d fatal error%s\n",
            number_of_errors,
            (number_of_errors == 1 ? "" : "s")
        );
        exit(1);
    }
}
