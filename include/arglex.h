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
 */

#ifndef INCLUDE_ARGLEX_H
#define INCLUDE_ARGLEX_H

#pragma interface


#define ARGLEX_END_MARKER {0,0}

class arglex
{
public:
	enum
	{
		token_eoln,
		token_help,
		token_license,
		token_number,
		token_option,
		token_page_width,
		token_page_length,
		token_stdio,
		token_string,
		token_tracing,
		token_verbose,
		token_version,
		token_MAX
	};
	
	struct value_ty
	{
		char	*alv_string;
		long	alv_number;
	};
	
	struct table_ty
	{
		char	*name;
		int	token;
	};

private:
	int		argc;
	char		**argv;
	int		token;
	char		*value_string_;
	long		value_number_;
	table_ty	*table;
	char		*pushback[4];
	int		pushback_depth;

protected:
	void table_set(table_ty *);

public:
	arglex();
	arglex(arglex &);
	arglex(int, char **);
	~arglex();

	int token_cur() const { return token; }
	int token_next();
	int token_first();
	const char *value_string() const { return value_string_; }
	long value_number() const { return value_number_; }
	const char *token_name(int);
	void usage() const;
	void help(const char * = 0) const;
	void version() const;
	void license() const;
	void bad_argument() const;
	void usage_tail_set(const char *);

private:
	mutable const char *usage_tail_;
	const char *usage_tail_get() const;
};

int arglex_compare(char *formal, char *actual);

#endif /* INCLUDE_ARGLEX_H */
