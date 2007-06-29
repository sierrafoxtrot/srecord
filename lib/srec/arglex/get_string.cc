//
//      srecord - The "srecord" program.
//      Copyright (C) 2007 Peter Miller
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
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
//      02110-1301 USA.
//
// MANIFEST: functions to impliment the srec_arglex_get_string class
//

#include <lib/srec/arglex.h>


std::string
srec_arglex::get_string(const char *caption)
{
    if (token_cur() != token_string)
    {
        fatal_error
        (
            "string expected for %s before %s",
            caption,
            token_name(token_cur())
        );
        // NOTREACHED
    }
    std::string result = value_string();
    token_next();
    return result;
}
