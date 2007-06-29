//
//      srecord - manipulate eprom load files
//      Copyright (C) 1998-2003, 2005-2007 Peter Miller
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
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
//      02110-1301 USA.
//

#ifndef INCLUDE_SREC_OUTPUT_FILE_H
#define INCLUDE_SREC_OUTPUT_FILE_H


#include <string>
#include <lib/srec/output.h>
#include <lib/format_printf.h>


/**
  * The srec_output_file class is used to represent a generic output file.
  * It provides a numnber of services useful to many output file formats.
  */
class srec_output_file:
    public srec_output
{
public:
    /**
      * The default constructor.
      * Output will be sent to the standard output.
      */
    srec_output_file();

    /**
      * The constructor.  The output will be sent to the named file (or
      * the standard output if the file name is "-").
      *
      * @param file_name
      *     The name of the file to be written.
      */
    srec_output_file(const std::string &file_name);

    /**
      * The destructor.
      */
    virtual ~srec_output_file();

    // See base class for documentation.
    virtual const std::string filename() const;

    /**
      * Call this method if you want the output to contain only data
      * records.  No header records or start address records (or any
      * other records) will be emitted.
      */
    static void data_only();

    /**
      * The crlf method is used to force CRLF line termination, event if
      * the current operating system's native text files use something
      * else.
      */
    static void crlf();

protected:
    /**
      * The put_char method is used to send a character to the output.
      * Usually, this is sufficient, however derived classes may
      * over-ride it if they have a special case.  Over-ride with
      * caution, as it affects many other methods.
      */
    virtual void put_char(int);

    /**
      * The put_nibble method is used to send a hexadecimal digit (0..9,
      * A..F) to the output.  It calls put_char to send the output.
      */
    void put_nibble(int);

    /**
      * The put_byte method is used to send a byte value to the output.
      * The default implementation is to call the put_nibble method
      * twice, big-endian (most significant nibble first).
      *
      * The value of the byte will be added to the running checksum, via
      * the checksum_add() method.
      *
      * Usually, this get_byte() method implemention is sufficient for
      * most output classes, however derived classes may over-ride it if
      * they have a special case.  Over-ride with caution, as it affects
      * many other methods.
      */
    virtual void put_byte(unsigned char);

    /**
      * The put_word method is used to send a 16-bit value to the
      * output.  The put_byte() method is called twice, and the two byte
      * values are sent big-endian (most significant byte first).
      */
    virtual void put_word(int);

    /**
      * The put_3bytes method is used to send a 24-bit value to the
      * output.  The put_byte() method is called three times, and the
      * three byte values are sent big-endian (most significant byte
      * first).
      */
    virtual void put_3bytes(unsigned long);

    /**
      * The put_4bytes method is used to send a 32-bit value to the
      * output.  The put_byte() method is called four times, and the
      * four byte values are sent big-endian (most significant byte
      * first).
      */
    virtual void put_4bytes(unsigned long);

    /**
      * The checksum_reset method is used to set the running checksum to
      * zero.
      */
    void checksum_reset();

    /**
      * The checksum_add method is used to add another 8-bit value to
      * the running checksum.
      *
      * The default implementation uses a simple addition.  Derived
      * classesmay override if they need to.  Do this with caution, as
      * it affects other methods.
      */
    virtual void checksum_add(unsigned char n);

    /**
      * The checksum_get method is used to get the current value of the
      * running checksum (added to by the checksum_add() method, usually
      * called by the get_byte() method).  Only the lower 8 bits of the
      * sum are returned.
      */
    int checksum_get();

    /**
      * The checksum_get16 method is used to get the current value of
      * the running checksum (added to by the checksum_add() method,
      * usually called by the get_byte() method).  Only the lower 16
      * bits of the sum are returned.
      */
    int checksum_get16();

    /**
      * The seek_to method is used to move the output position to the
      * specified location in the output file.
      */
    void seek_to(unsigned long);

    /**
      * The put_string method is used to send a nul-terminated C string
      * to the output.  Multiple calls to put_char() are made.
      */
    void put_string(const char *);

    /**
      * The put_string method is used to send C++ string
      * to the output.  Multiple calls to put_char() are made.
      *
      * @param s
      *     The string to pint.
      */
    void put_string(const std::string &s);

    /**
      * The put_stringf method is used to send a formatted string to the
      * output.  The format and operation ios similar to the standard
      * printf function.  Multiple calls to put_char() are made.
      */
    void put_stringf(const char *, ...)             FORMAT_PRINTF(2, 3);

    /**
      * The mode method returns a suitable mode for passing to fopen.
      * The default implementation returns "w+" but derived classes may
      * over-ride it (e.g. "wb" for binary).
      */
    virtual const char *mode() const;

    /**
      * The data_only_flag class variable is set by the data_only()
      * method, to remember that only data records are to be sent to the
      * output.  Header records, start address records, etc, are all
      * suppressed.
      */
    static bool data_only_flag;

    /**
      * The crlf_flag class variable is used to remember whether or not
      * to force CRLF line termination, event if the current operating
      * system's native text files use something else.
      */
    static bool crlf_flag;

private:
    /**
      * The file_name instance variable is used by the filename() and
      * filename_and_line() methods to report the name of the input
      * file.  This makes for informative error mesages.
      */
    std::string file_name;

    /**
      * The line_number instance variable is used by the get_char
      * method to remember the current line number.  It us used by the
      * filename_and_line() method to report the current file location.
      */
    int line_number;

    /**
      * The vfp instance variable is used by the get_fp() method to
      * remember the file pointer.  You need to cast it to FILE* befor
      * you use it.  Never access this instance variable directly,
      * always go via the get_fp() method.  This ensures the file has
      * been opened first!
      */
    void *vfp;

protected:
    /**
      * The checksum instance variable is used record the running
      * checksum.  NEVER access this variable directly.  Always use the
      * checksum_reset() method to set it mack to its initial state.
      * Always use the checksum_add() method to add a byte to it.
      * Always use the checksum_get() or checksum_get16() methods to
      * read its value.
      */
    int checksum;

    /**
      * The fatal_alignment_error method is used to report problems
      * with unaligned data in formats that require aligned data.  It
      * suggests a fill to fix the problem.
      *
      * @param alignment
      *     The necessary byte alignment
      */
    void fatal_alignment_error(int alignment);

private:
    /**
      * The position instance variable is used to remember the
      * current position within the output file.  Set by the put_char
      * method, and the seek_to method.  Used by the seek_to method.
      */
    unsigned long position;

    /**
      * The is_regular instance variable is used to remember whther
      * or not the file is a regular file.  This is set by the
      * set_is_regular method.  It is used by the seek_to method.
      */
    bool is_regular;

    /**
      * The set_is_regular method shall be used whenevr vfp is assigned,
      * to estanblish whther the output file is a regular file or a
      * special file (like a pipe).
      */
    void set_is_regular(void);

    /**
      * The get_fp method is used to get the stdio file pointer
      * associated with this input file.  (By avoiding a FILE*
      * declaraion, we avoid having to include <stdio.h> for not
      * particularly good reason.  Take care when casting.)
      *
      * If the file has not been opened yet, it will be opened by this
      * method.
      */
    void *get_fp();

    /**
      * The copy constructor.  Do not use.
      */
    srec_output_file(const srec_output_file &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_output_file &operator=(const srec_output_file &);
};

#endif // INCLUDE_SREC_OUTPUT_FILE_H
