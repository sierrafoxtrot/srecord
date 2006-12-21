//
//	srecord - manipulate eprom load files
//	Copyright (C) 2003, 2006 Peter Miller
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
// MANIFEST: interface definition for include/srec/output/file/fairchild.cc
//

#ifndef INCLUDE_SREC_OUTPUT_FILE_FAIRCHILD_H
#define INCLUDE_SREC_OUTPUT_FILE_FAIRCHILD_H


#include <lib/srec/output/file.h>

/**
  * The srec_output_file_fairchild class is used to represent the running
  * state when writing a file in Fairchild Fairbug format.
  */
class srec_output_file_fairchild:
    public srec_output_file
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_output_file_fairchild();

    /**
      * The constructor.
      */
    srec_output_file_fairchild(const char *filename);

    // See base class for documentation.
    void write(const srec_record &);

    // See base class for documentation.
    void line_length_set(int);

    // See base class for documentation.
    void address_length_set(int);

    // See base class for documentation.
    int preferred_block_size_get() const;

private:
    /**
      * The emit method is used to emit a single byte to the output.
      * This method maintains the state of where we are in relation to
      * the output record structure.
      */
    void emit(unsigned long address, unsigned char value);

    /**
      * The put_nibble method puts a single hex-digit on the output.
      * We override the one in the base class because the checksum is
      * nibble-based, not byte-based.
      */
    void put_nibble(unsigned);

    /**
      * The put_byte method puts two hex-digits to the output.
      * We override the one in the base class because the checksum is
      * nibble-based, not byte-based.
      */
    void put_byte(unsigned char);

    /**
      * The address instance variable is used to remember the current
      * memory position within the output.
      */
    unsigned long address;

    /**
      * The data_bytes_emitted instance variable is used by the emit
      * method to remember how many bytes have been emitted to the
      * current output record.  (They are always exactly 8 bytes long.)
      */
    unsigned data_bytes_emitted;

    /**
      * The default constructor.  Do not use.
      */
    srec_output_file_fairchild();

    /**
      * The copy constructor.  Do not use.
      */
    srec_output_file_fairchild(const srec_output_file_fairchild &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_output_file_fairchild &operator=(const srec_output_file_fairchild &);
};

#endif // INCLUDE_SREC_OUTPUT_FILE_FAIRCHILD_H
