//
// srecord - The "srecord" program.
// Copyright (C) 2007 Peter Miller
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at
// your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//

#include <cstdio>
#include <cstdlib>
#include <unistd.h>

#include <lib/crc16.h>
#include <lib/progname.h>
#include <lib/quit.h>


static void
usage()
{
    fprintf(stderr, "Usage: [ -x ] %s\n", progname_get());
    exit(1);
}


int
main(int argc, char **argv)
{
    progname_set(argv[0]);
    bool ccitt = true;
    for (;;)
    {
        int c = getopt(argc, argv, "x");
        if (c == EOF)
            break;
        switch (c)
        {
        case 'x':
            ccitt = false;
            break;

        default:
            usage();
            // NOTREACHED
        }
    }
    if (optind != argc)
        usage();

    crc16 check(ccitt);
    for (;;)
    {
        char buffer[1024];
        int n = read(0, buffer, sizeof(buffer));
        if (n < 0)
            quit_default.fatal_error_errno("read stdin");
        if (!n)
            break;
        check.nextbuf(buffer, n);
    }
    printf("0x%04X\n", check.get());
    return 0;
}
