//
//	srecord - manipulate eprom load files
//	Copyright (C) 2000 Hendrik De Vloed - hendrik.devloed@rug.ac.be
//	Copyright (C) 2006 Peter Miller
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
// MANIFEST: interface definition for lib/srec/output/file/vhdl.cc
//

#ifndef INCLUDE_SREC_OUTPUT_FILE_VHDL_H
#define INCLUDE_SREC_OUTPUT_FILE_VHDL_H


#include <srec/output/file.h>

/**
  * The srec_output_file_vhdl class is used to represent the state out
  * an output file in VHDL format.
  */
class srec_output_file_vhdl:
    public srec_output_file
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_output_file_vhdl();

    /**
      * The constructor.
      *
      * \param filename
      *     The name of the file to be written.
      */
    srec_output_file_vhdl(const char *filename);

    // See base class for documentation
    void write(const srec_record &);

    // See base class for documentation
    void line_length_set(int);

    // See base class for documentation
    void address_length_set(int);

    // See base class for documentation
    int preferred_block_size_get() const;

    // See base class for documentation
    void command_line(srec_arglex *cmdln);

private:
    /**
      * The bytes_per_word instance variable is used to remember how
      * many bytes to emit per output row.  This is because words are
      * packed into an arry of values larger than a single byte.
      */
    unsigned bytes_per_word;

    /**
      * The prefix instance variable is used to remember the prefix of
      * the various names emitted into the output.
      */
    string prefix;

    /**
      * The fubar instance variable is used to remember whether the
      * emit_header method has already been called.
      */
    bool header_done;

    /**
      * The current_address instance variable is used to remember where
      * we are up to in remebering data.  Note that it is potentially
      * <i>within</i> the current_word.
      */
    unsigned long current_address;

    /**
      * The current_byte instance variable is used to remember where we
      * are located within the current_word.  The value will be in the
      * range 0..(bytes_per_word-1).
      */
    unsigned current_byte;

    /**
      * The current_word instance variable is used to remember the
      * accumulated bytes values, modulo bytes_per_word.
      */
    unsigned long current_word;

    /**
      * The emit_header method is used to emit the file header,
      * if necessary.
      */
    void emit_header();

    /**
      * The emit_word method is used to add another byte to the outout.
      * It will be accumulated withing current_word until we have
      * bytes_per_word bytes, and then emitted as a row of output.
      */
    void emit_word(unsigned long address, unsigned value);

    /**
      * The default constructor.  Do not use.
      */
    srec_output_file_vhdl();

    /**
      * The copy constructor.  Do not use.
      */
    srec_output_file_vhdl(const srec_output_file_vhdl &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_output_file_vhdl &operator=(const srec_output_file_vhdl &);
};

#endif // INCLUDE_SREC_OUTPUT_FILE_VHDL_H
