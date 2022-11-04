//
//      srecord - manipulate eprom load files
//      Copyright (C) 1998-2002, 2005-2010, 2013, 2014 Peter Miller
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

#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <vector>

#include <srecord/interval.h>
#include <srecord/arglex/tool.h>
#include <srecord/input/file.h>
#include <srecord/memory.h>
#include <srecord/record.h>
#include <srecord/string.h>


auto
main(int argc, char **argv) -> int
{
    srecord::arglex_tool cmdline(argc, argv);
    cmdline.token_first();
    typedef std::vector<srecord::input::pointer> infile_t;
    infile_t infile;
    bool verbose = false;

    while (cmdline.token_cur() != srecord::arglex::token_eoln)
    {
        switch (cmdline.token_cur())
        {
        default:
            cmdline.default_command_line_processing();
            continue;

        case srecord::arglex_tool::token_paren_begin:
        case srecord::arglex_tool::token_string:
        case srecord::arglex_tool::token_stdio:
        case srecord::arglex_tool::token_generator:
            infile.push_back(cmdline.get_input());
            continue;

        case srecord::arglex::token_verbose:
            verbose = true;
            break;
        }
        cmdline.token_next();
    }
    if (infile.size() == 0U)
        infile.push_back(cmdline.get_input());

    std::cout << std::hex << std::uppercase;

    //
    // Read each file and emit informative gumph.
    //
    for (auto it = infile.begin(); it != infile.end(); ++it)
    {
        srecord::input::pointer ifp = *it;
        if (infile.size() > 1U)
        {
            std::cout << std::endl;
            std::cout << ifp->filename() << ":" << std::endl;
        }
        std::cout << "Format: "
            << srecord::string_url_encode(ifp->get_file_format_name())
            << std::endl;
        srecord::record record;
        srecord::interval range;
        while (ifp->read(record))
        {
            switch (record.get_type())
            {
            case srecord::record::type_header:
                {
                    if (record.get_length() < 1U)
                        break;
                    std::cout << "Header: ";
                    std::string s(
                        (const char *)record.get_data(),
                        record.get_length()
                    );
                    s = srecord::string_quote_c(s);
                    std::cout << s << '\n';
                }
                break;

            case srecord::record::type_data:
                range +=
                    srecord::interval
                    (
                        record.get_address(),
                        record.get_address() + record.get_length()
                    );
                break;

            case srecord::record::type_execution_start_address:
                {
                    const uint32_t addr = record.get_address();
                    std::cout
                        << "Execution Start Address: "
                        << std::setfill('0') << std::setw(8) << addr
                        << std::endl;
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

        const uint32_t range_lowest  = range.get_lowest();
        const uint32_t range_highest = range.get_highest();
        int prec = 4;
        if ((range_highest > (1UL << 24)) || (range_highest == 0UL))
            prec = 8;
        else if (range_highest > (1UL << 16))
            prec = 6;
        std::cout << std::setfill('0');

        uint32_t number_bytes = 0UL;
        bool first_line = true;
        for (;;)
        {
            srecord::interval tmp = range;
            tmp.first_interval_only();
            if (first_line)
            {
                std::cout << "Data:   ";
                first_line = false;
            }
            else
            {
                std::cout << "        ";
            }
            const uint32_t lo = tmp.get_lowest();
            const uint32_t hi = tmp.get_highest();
            const auto hi_address = static_cast<uint32_t>(hi - 1U);
            std::cout
                << std::setw(prec) << lo
                << " - "
                << std::setw(prec) << hi_address;
            const auto interval_size = static_cast<uint32_t>(hi - lo);
            if(verbose)
            {
                std::cout << " (" << std::setw(prec) << interval_size<< ")";
            }
            std::cout << std::endl;

            number_bytes += interval_size;
            range -= tmp;
            if (range.empty())
                break;
        }

        if (verbose)
        {
            std::cout << "Filled: ";
            if (number_bytes == 0UL)
                std::cout << "100000000";
            else
                std::cout << std::setw(prec) << number_bytes;
            std::cout << std::endl;
            const auto range_size
                = static_cast<uint32_t>(range_highest - range_lowest);
            const double real_number_bytes
                = (number_bytes == 0UL) ? 4294967296.0 : number_bytes;
            const double real_range_size
                = (range_size == 0UL) ? 4294967296.0 : range_size;
            const double alloc_ratio = real_number_bytes/real_range_size;
            std::cout
                << std::setfill(' ')
                << std::fixed
                << std::showpoint
                << "Allocated: "
                << std::setw(6) << std::setprecision(2)
                << (alloc_ratio * 100.0) << "%"
                << "    Holes: "
                << std::setw(6) << std::setprecision(2)
                << ((1.0 - alloc_ratio) * 100.0) << "%"
                << std::endl;
        }
    }

    //
    // success
    //
    return EXIT_SUCCESS;
}
