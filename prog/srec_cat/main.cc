/*
 *	srecord - manipulate eprom load files
 *	Copyright (C) 1998 Peter Miller;
 *	All rights reserved.
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program; if not, write to the Free Software
 *	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * MANIFEST: operating system entry point
 */

#include <srec/arglex.h>
#include <srec/input.h>
#include <srec/memory.h>
#include <srec/memory/walker/writer.h>
#include <srec/output.h>

#include <iostream.h>
#include <stdlib.h>
#include <vector>


static void
usage(const char *progname)
{
	cerr << "Usage: " << progname
		<< " [ <option>...][ <infile>... ]" << endl;
	exit(1);
}


int
main(int argc, char **argv)
{
	srec_arglex cmdline(argc, argv);
	cmdline.token_next();
	typedef vector<srec_input *> infile_t;
	infile_t infile;
	srec_output *outfile = 0;
	while (cmdline.token_cur() != arglex::token_eoln)
	{
		switch (cmdline.token_cur())
		{
		case srec_arglex::token_option:
			cerr << "Unknown ``" << cmdline.value_string()
				<< "'' option" << endl;
			usage(argv[0]);

		default:
			cerr << "Misplaced ``" << cmdline.value_string()
				<< "'' option" << endl;
			usage(argv[0]);

		case srec_arglex::token_string:
		case srec_arglex::token_stdio:
			infile.push_back(cmdline.get_input());
			continue;

		case srec_arglex::token_output:
			if (outfile)
				usage(argv[0]);
			outfile = cmdline.get_output();
			continue;
		}
		cmdline.token_next();
	}
	if (infile.size() == 0)
		infile.push_back(cmdline.get_input());
	if (!outfile)
		outfile = cmdline.get_output();

	/*
	 * Read the input into memory.	This allows the data to be
	 * consolidated on output, and warnings to be issued for
	 * duplicates.
	 *
	 * It is assumed the data will all fit into memory.  This is
	 * usually reasonable, because these utilities are used for
	 * eproms which are usualloy smaller than the available virtual
	 * memory of the development system.
	 */
	srec_memory *mp = new srec_memory();
	unsigned long taddr = 0;
	for (infile_t::iterator it = infile.begin(); it != infile.end(); ++it)
	{
		srec_input *ifp = *it;
		unsigned long taddr2 = mp->reader(ifp, true);
		if (taddr < taddr2)
			taddr = taddr2;
		delete ifp;
	}

	/*
	 * Open the output file and write the remembered data out to it.
	 */
	outfile->write_header();
	srec_memory_walker *w = new srec_memory_walker_writer(outfile);
	mp->walk(w);
	outfile->write_termination(taddr);
	delete outfile;

	/*
	 * Dispose of the memory image of the data.
	 * (Probably not necessary.)
	 */
	delete mp;

	/*
	 * success
	 */
	exit(0);
	return 0;
}
