//
//      srecord - manipulate eprom load files
//      Copyright (C) 1998, 1999, 2001-2003, 2005-2007 Peter Miller
//
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111, USA.
//

#ifndef INCLUDE_SREC_OUTPUT_FILE_C_H
#define INCLUDE_SREC_OUTPUT_FILE_C_H


#include <lib/srec/output/file.h>
#include <lib/interval.h>

/**
  * The srec_output_file_c class is used to represent an output file
  * which emits C code.
  */
class srec_output_file_c:
    public srec_output_file
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_output_file_c();

    /**
      * The constructor.
      *
      * @param file_name
      *     The file name to open to write data to.  The name "-" is
      *     understood to mean the standard output.
      */
    srec_output_file_c(const string &file_name);

protected:
    // See base class for documentation.
    virtual void write(const srec_record &);

    // See base class for documentation.
    void line_length_set(int);

    // See base class for documentation.
    void address_length_set(int);

    // See base class for documentation.
    int preferred_block_size_get() const;

    // See base class for documentation.
    void command_line(srec_arglex *cmdln);

    // See base class for documentation.
    const char *format_name() const;

private:
    /**
      * The prefix instance variable is used to remember the variable
      * name prefix to be used in the output.
      */
    string prefix;

    /**
      * The taddr instance variable is used to remember the
      * termination address, to be emitted in the footer.
      */
    unsigned long taddr;

    /**
      * The range instance variable is used to remember the range
      * of addresses present in the output.
      */
    interval range;

    /**
      * The header_done instance variable is used t remember whether
      * the emit_header method has been called.
      */
    bool header_done;

    /**
      * The column instance variable is used to remember the current
      * printing column on the line.
      */
    int column;

    /**
      * The current_address instance variabel is used to remember
      * the current address that the file is positioned at.  This is
      * used to know whether we need to add padding.
      */
    unsigned long current_address;

    /**
      * The line_length instance variable is used to remember the
      * maximum line length.  The output usually does not exceed it.
      */
    int line_length;

    /**
      * The address_length instance variable is used toremember how
      * many bytes to emit when emitting addresses.
      */
    int address_length;

    /**
      * The constant instance variable is used to remember whether or
      * not to use the "const" keyword.
      */
    bool constant;

    /**
      * The include instance variable is used to remember whether or not
      * to generate an include file.
      */
    bool include;

    /**
      * The include_file_name instance variable is used to remember the
      * name of the include file to be generated.
      */
    string include_file_name;

    /**
      * The output_word instance variable is used to remember whether or not
      * the input bytes should be emitted as word.
      */
    bool output_word;

    /**
      * The hex_style instance variable is used to remember whether or
      * not we are to output number in hexadecimal (true) or decimal
      * (false).
      */
    bool hex_style;

    /**
      * The section_style instance variable is used to remember whether
      * or not the output is to contain "sections".
      *
      * In non-section output, padding of 0xFF is used to pad the data
      * for correct addressing.  In section output, tables of addresses
      * and lenthgs are emitted, and the actual data is intended to be
      * relocated at run time.
      */
    bool section_style;

    /**
      * The emit_header method is used to emit the initial portion
      * of the array declaration.  It does nothing if header_done
      * is true.
      */
    void emit_header();

    /**
      * The emit_byte method is used to emit a single byte.  It uses
      * column to track the position, so as not to exceed line_length.
      */
    void emit_byte(int);

    /**
      * The emit_byte method is used to emit a single byte.  It uses
      * column to track the position, so as not to exceed line_length.
      */
    void emit_word(unsigned int);

    /**
      * The format_address method is used to format an address, taking
      * the hex_style and address_length instance variable settings.
      *
      * @param addr
      *     The adress to be formatted
      */
    string format_address(unsigned long addr);

    /**
      * The default constructor.  Do not use.
      */
    srec_output_file_c();

    /**
      * The copy constructor.  Do not use.
      */
    srec_output_file_c(const srec_output_file_c &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_output_file_c &operator=(const srec_output_file_c &);
};

#endif // INCLUDE_SREC_OUTPUT_FILE_C_H
