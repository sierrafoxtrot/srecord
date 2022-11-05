//
// srecord - manipulate eprom load files
// Copyright (C) 1998, 1999, 2002, 2006, 2007, 2010 Peter Miller
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at your
// option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
// for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//

#include <iostream>

#include <srecord/versn_stamp.h>
#include <patchlevel.h>
#include <srecord/progname.h>


auto
srecord::version_stamp() -> const char *
{
    return PATCHLEVEL;
}


auto
srecord::copyright_years() -> const char *
{
    return COPYRIGHT_YEARS;
}

auto
srecord::git_sha1() -> const char *
{
    return GIT_SHA1;
}

void
srecord::print_version()
{
    std::cout << progname_get() << " version " << version_stamp()
              << " [git hash " << git_sha1() << "]"<< std::endl;
    std::cout << "Copyright (C) " << copyright_years() << " Peter Miller"
        << std::endl;
    std::cout << std::endl;
    std::cout << "The " << progname_get()
        << " program comes with ABSOLUTELY NO WARRANTY;" << std::endl;
    std::cout << "for details use the '" << progname_get()
        << " -LICense' command." << std::endl;
    std::cout << "The " << progname_get()
        << " program is free software, and you are welcome" << std::endl;
    std::cout << "to redistribute it under certain conditions; for"
        << std::endl;
    std::cout << "details use the '" << progname_get() << " -LICense' command."
        << std::endl;
}
