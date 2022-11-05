//
// srecord - Manipulate EPROM load files
// Copyright (C) 2013, 2014 Peter Miller
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

#include <sstream>
#include <cstring>

#include <srecord/string.h>


static auto
nibble(unsigned x) -> char
{
    x &= 0x0F;
    if (x < 10) {
        return ('0' + x);
}
    return (('A' - 10) + x);
}


static auto
needs_quoting(unsigned char x) -> bool
{
    if (x == '%') {
        return true;
}
    if (x >= 0x80) {
        return true;
}
    return isprint(x) == 0;
}


auto
srecord::string_url_encode(const std::string &text) -> std::string
{
    const char *begin = text.c_str();
    const char *end = begin + text.size();
    const char *cur = begin;
    std::stringstream result;
    while (cur < end)
    {
        unsigned char c = *cur++;
        if (needs_quoting(c))
        {
            result << '%';
            result << nibble(c >> 4);
            result << nibble(c);
        }
        else
        {
            result << c;
        }
    }
    return result.str();
}
