//
//	srecord - manipulate eprom load files
//	Copyright (C) 1998, 1999, 2002, 2003 Peter Miller;
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
// MANIFEST: interface definition for the lib/common/arglex.cc file
//

#ifndef INCLUDE_ARGLEX_H
#define INCLUDE_ARGLEX_H

#pragma interface "arglex"

#include <vector>

#define ARGLEX_END_MARKER {0,0}

/**
  * The arglex class is used to implement a lexical analizer for command
  * line arguments.  Each comamnd line argument is analized to determine
  * if it is a number, a string (file name) or an option.
  */
class arglex
{
public:
	/**
	  * The "normal" command line tokens common to all programs.
	  */
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

	/**
	  * The value_ty struct is used to represent the value of a
	  * command line argument.
	  */
	struct value_ty
	{
		/**
		  * The literal string value of the token.
		  */
		char	*alv_string;

		/**
		  * The integer vale of the token.  Only meaningful
		  * for token_number, otherwise zero.
		  */
		long	alv_number;
	};

	/**
	  * The table_ty struct is used to repesent a row of a
	  * commandline option table, used to match option names with
	  * their corresponding tokens.
	  */
	struct table_ty
	{
		/**
		  * The name of the commen line option.
		  */
		char	*name;

		/**
		  * The corresponding token.
		  */
		int	token;
	};

private:
	/**
	  * The argc instance variable tracks trhe length of the
	  * command line.  As command line arguments are processed,
	  * argc decreases.
	  */
	int argc;

	/**
	  * The argv instance variable tracks the values of the strings
	  * which are the command line arguments.  As command line
	  * arguments are processed, argv increases.
	  */
	char **argv;

	/**
	  * The token instance variable tracks the current token in the
	  * parse sequence.
	  */
	int token;

	/**
	  * The value_string_ instance variable tracks the value of the
	  * current command line argument.
	  */
	char *value_string_;

	/**
	  * The value_number_ instance variable tracks the numeric value
	  * of the current command line argument.  Usually zero unless
	  * the current command line argument is a number.
	  */
	long value_number_;

	/**
	  * The table_ptr_vec_t type is used to declare the `tables'
	  * instance variable.  Also used to simplify the code use to
	  * manipulate the `tables' instance variable.
	 */
	typedef vector<table_ty *> table_ptr_vec_t;

	/**
	  * The tables instance variable tracks the command line token
	  * tables to be scanned to determine if a command line argument
	  * is a particular token.  There is usually one per derived
	  * class.  Append more tables with the `table_set' method.
	  */
	table_ptr_vec_t	tables;

	/**
	  * The pushback instance variable tracks command line argument
	  * (or, often, portions of command line arguments) which have
	  * been "pushed back" to be re-scanned later.
	  */
	char *pushback[4];

	/**
	  * The pushback_depth instance variable tracks how many entries
	  * have been used in the `pushback' array.
	  */
	int pushback_depth;

protected:
	/**
	  * The table_set method is used to append more command line
	  * token tables to the list of tables to be scanned.  Usually one
	  * per derived class.
	  */
	void table_set(table_ty *);

public:
	/**
	  * The default constructor.
	  */
	arglex();

	/**
	  * The copy constructor.
	  */
	arglex(arglex &);

	/**
	  * The normal constructor.  The argv and argv should be those
	  * passed to main().  Not manipulation is required.
	  */
	arglex(int argc, char **argv);

	/**
	  * The destructor.
	  */
	virtual ~arglex();

	/**
	  * The token_cur method is used to get the type of the current
	  * token.
	  */
	int token_cur() const { return token; }

	/**
	  * The token_next method is used to advance to the next command
	  * line option and determine what type of token it is.
	  * It returns the type of the token; this value may also be
	  * fetched using the token_cur method.
	  */
	int token_next();

	/**
	  * The token_first method is used to fetch the fisrt command
	  * like token (rather than use the token_next method).  This does
	  * standard "help" and "version" options.
	  */
	int token_first();

	/**
	  * The value_string method is used to get the string value of
	  * the current token.
	  */
	const char *value_string() const { return value_string_; }

	/**
	  * The value_number method is used to get the numeric value of
	  * the current token.
	  */
	long value_number() const { return value_number_; }

	/**
	  * The toke_name method is used to turn a token type number
	  * into an equivalent string.	useful for some error messages.
	  */
	const char *token_name(int);

	/**
	  * The usage method is used to print a usage summary.
	  * This is a fatal error; this method does not return.
	  */
	void usage() const;

	/**
	  * The help method is used to print a helpmessage.
	  */
	void help(const char * = 0) const;

	/**
	  * The version method is used to print a version message.
	  */
	void version() const;

	/**
	  * The license method is used to print the license conditions
	  * of the program.
	  */
	void license() const;

	/**
	  * The bad_argument method is used to print an error message
	  * when the current token is inappropriate, then it calls usage,
	  * which exits.  This method does not return.
	  */
	void bad_argument() const;

	/**
	  * The usage_tail_set method is used to set the end of the
	  * command line to be printed as part of the usage method.
	  */
	void usage_tail_set(const char *);

private:
	/**
	  * The usage_tail_ instance variable tracks the end part of
	  * the command line printed by the `usage' method.
	  * Defaults to the empty string.
	  */
	mutable const char *usage_tail_;

	/**
	  * The usage_tail_get method is used to get the tail end of
	  * the command line to be printed by the `usage' method.
	  */
	const char *usage_tail_get() const;
};

/**
  * The arglex_compare function is used to compare a command line string
  * with a formal spec of the option, to see if they compare equal.
  *
  * The actual is case-insensitive.  Uppercase in the formal means a
  * mandatory character, while lower case means optional.  Any number of
  * consecutive optional characters may be supplied by actual, but none
  * may be skipped, unless all are skipped to the next non-lower-case
  * letter.
  *
  * The underscore (_) is like a lower-case minus, it matches "", "-"
  * and "_".
  *
  * The "*" in a pattern matches everything to the end of the line,
  * anything after the "*" is ignored.  The rest of the line is pointed
  * to by the "partial" variable as a side-effect (else it will be 0).
  * This rather ugly feature is to support "-I./dir" type options.
  *
  * A backslash in a pattern nominates an exact match required, case
  * must matche excatly here.  This rather ugly feature is to support
  * "-I./dir" type options.
  *
  * For example: "-project" and "-P' both match "-Project", as does
  * "-proJ", but "-prj" does not.
  *
  * For example: "-devDir" and "-d_d' both match "-Development_Directory",
  * but "-dvlpmnt_drctry" does not.
  *
  * For example: to match include path specifications, use a pattern
  * such as "-\\I*", and the partial global variable will have the path
  * in it on return.
  */
bool arglex_compare(char *formal, char *actual);

#endif // INCLUDE_ARGLEX_H
