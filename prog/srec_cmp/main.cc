//
//	srecord - manipulate eprom load files
//	Copyright (C) 1998-2003, 2005, 2006 Peter Miller
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

#include <lib/srec/arglex.h>
#include <lib/srec/input/file.h>
#include <lib/srec/memory.h>
#include <lib/srec/record.h>

#include <iostream>
using namespace std;
#include <cstdlib>
#include <vector>


static bool
start_addresses_differ(srec_record *rp1, srec_record *rp2)
{
    return (rp1 && rp2 && rp1->get_address() != rp2->get_address());
}


int
main(int argc, char **argv)
{
    srec_arglex cmdline(argc, argv);
    cmdline.usage_tail_set("<file1> <file2>");
    cmdline.token_first();
    srec_input *if1 = 0;
    srec_input *if2 = 0;
    bool verbose = false;
    while (cmdline.token_cur() != srec_arglex::token_eoln)
    {
	switch (cmdline.token_cur())
	{
	default:
	    cmdline.default_command_line_processing();
            continue;

	case srec_arglex::token_string:
	case srec_arglex::token_stdio:
	    if (!if1)
		if1 = cmdline.get_input();
	    else if (!if2)
		if2 = cmdline.get_input();
	    else
	    {
		cerr << argv[0] << ": too many input files specified" << endl;
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
	cerr << argv[0] << ": two input files required" << endl;
	cmdline.usage();
    }

    //
    // Read the first file into memory.
    //
    srec_memory *mp1 = new srec_memory();
    mp1->reader(if1);

    //
    // Read the second file into memory.
    //
    srec_memory *mp2 = new srec_memory();
    mp2->reader(if2);

    //
    // Error message and non-zero exit status if the files differ.
    //
    if (verbose)
    {
	bool different = srec_memory::compare(*mp1, *mp2);
	if
	(
	    start_addresses_differ
	    (
		mp1->get_start_address(),
		mp2->get_start_address()
	    )
	)
	{
	    cout << hex << "Start address "
		<< mp1->get_start_address()->get_address()
	       	<< " not equal to " << mp2->get_start_address()->get_address()
	       	<< "." << dec << endl;
	    different = true;
	}
	if (different)
	    exit(2);
	cerr << argv[0] << ": files ``" << if1->filename() << "'' and ``"
	    << if2->filename() << "'' are the same." << endl;
    }
    else
    {
	if
	(
	    *mp1 != *mp2
	||
	    start_addresses_differ
	    (
		mp1->get_start_address(),
		mp2->get_start_address()
	    )
	)
	{
	    cerr << argv[0] << ": files ``" << if1->filename() << "'' and ``"
		<< if2->filename() << "'' differ" << endl;
	    exit(2);
	}
    }


    delete if1;
    delete if2;

    //
    // success
    //
    exit(0);
    return 0;
}
