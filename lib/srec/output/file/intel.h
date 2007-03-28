//
//      srecord - manipulate eprom load files
//      Copyright (C) 1998, 1999, 2001-2003, 2006, 2007 Peter Miller
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
// MANIFEST: interface definition for lib/srec/output/file/intel.cc
//

#ifndef INCLUDE_SREC_OUTPUT_FILE_INTEL_H
#define INCLUDE_SREC_OUTPUT_FILE_INTEL_H


#include <lib/srec/output/file.h>

/**
  * The srec_output_file_intel class is used to write an EPROM load file
  * in Intel Hex format.
  */
class srec_output_file_intel:
    public srec_output_file
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_output_file_intel();

    /**
      * The constructor.
      *
      * @param file_name
      *     The name of the file to be written.
      */
    srec_output_file_intel(const string &file_name);

    // See base class for documentation.
    virtual void write(const srec_record &);

    // See base class for documentation.
    virtual void line_length_set(int);

    // See base class for documentation.
    virtual void address_length_set(int);

    // See base class for documentation.
    virtual int preferred_block_size_get() const;

private:
    /**
      * The write_inner method is used toi write a single line to the
      * output file.
      */
    void write_inner(int, unsigned long, const void *, int);

    /**
      * The address_base instance variable is used to remember the
      * current position within the output file
      */
    unsigned long address_base;

    /**
      * The pref_block_size instance variable is used to remember the
      * preferred number of bytes on each line of the output file.
      */
    int pref_block_size;

    /**
      * The mode instance variable is used to remember what addressing
      * mode the file is currently in.  If set to "segmented" (via the
      * address_length_set method) you get 16-bit MCS-86 output (record
      * type 2, extended segment address record).  The default value of
      * "linear" gets you 32-bit output (record type 4, extended linear
      * address record).
      */
    enum { linear, segmented } mode;

    /**
      * The default constructor.  Do not use.
      */
    srec_output_file_intel();

    /**
      * The copy constructor.  Do not use.
      */
    srec_output_file_intel(const srec_output_file_intel &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_output_file_intel &operator=(const srec_output_file_intel &);
};

#endif // INCLUDE_SREC_OUTPUT_FILE_INTEL_H
