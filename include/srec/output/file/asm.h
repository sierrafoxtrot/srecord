//
//	srecord - manipulate eprom load files
//	Copyright (C) 2003, 2006 Peter Miller;
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
// MANIFEST: interface definition for include/srec/output/file/asm.cc
//

#ifndef INCLUDE_SREC_OUTPUT_FILE_ASM_H
#define INCLUDE_SREC_OUTPUT_FILE_ASM_H


#include <srec/output/file.h>
#include <interval.h>

/**
  * The srec_output_file_asm class is used to represent an output file
  * which emits assembler code.
  */
class srec_output_file_asm:
    public srec_output_file
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_output_file_asm();

    /**
      * The constructor.
      */
    srec_output_file_asm(const char *);

    // See base class for documentation.
    virtual void write(const srec_record &);

    // See base class for documentation.
    void line_length_set(int);

    // See base class for documentation.
    void address_length_set(int);

    // See base class for documentation.
    int preferred_block_size_get() const;

private:
    /**
      * The taddr instance variabel is used to remember the
      * termination address, to be emitted in the footer.
      */
    unsigned long taddr;

    /**
      * The range instance variable is used to remember the range
      * of addresses present in the output.
      */
    interval range;

    /**
      * The column instance variable is used to remember the current
      * printing column on the line.
      */
    int column;

    /**
      * The current_address instance variabel is used to remember
      * the current address that the file is positioned at.  This is
      * used to know whether we need to add padding.
      */
    unsigned long current_address;

    /**
      * The line_length instance variable is used to remember the
      * maximum line length.  The output usually does not exceed it.
      */
    int line_length;

    /**
      * The org_warn instance variable is used to remember if the ORG
      * directive warning comment has been issued.
      */
    bool org_warn;

    /**
      * The emit_byte method is used to emit a single byte.  It uses
      * column to track the position, so as not to exceed line_length.
      */
    void emit_byte(int);

    /**
      * The default constructor.  Do not use.
      */
    srec_output_file_asm();

    /**
      * The copy constructor.  Do not use.
      */
    srec_output_file_asm(const srec_output_file_asm &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_output_file_asm &operator=(const srec_output_file_asm &);
};

#endif // INCLUDE_SREC_OUTPUT_FILE_ASM_H
