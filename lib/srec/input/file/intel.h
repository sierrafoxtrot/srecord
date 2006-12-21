//
//	srecord - manipulate eprom load files
//	Copyright (C) 1998-2000, 2002, 2003, 2006 Peter Miller
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
// MANIFEST: interface definition for srec/input/file/intel.cc
//

#ifndef INCLUDE_SREC_INPUT_FILE_INTEL_H
#define INCLUDE_SREC_INPUT_FILE_INTEL_H


#include <lib/srec/input/file.h>

/**
  * The srec_input_file_intel class is used to represent the parse state
  * of an Intel Hex formatted file.
  */
class srec_input_file_intel:
    public srec_input_file
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_input_file_intel();

    /**
      * The constructor.
      */
    srec_input_file_intel(const char *filename);

    // See base class for documentation.
    int read(srec_record &);

    // See base class for documentation.
    const char *get_file_format_name() const;

private:
    /**
      * Read one record from the file.  The read method is a wrapper
      * around this method.
      */
    int read_inner(srec_record &);

    /**
      * The data_record_count instance variable is used to remember the
      * number of data records seen to date.
      */
    int data_record_count;

    /**
      * The garbage_warning instance variable is used to remember wherther
      * or not a warning has already been issued about garbage lines
      * of input.
      */
    bool garbage_warning;

    /**
      * The seen_some_input instance variable is used to remember whether
      * or not the file contains any data.
      */
    bool seen_some_input;

    /**
      * The termination_seen instance variable is used to remember
      * whether or not a termination record has been seen yet.
      */
    bool termination_seen;

    /**
      * The mode instance variable is used to remember what addressing
      * mode the file is currently in.
      */
    enum { linear, segmented } mode;

    /**
      * The address_base instance variable is used to remember the
      * segment base address when in segmented addressing mode.
      */
    unsigned long address_base;

    /**
      * The pushback instance variable is used to remember the previous
      * record in the file.  This is needed in some instances, but not always.
      */
    srec_record *pushback;

    /**
      * The end_seen instance variable is used to remember whether or
      * not the end of file has been seen yet.
      */
    bool end_seen;

    /**
      * The default constructor.  Do not use.
      */
    srec_input_file_intel();

    /**
      * The copy constructor.  Do not use.
      */
    srec_input_file_intel(const srec_input_file_intel &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_input_file_intel &operator=(const srec_input_file_intel &);
};

#endif // INCLUDE_SREC_INPUT_FILE_INTEL_H
