//
//      srecord - manipulate eprom load files
//      Copyright (C) 2004, 2006-2008 Peter Miller
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

#ifndef INCLUDE_SREC_INPUT_FILE_AOMF_H
#define INCLUDE_SREC_INPUT_FILE_AOMF_H


#include <lib/srec/input/file.h>

/**
  * The srec_input_file_aomf class is used to parse Intel Absolute Object
  * Module Format (AOMF) file.
  */
class srec_input_file_aomf:
    public srec_input_file
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_input_file_aomf();

private:
    /**
      * The constructor.  The input is read from the named file (or
      * the standard input if the file name is "-"). It is private on
      * purpose, use the "create" class method instead.
      *
      * @param file_name
      *     The name of the file to be read.
      */
    srec_input_file_aomf(const string &file_name);

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
    bool read(srec_record &record);

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
      * The current_buffer instance variable is used to remember the
      * base of an array which buffers the current input record.
      */
    unsigned char *current_buffer;

    /**
      * The current_length instance variable is used to remember
      * the length of the current record.  It is zero if there is no
      * "current" record.
      */
    size_t current_length;

    /**
      * The current_maximum instance variable is used to remember the
      * length of the current_buffer array.  It is zero if there is no
      * "current" buffer.
      */
    size_t current_maximum;

    /**
      * The current_pos instance variable is used to remember the
      * position within the current_buffer array.
      */
    size_t current_pos;

    /**
      * The current_address instance variable is used to track the
      * load address of the current record.  It is updated each time
      * we return a partial block, so that we alsoways return the
      * correct load address.
      */
    unsigned long current_address;

    enum state_t
    {
        expecting_header,
        expecting_data,
        expecting_eof
    };

    /**
      * The state instance variable is used to remember what to expect
      * next from the file.
      */
    state_t state;

    /**
      * The slurp method is used to fill the current_buffer array,
      * and set the current_length.
      */
    int slurp(void);

    /**
      * The copy constructor.  Do not use.
      */
    srec_input_file_aomf(const srec_input_file_aomf &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_input_file_aomf &operator=(const srec_input_file_aomf &);
};

#endif // INCLUDE_SREC_INPUT_FILE_AOMF_H
