//
//      srecord - manipulate eprom load files
//      Copyright (C) 2001, 2003, 2006-2008 Peter Miller
//
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 3 of the License, or
//      (at your option) any later version.
//
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with this program. If not, see
//      <http://www.gnu.org/licenses/>.
//

#ifndef INCLUDE_SREC_INPUT_FILE_DEC_BINARY_H
#define INCLUDE_SREC_INPUT_FILE_DEC_BINARY_H


#include <lib/srec/input/file.h>

/**
  * The dec_binary class is used to parse a DEC Binary (PDP 11 absolute
  * loader XXDP) format file.
  */
class srec_input_file_dec_binary:
    public srec_input_file
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_input_file_dec_binary();

private:
    /**
      * A constructor.  The input is read from the named file (or
      * the standard input if the file anme is "-").
      *
      * @param file_name
      *     The name of the file to be read.
      */
    srec_input_file_dec_binary(const string &file_name);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param file_name
      *     The name of the file to be read.
      * @returns
      *     smart pointer to new instance
      */
    static pointer create(const string &file_name);

protected:
    // See base class for documentation.
    int read(srec_record &);

    // See base class for documentation.
    const char *get_file_format_name() const;

    // See base class for documentation.
    const char *mode() const;

private:
    /**
      * The get_byte method is used to fetch a byte of input, and
      * update the checksum.  We over-ride the base implementation,
      * because we use raw bytes rather than two hex digits.
      */
    int get_byte();

    /**
      * The get_word method is used to fetch a 16-bit word of input.
      * It calls the get_byte() method twice.  We over-ride the base
      * implementation, because it uses big-endian and this format
      * is little-endian.
      */
    int get_word();

    /**
      * This format has NUL characters for optional badding around
      * blocks.  This method is used to skip past such padding.
      * Returns true if there is more inoput available, or false at
      * end of file.
      */
    bool skip_nul();

    /**
      * The current_pos instance variable is used to track out
      * position within the current record.  We need todo this
      * becase DEC Binary records can be significantly longer than
      * other formats.
      */
    unsigned long current_pos;

    /**
      * The current_length instance variable is used to remember
      * the length of the current record.  It is zero if there is no
      * "current" record.
      */
    unsigned long current_length;

    /**
      * The current_address instance variable is used to track the
      * load address of the current record.  It is updated each time
      * we return a partial block, so that we alsoways return the
      * correct load address.
      */
    unsigned long current_address;

    /**
      * The copy constructor.  Do not use.
      */
    srec_input_file_dec_binary(const srec_input_file_dec_binary &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_input_file_dec_binary &operator=(const srec_input_file_dec_binary &);
};

#endif // INCLUDE_SREC_INPUT_FILE_DEC_BINARY_H
