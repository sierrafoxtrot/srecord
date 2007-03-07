//
//      srecord - manipulate eprom load files
//      Copyright (C) 2004, 2006, 2007 Peter Miller
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
// MANIFEST: interface definition for include/srec/output/file/aomf.cc
//

#ifndef INCLUDE_SREC_OUTPUT_FILE_AOMF_H
#define INCLUDE_SREC_OUTPUT_FILE_AOMF_H


#include <lib/srec/output/file.h>

/**
  * The srec_output_file_aomf class is used to represent the output
  * state of a file in Intel Absolute Object Module Format (AOMF).
  */
class srec_output_file_aomf:
    public srec_output_file
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_output_file_aomf();

    /**
      * The default constructor.  The input will be read from the
      * standard input.
      */
    srec_output_file_aomf();

    /**
      * A constructor.  The input will be read from the named file
      * (or the standard input if the file name is "-").
      */
    srec_output_file_aomf(const char *filename);

    // See base class for documentation.
    void write(const srec_record &);

    // See base class for documentation.
    virtual int preferred_block_size_get() const;

    // See base class for documentation.
    void line_length_set(int);

    // See base class for documentation.
    void address_length_set(int);

    // See base class for documentation.
    const char *mode() const;

private:
    /**
      * The emit_record method is used to emit records in the AOMF format.
      * Each has an 8-but type, a 16-bit little-endian length, a payload,
      * and an 8-bit 2s complement checksum.
      */
    void emit_record(int, const unsigned char *, size_t);

    /**
      * The module_header_record method is used to write an AOMF Module
      * Header Record.
      */
    void module_header_record(const char*);

    /**
      * The content_record method is used to write an AOMF Content Record.
      */
    void content_record(unsigned long address, const unsigned char *data,
        size_t length);

    /**
      * The module_header_record method is used to write an AOMF Module
      * End Record.
      */
    void module_end_record(const char*);

    /**
      * See base class for documentation.  We are over-riding it
      * because we use raw binary, so we call the put_char() method.
      * This method also tracks the byte_offset, so that we can
      * align to specific boundaries.  Calls the checksum_add() method.
      */
    void put_byte(unsigned char);

    /**
      * The byte_offset instance variable is used to track the location
      * in the output file.  Maintained by the put_byte() method.
      */
    unsigned long byte_offset;

    /**
      * The module_name instance variable is used to remember the
      * information form the Module Header Record for reproduction in
      * the Module End Record (they are required to agree).
      */
    string module_name;

    /**
      * See the base class for documentation.  We are over-riding
      * the base class implementation, because it is big-endian
      * and this format needs little-endian.
      */
    void put_word(int);

    /**
      * The copy constructor.  Do not use.
      */
    srec_output_file_aomf(const srec_output_file_aomf &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_output_file_aomf &operator=(const srec_output_file_aomf &);
};

#endif // INCLUDE_SREC_OUTPUT_FILE_AOMF_H
