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

#include <arglex.h>
#include <srec/input/file/srecord.h>
#include <srec/memory.h>
#include <srec/memory/walker/writer.h>
#include <srec/output/file/srecord.h>
#include <srec/record.h>

#include <iostream.h>
#include <stdlib.h>
#include <vector>


class arglex_here: public arglex
{
public:
	enum
	{
		token_output = arglex::token_MAX,
	};

	arglex_here(int, char **);
};

arglex_here::arglex_here(int ac, char **av)
	: arglex(ac, av)
{
	static table_ty table[] =
	{
		{ "-Output", token_output, },
		ARGLEX_END_MARKER
	};

	table_set(table);
}


static void
usage(const char *progname)
{
	cerr << "Usage: " << progname << " <file1> <file2>" << endl;
	exit(1);
}


int
main(int argc, char **argv)
{
	arglex_here cmdline(argc, argv);
	cmdline.token_next();
	const char *fn1 = 0;
	const char *fn2 = 0;
	while (cmdline.token_cur() != arglex::token_eoln)
	{
		switch (cmdline.token_cur())
		{
		case arglex::token_option:
			cerr << "Unknown ``" << cmdline.value_string()
				<< "'' option" << endl;
			usage(argv[0]);

		default:
			cerr << "Misplaced ``" << cmdline.value_string()
				<< "'' option" << endl;
			usage(argv[0]);

		case arglex::token_string:
			if (!fn1)
				fn1 = cmdline.value_string();
			else if (!fn2)
				fn2 = cmdline.value_string();
			else
				usage(argv[0]);
			break;

		case arglex::token_stdio:
			if (!fn1)
				fn1 = "-";
			else if (!fn2)
				fn2 = "-";
			else
				usage(argv[0]);
			break;
		}
		cmdline.token_next();
	}
	if (!fn1 || !fn2)
	{
		cerr << argv[0] << ": two files names required" << endl;
		usage(argv[0]);
	}

	/*
	 * Read the first file into memory.
	 */
	srec_input *if1 = new srec_input_file_srecord(fn1);
	srec_memory *mp1 = new srec_memory();
	unsigned long ta1 = mp1->reader(if1);
	delete if1;

	/*
	 * Read the second file into memory.
	 */
	srec_input *if2 = new srec_input_file_srecord(fn2);
	srec_memory *mp2 = new srec_memory();
	unsigned long ta2 = mp2->reader(if2);
	delete if2;

	/*
	 * Error message and non-zero exit status if the files differ.
	 */
	if (*mp1 != *mp2 || ta1 != ta2)
	{
		cerr << argv[0] << ": files ``" << fn1 << "'' and ``"
			<< fn2 << "'' differ" << endl;
		exit(2);
	}

	/*
	 * success
	 */
	exit(0);
	return 0;
}
