/*
 *	srecord - manipulate eprom load files
 *	Copyright (C) 1998, 1999, 2000 Peter Miller;
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
 *	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111, USA.
 *
 * MANIFEST: operating system entry point
 */

#include <interval.h>
#include <srec/arglex.h>
#include <srec/input.h>
#include <srec/record.h>

#include <ctype.h>
#include <iostream.h>
#include <stdlib.h>
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
			cmdline.bad_argument();
			/* NOTREACHED */

		case srec_arglex::token_string:
		case srec_arglex::token_stdio:
			infile.push_back(cmdline.get_input());
			continue;
		}
		cmdline.token_next();
	}
	if (infile.size() == 0)
		infile.push_back(cmdline.get_input());

	/*
	 * Read each file and emit informative gumph.
	 */
	for (infile_t::iterator it = infile.begin(); it != infile.end(); ++it)
	{
		srec_input *ifp = *it;
		if (infile.size() > 1)
		{
			cout << endl;
			cout << ifp->filename() << ":" << endl;
		}
		cout << "Format:\t" << ifp->get_file_format_name() << endl;
		srec_record record;
		interval range;
		while (ifp->read(record))
		{
			switch (record.get_type())
			{
			case srec_record::type_header:
				if (record.get_length() < 1)
					break;
				cout << "Header:\t\"";
				for (int j = 0; j < record.get_length(); ++j)
				{
					int c = record.get_data(j) & 127;
					if (c == '\\' || c == '"')
						cout << '\\' << (char)c;
					else if (isprint(c))
						cout << (char)c;
					else
						cout.form("\\%03o", c);
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

			case srec_record::type_termination:
				cout << "Start:\t";
				cout.form("%08lX", record.get_address());
				cout << endl;
				break;

			default:
				/* ignored */
				break;
			}
		}
		if (range.empty())
		{
			cout << "Data:\tnone" << endl;
			continue;
		}
		cout << "Data:";
		int prec = 4;
		if (range.get_highest() > (1L << 24))
			prec = 8;
		else if (range.get_highest() > (1L << 16))
			prec = 6;
		for (;;)
		{
			interval tmp = range;
			tmp.first_interval_only();
			cout << "\t";
			cout.form("%0*lX", prec, tmp.get_lowest());
			cout << " - ";
			cout.form("%0*lX", prec, tmp.get_highest() - 1);
			cout << endl;
			range -= tmp;
			if (range.empty())
				break;
		}
	}

	/*
	 * success
	 */
	exit(0);
	return 0;
}
