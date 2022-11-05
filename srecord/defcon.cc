//
// srecord - Manipulate EPROM load files
// Copyright (C) 2014 Peter Miller
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

#include <cstdlib>
#include <cstring>

#include <srecord/defcon.h>
#include <srecord/sizeof.h>


auto
srecord::text_from_defcon(defcon_t x) -> const char *
{
    switch (x)
    {
    default:
    case defcon_ignore:
        return "ignore";

    case defcon_warning:
        return "warning";

    case defcon_fatal_error:
        return "error";
    }
}


struct table_t
{
    const char *name;
    srecord::defcon_t value;
};


static const table_t table[] =
{
    // Keep this table in strcmp(3) order
    // suitable vim command: 'a,'b! LANG=C sort
    { "error", srecord::defcon_fatal_error },
    { "fatal", srecord::defcon_fatal_error },
    { "ignore", srecord::defcon_ignore },
    { "none", srecord::defcon_ignore },
    { "nothing", srecord::defcon_ignore },
    { "warn", srecord::defcon_warning },
    { "warning", srecord::defcon_warning },
};


static auto
compare(const void *va, const void *vb) -> int
{
    const auto *a = (const table_t *)va;
    const auto *b = (const table_t *)vb;
    return strcmp(a->name, b->name);
}


auto
srecord::defcon_from_text(const char *text) -> int
{
    table_t key = { text, defcon_ignore };
    auto *tp =
        (table_t *)
        bsearch(&key, table, SIZEOF(table), sizeof(table_t), compare);
    if (tp == nullptr) {
        return -1;
}
    return tp->value;
}
