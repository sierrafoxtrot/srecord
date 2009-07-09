//
// srecord - The "srecord" program.
// Copyright (C) 2007-2009 Peter Miller
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

#include <lib/bitrev.h>
#include <lib/crc16.h>
#include <lib/progname.h>
#include <lib/quit.h>


static void
usage()
{
    fprintf(stderr, "Usage: [ -av ] %s\n", progname_get());
    exit(1);
}


int
main(int argc, char **argv)
{
    progname_set(argv[0]);
    crc16::seed_mode_t seed_mode = crc16::seed_mode_ccitt;
    bool augment = true;
    unsigned short polynomial = crc16::polynomial_ccitt;
    bool print_table = false;
    crc16::bit_direction_t bitdir = crc16::bit_direction_most_to_least;
    bool h_flag = false;
    for (;;)
    {
        int c = getopt(argc, argv, "abchp:rtx");
        if (c == EOF)
            break;
        switch (c)
        {
        case 'a':
            augment = !augment;
            break;

        case 'b':
            seed_mode = crc16::seed_mode_broken;
            break;

        case 'c':
            seed_mode = crc16::seed_mode_ccitt;
            break;

        case 'h':
            h_flag = true;
            break;

        case 'p':
            polynomial = strtol(optarg, 0, 0);
            break;

        case 'r':
            bitdir = crc16::bit_direction_least_to_most;
            break;

        case 't':
            print_table = true;
            break;

        case 'x':
            seed_mode = crc16::seed_mode_xmodem;
            break;

        default:
            usage();
            // NOTREACHED
        }
    }
    if (optind != argc)
        usage();

    crc16 check(seed_mode, augment, polynomial, bitdir);
    if (print_table)
    {
        check.print_table();
        return 0;
    }
    for (;;)
    {
        char buffer[1024];
        int n = read(0, buffer, sizeof(buffer));
        if (n < 0)
            quit_default.fatal_error_errno("read stdin");
        if (!n)
            break;
        if (h_flag)
        {
            for (int j = 0; j < n; ++j)
                buffer[j] = bitrev8(buffer[j]);
        }
        check.nextbuf(buffer, n);
    }

    // The h_flags is use to validate the crc16 least-to-most code.
    // Because that code calculate the CRC bit reversed, we bit reverse
    // here so that we can test for identical-ness.
    if (h_flag)
        printf("0x%04X\n", bitrev16(check.get()));
    else
        printf("0x%04X\n", check.get());
    return 0;
}
