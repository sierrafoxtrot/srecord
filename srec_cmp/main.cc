//
//      srecord - manipulate eprom load files
//      Copyright (C) 1998-2003, 2005-2008 Peter Miller
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

#include <lib/srec/arglex.h>
#include <lib/srec/input/file.h>
#include <lib/srec/memory.h>
#include <lib/srec/record.h>

#include <iostream>
#include <cstdlib>
#include <vector>


static bool
execution_start_addresses_differ(srec_record *rp1, srec_record *rp2)
{
    return (rp1 && rp2 && rp1->get_address() != rp2->get_address());
}


int
main(int argc, char **argv)
{
    srec_arglex cmdline(argc, argv);
    cmdline.usage_tail_set("<file1> <file2>");
    cmdline.token_first();
    srec_input::pointer if1;
    srec_input::pointer if2;
    bool verbose = false;
    while (cmdline.token_cur() != srec_arglex::token_eoln)
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
            if (!if1)
                if1 = cmdline.get_input();
            else if (!if2)
                if2 = cmdline.get_input();
            else
            {
                std::cerr << argv[0] << ": too many input files specified"
                    << std::endl;
                cmdline.usage();
            }
            continue;

        case arglex::token_verbose:
            verbose = true;
            break;
        }
        cmdline.token_next();
    }
    if (!if1 || !if2)
    {
        std::cerr << argv[0] << ": two input files required" << std::endl;
        cmdline.usage();
    }

    //
    // Read the first file into memory.
    //
    srec_memory m1;
    m1.reader(if1);

    //
    // Read the second file into memory.
    //
    srec_memory m2;
    m2.reader(if2);

    //
    // Error message and non-zero exit status if the files differ.
    //
    if (verbose)
    {
        bool different = srec_memory::compare(m1, m2);
        if
        (
            execution_start_addresses_differ
            (
                m1.get_execution_start_address(),
                m2.get_execution_start_address()
            )
        )
        {
            std::cout << std::hex << "Execution start address "
                << m1.get_execution_start_address()->get_address()
                << " not equal to "
                << m2.get_execution_start_address()->get_address()
                << "." << std::dec << std::endl;
            different = true;
        }
        if (different)
            exit(2);
        std::cerr << argv[0] << ": files \"" << if1->filename() << "\" and \""
            << if2->filename() << "\" are the same." << std::endl;
    }
    else
    {
        if
        (
            m1 != m2
        ||
            execution_start_addresses_differ
            (
                m1.get_execution_start_address(),
                m2.get_execution_start_address()
            )
        )
        {
            std::cerr << argv[0] << ": files \"" << if1->filename()
                << "\" and \"" << if2->filename() << "\" differ" << std::endl;
            exit(2);
        }
    }

    //
    // success
    //
    return 0;
}
