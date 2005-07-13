//
//	srecord - manipulate eprom load files
//	Copyright (C) 1998-2000, 2002, 2003, 2005 Peter Miller;
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
// MANIFEST: interface definition for lib/srec/input.cc
//

#ifndef INCLUDE_SREC_INPUT_H
#define INCLUDE_SREC_INPUT_H

#pragma interface "srec_input"

#include <string>
using namespace std;
#include <stdarg.h>
#include <format_printf.h>


class quit; // forward
class srec_arglex; // forward

/**
  * The srec_input class is used to represent an abstract EPROM load
  * file source.  It could be one of many file formats, or a chain of
  * filters applied to an input file.
  */
class srec_input
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_input();

    /**
      * The read method is used to read one record from the input.
      * It returns 0 at the end of the input, and 1 if a record is
      * read successfully.
      *
      * See the srec_record documentation (header file) for details
      * of the various record types.
      *
      * Note: there is no guarantee that a header record will appear
      * first, or that a start address record will appear last.
      */
    virtual int read(class srec_record &) = 0;

    /**
      * The fatal_error method is used to report problems parsing
      * the file.  Do not put a newline at the end of the message.
      * Usually called from within derived class methods.  This method
      * does not return.
      *
      * The file name and line number are automatically included
      * in the message.  The filename_and_line method is called to
      * determine them.
      */
    virtual void fatal_error(const char *, ...) const
							FORMAT_PRINTF(2, 3);
    /**
      * The fatal_error_errno method is used to report problems
      * reading the input file.  Do not put a newline at the end
      * of the message.  The string equivalent of errno is appended
      * to the error message.  This method does not return.
      *
      * The file name and line number are automatically included
      * in the message.  The filename_and_line method is called to
      * determine them.
      */
    virtual void fatal_error_errno(const char *, ...) const
							FORMAT_PRINTF(2, 3);
    /**
      * The warning method is used to report potential (but non-fatal)
      * problems parsing the file.	Do not put a newline at the
      * end of the message.  Usually called from within derived
      * class methods.
      *
      * The file name and line number are automatically included
      * in the message.  The filename_and_line method is called to
      * determine them.
      */
    virtual void warning(const char *, ...) const
							FORMAT_PRINTF(2, 3);
    /**
      * The filename method is used to get the name of the input file
      * being processed.  Derived classes must supply this method.
      */
    virtual const string filename() const = 0;

    /**
      * The filename_and_line method is used to get the name
      * and current line number within the file.  The default
      * implementation simply calls the filename method and returns
      * that.  Text formats should be cleverer.
      */
    virtual const string filename_and_line() const;

    /**
      * The get_file_format_name method is used to find out the name
      * of the file format being read.  Derived classes must supply
      * this method.
      */
    virtual const char *get_file_format_name() const = 0;

    /**
      * The set_quit method is used to set the disposition of the
      * error messages, and the "exit" implementation.  The default
      * is to write error messages on the standard error, and to
      * exit using the standard C exit function.
      */
    void set_quit(quit &);

    /**
      * The reset_quit method is used to cause the disposition of
      * the error messages, and the "exit" back to the default.
      */
    void reset_quit();

    /**
      * The disable_checksum_validation method is used to have this
      * input stream ignore checksum errors.
      */
    virtual void disable_checksum_validation() = 0;

    /**
      * The command_line method is used by arglex_srec::get_input when
      * parsing the command line, to give the format an opportunity
      * to grab extra arguments off the command line.  The default
      * implementation does nothing.
      *
      * @param cmdln
      *     Where to obtain information about the curreent parse stat of
      *     the command line.
      */
    virtual void command_line(srec_arglex *cmdln);

private:
    /**
      * The quitter instance variable is used to remember how to quit.
      * It is set by the set_quit and reset_quit.  It is used by
      * the fatal_error, fatal_error_with_errno and warning methods.
      */
    quit *quitter;

protected:
    /**
      * The default constructor.  Only derived classes may call.
      */
    srec_input();

private:
    /**
      * The copy constructor.  Do not use.
      */
    srec_input(const srec_input &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_input &operator=(const srec_input &);
};

#endif // INCLUDE_SREC_INPUT_H
