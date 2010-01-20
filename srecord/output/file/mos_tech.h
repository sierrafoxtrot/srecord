//
//      srecord - manipulate eprom load files
//      Copyright (C) 2000-2002, 2005-2008, 2010 Peter Miller
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

#ifndef SRECORD_OUTPUT_FILE_MOS_TECH_H
#define SRECORD_OUTPUT_FILE_MOS_TECH_H

#include <srecord/output/file.h>

/**
  * The srec_output_file_mos_tech class is used to write a MOS Technology
  * formatted file.
  */
class srec_output_file_mos_tech:
    public srec_output_file
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_output_file_mos_tech();

private:
    /**
      * The constructor.  It is private on purpose, use the #create
      * class method instead.
      *
      * @param file_name
      *     The name of the file to be written.  The special name "-"
      *     indicates the standard output is to be used.
      */
    srec_output_file_mos_tech(const std::string &file_name);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param file_name
      *     The name of the file to be written.
      */
    static pointer create(const std::string &file_name);

protected:
    // See base class for documentation.
    void write(const srec_record &);

    // See base class for documentation.
    void line_length_set(int);

    // See base class for documentation.
    void address_length_set(int);

    // See base class for documentation.
    int preferred_block_size_get() const;

    // See base class for documentation.
    const char *format_name() const;

private:
    /**
      * The pref_block_size is used to remember the preferred block
      * size.  Set by the line_length_set() method.  Read by the
      * preferred_block_size_get() method.
      */
    int pref_block_size;

    /**
      * The data_record_count instance variable is used to remember the
      * number of data records priesent in the output.  This is used
      * when writing the end-of-file record.
      */
    int data_record_count;

    /**
      * The write_inner method is used to write a single line (record)
      * to the file.  Use by the write() method.
      */
    void write_inner(int type, unsigned long addr, int addr_len,
        const void *data, int data_len);

    /**
      * The default constructor.  Do not use.
      */
    srec_output_file_mos_tech();

    /**
      * The copy constructor.  Do not use.
      */
    srec_output_file_mos_tech(const srec_output_file_mos_tech &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_output_file_mos_tech &operator=(const srec_output_file_mos_tech &);
};

#endif // SRECORD_OUTPUT_FILE_MOS_TECH_H
