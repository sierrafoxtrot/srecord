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

#include <iostream.h>
#include <stdlib.h>
#include <vector>


static void
usage(const char *progname)
{
	cerr << "Usage: " << progname << " <file1> <file2>" << endl;
	exit(1);
}


int
main(int argc, char **argv)
{
	srec_arglex cmdline(argc, argv);
	cmdline.token_next();
	srec_input *if1 = 0;
	srec_input *if2 = 0;
	while (cmdline.token_cur() != srec_arglex::token_eoln)
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
			if (!if1)
				if1 = cmdline.get_input();
			else if (!if2)
				if2 = cmdline.get_input();
			else
			{
				cerr << argv[0]
					<< ": too many input files specified"
					<< endl;
				usage(argv[0]);
			}
			continue;
		}
		cmdline.token_next();
	}
	if (!if1 || !if2)
	{
		cerr << argv[0] << ": two input files required" << endl;
		usage(argv[0]);
	}

	/*
	 * Read the first file into memory.
	 */
	srec_memory *mp1 = new srec_memory();
	unsigned long ta1 = mp1->reader(if1);

	/*
	 * Read the second file into memory.
	 */
	srec_memory *mp2 = new srec_memory();
	unsigned long ta2 = mp2->reader(if2);

	/*
	 * Error message and non-zero exit status if the files differ.
	 */
	if (*mp1 != *mp2 || ta1 != ta2)
	{
		cerr << argv[0] << ": files ``" << if1->filename()
			<< "'' and ``" << if2->filename() << "'' differ"
			<< endl;
		exit(2);
	}

	delete if1;
	delete if2;

	/*
	 * success
	 */
	exit(0);
	return 0;
}
