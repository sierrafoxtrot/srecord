//
//      srecord - manipulate eprom load files
//      Copyright (C) 1998-2002, 2005-2007 Peter Miller
//
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 3 of the License, or
//      (at your option) any later version.
//
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with this program. If not, see
//      <http://www.gnu.org/licenses/>.
//

#include <lib/interval.h>
#include <lib/srec/arglex.h>
#include <lib/srec/input/file.h>
#include <lib/srec/memory.h>
#include <lib/srec/record.h>

#include <cctype>
#include <iostream>
using namespace std;
#include <cstdlib>
#include <cstdio>
#include <vector>


int
main(int argc, char **argv)
{
    srec_arglex cmdline(argc, argv);
    cmdline.token_first();
    typedef vector<srec_input *> infile_t;
    infile_t infile;
    while (cmdline.token_cur() != arglex::token_eoln)
    {
        switch (cmdline.token_cur())
        {
        default:
            cmdline.default_command_line_processing();
            continue;

        case srec_arglex::token_string:
        case srec_arglex::token_stdio:
        case srec_arglex::token_generator:
            infile.push_back(cmdline.get_input());
            continue;
        }
        cmdline.token_next();
    }
    if (infile.size() == 0)
            infile.push_back(cmdline.get_input());

    //
    // Read each file and emit informative gumph.
    //
    for (infile_t::iterator it = infile.begin(); it != infile.end(); ++it)
    {
        srec_input *ifp = *it;
        if (infile.size() > 1)
        {
            cout << endl;
            cout << ifp->filename() << ":" << endl;
        }
        cout << "Format: " << ifp->get_file_format_name() << endl;
        srec_record record;
        interval range;
        while (ifp->read(record))
        {
            switch (record.get_type())
            {
            case srec_record::type_header:
                if (record.get_length() < 1)
                    break;
                cout << "Header: \"";
                for (int j = 0; j < record.get_length(); ++j)
                {
                    int c = record.get_data(j) & 127;
                    if (c == '\\' || c == '"')
                        cout << '\\' << (char)c;
                    else if (isprint(c))
                        cout << (char)c;
                    else
                    {
                        char buf[16];
                        snprintf(buf, sizeof(buf), "\\%03o", c);
                        cout << buf;
                    }
                }
                cout << "\"" << endl;
                break;

            case srec_record::type_data:
                range +=
                    interval
                    (
                        record.get_address(),
                        record.get_address() + record.get_length()
                    );
                break;

            case srec_record::type_start_address:
                {
                    cout << "Start:  ";
                    char buf[16];
                    snprintf(buf, sizeof(buf), "%08lX", record.get_address());
                    cout << buf << endl;
                }
                break;

            default:
                // ignored
                break;
            }
        }
        if (range.empty())
        {
            cout << "Data:   none" << endl;
            continue;
        }
        int prec = 4;
        if (range.get_highest() > (1L << 24))
            prec = 8;
        else if (range.get_highest() > (1L << 16))
            prec = 6;
        bool first_line = true;
        for (;;)
        {
            interval tmp = range;
            tmp.first_interval_only();
            if (first_line)
            {
                cout << "Data:   ";
                first_line = false;
            }
            else
                cout << "        ";
            char buf[32];
            snprintf(buf, sizeof(buf), "%0*lX", prec, tmp.get_lowest());
            cout << buf << " - ";
            snprintf(buf, sizeof(buf), "%0*lX", prec, tmp.get_highest() - 1);
            cout << buf << endl;
            range -= tmp;
            if (range.empty())
                break;
        }
    }

    //
    // success
    //
    exit(0);
    return 0;
}
