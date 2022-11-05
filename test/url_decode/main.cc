//
// srecord - Manipulate EPROM load files
// Copyright (C) 2013 Peter Miller
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

#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <getopt.h>
#include <string>

#include <srecord/progname.h>
#include <srecord/string.h>
#include <srecord/versn_stamp.h>


static void
usage()
{
    const char *prog = nullptr;

    prog = srecord::progname_get();
    fprintf(stderr, "Usage: %s -decode [ <infile> [ <outfile> ]]\n", prog);
    fprintf(stderr, "       %s -encode [ <infile> [ <outfile> ]]\n", prog);
    exit(EXIT_FAILURE);
}


static void
test_url_decode()
{
    for (;;)
    {
        char buf[1000];
        if (fgets(buf, sizeof(buf), stdin) == nullptr) {
            break;
}
        size_t len = strlen(buf);
        while (len > 0 && (isspace((unsigned char)buf[len - 1]) != 0)) {
            --len;
}
        std::string s(buf, len);
        printf("%s\n", srecord::string_url_decode(s).c_str());
    }
}


static void
test_url_encode()
{
    for (;;)
    {
        char buf[1000];
        if (fgets(buf, sizeof(buf), stdin) == nullptr) {
            break;
}
        size_t len = strlen(buf);
        while (len > 0 && (isspace((unsigned char)buf[len - 1]) != 0)) {
            --len;
}
        std::string s(buf, len);
        printf("%s\n", srecord::string_url_encode(s).c_str());
    }
}


auto
main(int argc, char **argv) -> int
{
    srecord::progname_set(argv[0]);
    using func_t = void (*)();
    func_t func = nullptr;
    for (;;)
    {
        static const
        struct option options[] =
        {
            { "decode", 0, nullptr, 'd' },
            { "encode", 0, nullptr, 'e' },
            { "version", 0, nullptr, 'V' },
            { nullptr, 0, nullptr,0 }
        };
        int c = getopt_long(argc, argv, "deV", options, nullptr);
        if (c == -1) {
            break;
}

        switch ((unsigned char)c)
        {
        case 'd':
            func = test_url_decode;
            break;

        case 'e':
            func = test_url_encode;
            break;

        case 'V':
            srecord::print_version();
            return EXIT_SUCCESS;

        default:
            fprintf(stderr, "unknown '%c' option\n", c);
            usage();
            return EXIT_FAILURE;
        }
    }
    if (func == nullptr) {
        usage();
}
    switch (argc - optind)
    {
    case 0:
        break;

    case 2:
        if (freopen(argv[optind + 1], "w", stdout) == nullptr)
        {
            perror(argv[optind + 1]);
            return EXIT_FAILURE;
        }
        // fall through...

    case 1:
        if (freopen(argv[optind], "r", stdin) == nullptr)
        {
            perror(argv[optind]);
            return EXIT_FAILURE;
        }
        break;

    default:
        usage();
    }

    (*func)();

    return EXIT_SUCCESS;
}
