//
//	srecord - manipulate eprom load files
//	Copyright (C) 1998, 1999, 2001-2005 Peter Miller;
//	All rights reserved.
//
//	This program is free software; you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published by
//	the Free Software Foundation; either version 2 of the License, or
//	(at your option) any later version.
//
//	This program is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.
//
//	You should have received a copy of the GNU General Public License
//	along with this program; if not, write to the Free Software
//	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111, USA.
//
// MANIFEST: operating system entry point
//

#include <arglex3.h>
#include <srec/input.h>
#include <srec/memory.h>
#include <srec/memory/walker/writer.h>
#include <srec/output.h>
#include <srec/output/file.h>

#include <iostream>
using namespace std;
#include <cstdlib>
#include <vector>


int
main(int argc, char **argv)
{
    srec_cat_arglex3 cmdline(argc, argv);
    cmdline.token_first();
    typedef vector<srec_input *> infile_t;
    infile_t infile;
    srec_output *outfile = 0;
    int line_length = 0;
    int address_length = 0;
    const char *header = 0;
    bool header_set = false;
    unsigned long start_address = 0;
    bool start_address_set = false;
    while (cmdline.token_cur() != arglex::token_eoln)
    {
	switch (cmdline.token_cur())
	{
	default:
	    cmdline.bad_argument();
	    // NOTREACHED

	case srec_arglex::token_string:
	case srec_arglex::token_stdio:
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
		cerr << "the line length " << line_length << " is invalid"
		    << endl;
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
		cerr << "the address length " << address_length
	    	    << " is invalid" << endl;
		exit(1);
	    }
	    break;

	case srec_cat_arglex3::token_data_only:
	    srec_output_file::data_only();
	    break;

	case srec_cat_arglex3::token_crlf:
	    srec_output_file::crlf();
	    break;

	case srec_cat_arglex3::token_multiple:
	    srec_memory::allow_overwriting();
	    break;

	case srec_cat_arglex3::token_header:
	    if (cmdline.token_next() != arglex::token_string)
	    {
		cerr << "the header option requires a string argument" << endl;
		exit(1);
	    }
	    header = cmdline.value_string();
	    header_set = true;
	    break;

	case srec_cat_arglex3::token_start_address:
	    cmdline.token_next();
	    start_address = cmdline.get_number("-Start_Address");
	    start_address_set = true;
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
    srec_memory *mp = new srec_memory();
    if (header_set)
	mp->set_header(header);
    for (infile_t::iterator it = infile.begin(); it != infile.end(); ++it)
    {
	srec_input *ifp = *it;
	mp->reader(ifp, true);
	delete ifp;
    }
    if (start_address_set)
	mp->set_start_address(start_address);

    //
    // Open the output file and write the remembered data out to it.
    //
    srec_memory_walker *w = new srec_memory_walker_writer(outfile);
    mp->walk(w);
    delete outfile;

    //
    // Dispose of the memory image of the data.
    // (Probably not necessary.)
    //
    delete mp;

    //
    // success
    //
    exit(0);
    return 0;
}
