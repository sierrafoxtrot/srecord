//
//	srecord - manipulate eprom load files
//	Copyright (C) 2000-2003, 2006 Peter Miller;
//	All rights reserved.
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
// MANIFEST: interface definition for lib/srec/output/file/vmem.cc
//

#ifndef INCLUDE_SREC_OUTPUT_FILE_VMEM_H
#define INCLUDE_SREC_OUTPUT_FILE_VMEM_H


#include <srec/output/file.h>

/**
  * The srec_output_file_vmem class is used to write a file in
  * Verilog VMEM format, to be loaded using the $readmemh() call.
  */
class srec_output_file_vmem:
    public srec_output_file
{
public:
    /**
      * The destrutcor.
      */
    virtual ~srec_output_file_vmem();

    /**
      * The constructor.
      */
    srec_output_file_vmem(const char *filename);

    // See base class for documentation.
    void write(const srec_record &);

    // See base class for documentation.
    void line_length_set(int);

    // See base class for documentation.
    void address_length_set(int);

    // See base class for documentation.
    int preferred_block_size_get() const;

    // See base class for documentation.
    void command_line(srec_arglex *cmdln);

private:
    /**
      * The address instance variable is used to remember where we are
      * up to in the output.
      */
    unsigned long address;

    /**
      * The column instance variable is used to remember which column
      * we are up to in the output.  Used to limit the length of lines
      * in the output.
      */
    int column;

    /**
      * The pref_block_size instance variable is used to remember the
      * number of bytes in the preferred block size.
      */
    int pref_block_size;

    /**
      * The width_shift instance variable is used to remember the number
      * of bits to shift addresses when emitting them.	This is based
      * on the memory width passed to the constructor.
      */
    unsigned width_shift;

    /**
      * The width_mask instance variable is used to remember the lower
      * bits to the right of the addresses shift.  This is most frequently
      * used to determine the boundaries of memory-width chunks of bytes.
      * This value is simply a pre-caluculation of ((1u << width_shift) - 1u).
      */
    unsigned width_mask;

    /**
      * The write_byte method is used to write a byte value to the output
      * at the current address.  The address and column are updated,
      * and any pre- and post-processing of trhe output line is performed.
      */
    void write_byte(unsigned value);

    /**
      * The write_byte_at method is used to write a byte value to the
      * output at the specified address.  Some bytes of padding may be
      * issued (via the write_byte() method) if the current address
      * and the new address (addr) are not the same, and are not
      * nicely aligned to the memory width given to the constructor.
      * Once address equality has been achieved, the value is written
      * via the write_byte() method.
      */
    void write_byte_at(unsigned long addr, unsigned value);

    /**
      * The default constructor.  Do not use.
      */
    srec_output_file_vmem();

    /**
      * The copy constructor.  Do not use.
      */
    srec_output_file_vmem(const srec_output_file_vmem &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_output_file_vmem &operator=(const srec_output_file_vmem &);
};

#endif // INCLUDE_SREC_OUTPUT_FILE_VMEM_H
