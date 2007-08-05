//
//      srecord - manipulate eprom load files
//      Copyright (C) 2003, 2006, 2007 Peter Miller
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

#ifndef INCLUDE_SREC_INPUT_FILE_VMEM_H
#define INCLUDE_SREC_INPUT_FILE_VMEM_H


#include <lib/srec/input/file.h>

/**
  * The srec_input_file_vmem class is used to repesent the parse
  * state when reading a Verilog VMEM format input file.
  */
class srec_input_file_vmem:
    public srec_input_file
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_input_file_vmem();

    /**
      * the constructor.
      *
      * @param file_name
      *     The name of the file to be read.
      */
    srec_input_file_vmem(const string &file_name);

    // See base class for documentation.
    int read(srec_record &);

    // See base class for documentation.
    const char *get_file_format_name() const;

private:
    /**
      * The seen_some_input instance variable is used to remember whether
      * any data has been seen in the input to date.
      */
    bool seen_some_input;

    /**
      * The address instance variable is used to remember where we are
      * up to in the input file, so it may be associated with data bytes.
      */
    unsigned long address;

    /**
      * The default constructor.  Do not use.
      */
    srec_input_file_vmem();

    /**
      * The copy constructor.  Do not use.
      */
    srec_input_file_vmem(const srec_input_file_vmem &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_input_file_vmem &operator=(const srec_input_file_vmem &);
};

#endif // INCLUDE_SREC_INPUT_FILE_VMEM_H
