//
//      srecord - manipulate eprom load files
//      Copyright (C) 1998, 1999, 2001-2008 Peter Miller
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

#include <iostream>
#include <cstdlib>
#include <vector>

#include <lib/srec/input/file.h>
#include <lib/srec/memory.h>
#include <lib/srec/memory/walker/writer.h>
#include <lib/srec/output.h>
#include <lib/srec/output/file.h>

#include <srec_cat/arglex3.h>


int
main(int argc, char **argv)
{
    srec_cat_arglex3 cmdline(argc, argv);
    cmdline.token_first();
    typedef std::vector<srec_input::pointer> infile_t;
    infile_t infile;
    srec_output::pointer outfile;
    int line_length = 0;
    int address_length = 0;
    std::string header;
    bool header_set = false;
    unsigned long execution_start_address = 0;
    bool execution_start_address_set = false;
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

        case srec_arglex::token_output:
            if (outfile)
                cmdline.usage();
            outfile = cmdline.get_output();
            continue;

        case srec_cat_arglex3::token_line_length:
            if (line_length > 0)
                cmdline.usage();
            if (cmdline.token_next() != arglex::token_number)
                cmdline.usage();
            line_length = cmdline.value_number();
            if (line_length <= 0)
            {
                std::cerr << "the line length " << line_length << " is invalid"
                    << std::endl;
                exit(1);
            }
            break;

        case srec_cat_arglex3::token_address_length:
            if (address_length > 0)
                cmdline.usage();
            if (cmdline.token_next() != arglex::token_number)
                cmdline.usage();
            address_length = cmdline.value_number();
            if (address_length <= 0 || address_length > (int)sizeof(long))
            {
                std::cerr << "the address length " << address_length
                    << " is invalid" << std::endl;
                exit(1);
            }
            break;

        case srec_cat_arglex3::token_data_only:
            srec_output_file::enable_header(false);
            srec_output_file::enable_data_count(false);
            srec_output_file::enable_goto_addr(false);
            srec_output_file::enable_footer(false);
            break;

        case srec_cat_arglex3::token_enable:
        case srec_cat_arglex3::token_disable:
            {
                int tok = cmdline.token_cur();
                bool yesno = (tok == srec_cat_arglex3::token_enable);
                cmdline.token_next();
                std::string name = cmdline.get_string(cmdline.token_name(tok));
                if (!srec_output_file::enable_by_name(name, yesno))
                {
                    std::cerr << "argument of " << cmdline.token_name(tok)
                        << "=" << name << " unknown" << std::endl;
                    cmdline.usage();
                }
            }
            continue;

        case srec_cat_arglex3::token_crlf:
            srec_output_file::line_termination_by_name("crlf");
            break;

        case srec_cat_arglex3::token_line_termination:
            {
                int tok = cmdline.token_cur();
                cmdline.token_next();
                std::string name = cmdline.get_string(cmdline.token_name(tok));
                if (!srec_output_file::line_termination_by_name(name))
                {
                    std::cerr << "line termination \"" << name << "\" unknown"
                        << std::endl;
                    cmdline.usage();
                }
            }
            continue;

        case srec_cat_arglex3::token_header:
            if (cmdline.token_next() != arglex::token_string)
            {
                std::cerr << "the header option requires a string argument"
                    << std::endl;
                exit(1);
            }
            header = cmdline.value_string();
            header_set = true;
            srec_output_file::enable_header(true);
            break;

        case srec_cat_arglex3::token_execution_start_address:
            if (execution_start_address_set)
            {
                std::cerr << "too many -execution-start-address options "
                    "specified" << std::endl;
                exit(1);
            }
            cmdline.token_next();
            execution_start_address =
                cmdline.get_number("-Execution_Start_Address");
            execution_start_address_set = true;
            srec_output_file::enable_goto_addr(true);
            continue;
        }
        cmdline.token_next();
    }
    if (infile.size() == 0)
        infile.push_back(cmdline.get_input());
    if (!outfile)
        outfile = cmdline.get_output();
    if (address_length > 0)
        outfile->address_length_set(address_length);
    if (line_length > 0)
        outfile->line_length_set(line_length);

    //
    // Read the input into memory.  This allows the data to be
    // consolidated on output, and warnings to be issued for
    // duplicates.
    //
    // It is assumed the data will all fit into memory.  This is
    // usually reasonable, because these utilities are used for
    // eproms which are usually smaller than the available virtual
    // memory of the development system.
    //
    srec_memory m;
    if (header_set)
        m.set_header(header.c_str());
    for (infile_t::iterator it = infile.begin(); it != infile.end(); ++it)
    {
        srec_input::pointer ifp = *it;
        m.reader(ifp, true);
    }
    if (execution_start_address_set)
        m.set_execution_start_address(execution_start_address);

    //
    // Open the output file and write the remembered data out to it.
    //
    srec_memory_walker::pointer w = srec_memory_walker_writer::create(outfile);
    m.walk(w);

    //
    // success
    //
    return 0;
}
