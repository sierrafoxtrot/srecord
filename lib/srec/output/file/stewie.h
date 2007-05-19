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

#ifndef INCLUDE_SREC_OUTPUT_FILE_STEWIE_H
#define INCLUDE_SREC_OUTPUT_FILE_STEWIE_H


#include <lib/srec/output/file.h>

/**
  * The srec_output_file_stewie class is used to represent the current
  * state of an output file in an undocumented binary format loosely
  * based on the Motorola format.
  */
class srec_output_file_stewie:
    public srec_output_file
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_output_file_stewie();

    /**
      * The constructor.
      *
      * @param file_name
      *     The name of the file to be written.
      */
    srec_output_file_stewie(const std::string &file_name);

protected:
    // See base class for documentation.
    void write(const srec_record &);

    // See base class for documentation.
    void line_length_set(int);

    // See base class for documentation.
    virtual void address_length_set(int);

    // See base class for documentation.
    virtual int preferred_block_size_get() const;

    // See base class for documentation.
    const char *mode() const;

    // See base class for documentation.
    void put_byte(unsigned char);

    // See base class for documentation.
    const char *format_name() const;

private:
    /**
      * The data_count instance variable is used to remember how many
      * data records have occurred so far in the output file.
      */
    unsigned long data_count;

    /**
      * The address_length instance variable is used to remember the
      * minimum number of bytes to use for addresses.
      */
    int address_length;

    /**
      * The preferred_block_size instance variable is used to remember
      * the prefrred block size for records.
      */
    int preferred_block_size;

    /**
      * Write a data record.
      */
    void write_inner(int, unsigned long, int, const void *, int);

    /**
      * The default constructor.  Do not use.
      */
    srec_output_file_stewie();

    /**
      * The copy constructor.  Do not use.
      */
    srec_output_file_stewie(const srec_output_file_stewie &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_output_file_stewie &operator=(const srec_output_file_stewie &);
};

#endif // INCLUDE_SREC_OUTPUT_FILE_STEWIE_H
