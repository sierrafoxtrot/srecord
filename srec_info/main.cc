//
//      srecord - manipulate eprom load files
//      Copyright (C) 1998-2002, 2005-2010 Peter Miller
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
#include <srecord/arglex.h>
#include <srecord/input/file.h>
#include <srecord/memory.h>
#include <srecord/record.h>

#include <cctype>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <vector>


int
main(int argc, char **argv)
{
    srec_arglex cmdline(argc, argv);
    cmdline.token_first();
    typedef std::vector<srec_input::pointer> infile_t;
    infile_t infile;
    while (cmdline.token_cur() != arglex::token_eoln)
    {
        switch (cmdline.token_cur())
        {
        default:
            cmdline.default_command_line_processing();
            continue;

        case srec_arglex::token_paren_begin:
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
        srec_input::pointer ifp = *it;
        if (infile.size() > 1)
        {
            std::cout << std::endl;
            std::cout << ifp->filename() << ":" << std::endl;
        }
        std::cout << "Format: " << ifp->get_file_format_name() << std::endl;
        srec_record record;
        interval range;
        while (ifp->read(record))
        {
            switch (record.get_type())
            {
            case srec_record::type_header:
                if (record.get_length() < 1)
                    break;
                std::cout << "Header: \"";
                for (size_t j = 0; j < record.get_length(); ++j)
                {
                    int c = record.get_data(j) & 127;
                    if (c == '\\' || c == '"')
                        std::cout << '\\' << (char)c;
                    else if (isprint(c))
                        std::cout << (char)c;
                    else
                    {
                        char buf[16];
                        snprintf(buf, sizeof(buf), "\\%03o", c);
                        std::cout << buf;
                    }
                }
                std::cout << "\"" << std::endl;
                break;

            case srec_record::type_data:
                range +=
                    interval
                    (
                        record.get_address(),
                        record.get_address() + record.get_length()
                    );
                break;

            case srec_record::type_execution_start_address:
                {
                    std::cout << "Execution Start Address: ";
                    unsigned long addr = record.get_address();
                    char buf[16];
                    snprintf(buf, sizeof(buf), "%08lX", addr);
                    std::cout << buf << std::endl;
                }
                break;

            default:
                // ignored
                break;
            }
        }
        if (range.empty())
        {
            std::cout << "Data:   none" << std::endl;
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
                std::cout << "Data:   ";
                first_line = false;
            }
            else
                std::cout << "        ";
            char buf[32];
            unsigned long lo = tmp.get_lowest();
            snprintf(buf, sizeof(buf), "%0*lX", prec, lo);
            std::cout << buf << " - ";
            unsigned long hi = tmp.get_highest() - 1;
            snprintf(buf, sizeof(buf), "%0*lX", prec, hi);
            std::cout << buf << std::endl;
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
