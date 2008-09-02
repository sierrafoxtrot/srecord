//
//      srecord - manipulate eprom load files
//      Copyright (C) 1998-2008 Peter Miller
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

#ifndef LIB_SREC_INPUT_FILE_H
#define LIB_SREC_INPUT_FILE_H

#include <string>

#include <lib/srec/input.h>

/**
  * The srec_input_file class is used to represent an generic input
  * file. It provides many helper methods common to most file input
  * classes.
  */
class srec_input_file:
    public srec_input
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_input_file();

    /**
      * The guess class method is used to open a file of an unknown
      * type.  It attempts all of the know formats one after the other.
      *
      * @param file_name
      *     The name of the file to be opened.
      */
    static pointer guess(const std::string &file_name);

    /**
      * The ignore_all_checksums method is used to set the global
      * ignore checksums flag.  This is usually the result of an
      * --ignore-checksums command line option.
      */
    static void ignore_all_checksums() { ignore_checksums_default = true; }

protected:
    // See base class for documentation.
    virtual std::string filename() const;

    // See base class for documentation.
    virtual std::string filename_and_line() const;

    // See base class for documentation.
    void disable_checksum_validation();

protected:
    /**
      * The constructor.  The input will be taken from the named file
      * (or the standard input if the filename is "-").
      *
      * Only derived classes may use this constructor.
      *
      * @param file_name
      *     The name of the file to be read.
      */
    srec_input_file(const std::string &file_name);

    /**
      * The get_char method is used to fetch a character from
      * the input.  Usually, this is sufficient, however derived
      * classes may over-ride it if they have a special case.
      * Over-ride with caution, as it affects many other methods.
      *
      * The line_number instance variable is maintained, so that the
      * filename_and_line method may report the current file location.
      * This makes for more informative error messages.
      */
    virtual int get_char();

    /**
      * The get_char_undo method is used to return a character to
      * the input.  (a later get_char or peak_char method will see
      * it again).  Only one character may be pushed back.
      */
    virtual void get_char_undo(int);

    /**
      * The peek_char method is used to look at the next character
      * of input, without actually consuming it (a later get_char
      * or peak_char method will still see it).
      */
    int peek_char();

    /**
      * The get_nibble_value class method is used to translate a
      * character into its hexadecimal value.
      *
      * @param c
      *     The character to translate
      * @returns
      *     int; 0..9 for '0'..'9', 10..15 for 'a'..'f',
      *     10..15 for 'A-Z', and -1 for everything else.
      */
    static int get_nibble_value(int c);

    /**
      * The get_nibble method is used to fetch one hexadecimal digit
      * from the input, via the get_char method.  It is not case
      * sensitive.  It returns a value for 0 to 15.  Characters which
      * are not hexadecimal digits will result in a fatal error,
      * and the method call will not return.
      */
    int get_nibble();

    /**
      * The get_byte method is used to fetch a byte value from the
      * input.  The default implementation is to call the get_nibble
      * method twice, and assemble them big-endian (most significant
      * nibble first).
      *
      * The value of the byte will be added to the running checksum,
      * via the checksum_add method.
      *
      * Usually, this get_byte method implementation is sufficient for
      * most input classes, however derived classes may over-ride
      * it if they have a special case.  Over-ride with caution,
      * as it affects many other methods.
      */
    virtual int get_byte();

    /**
      * The get_word method is used to fetch a 16-bit value from the
      * input.  The get_byte method is called twice, and the two byte
      * values are assembles big-endian (most significant byte first).
      */
    int get_word();

    /**
      * The get_3bytes method is used to fetch a 24-bit value from
      * the input.  The get_byte method is called three times,
      * and the three byte values are assembles big-endian (most
      * significant byte first).
      */
    unsigned long get_3bytes();

    /**
      * The get_4bytes method is used to fetch a 32-bit value from
      * the input.  The get_byte method is called four times,
      * and the three byte values are assembles big-endian (most
      * significant byte first).
      */
    unsigned long get_4bytes();

    /**
      * The checksum_get method is used to get the current value of
      * the running checksum (added to by the checksum_add method,
      * usually called by the get_byte method).  Only the lower 8
      * bits of the sum are returned.
      */
    int checksum_get() const;

    /**
      * The checksum_get16 method is used to get the current value of
      * the running checksum (added to by the checksum_add method,
      * usually called by the get_byte method).  Only the lower 16
      * bits of the sum are returned.
      */
    int checksum_get16() const;

    /**
      * The checksum_add method is used to add another 8-bit value
      * to the running checksum.
      */
    virtual void checksum_add(unsigned char n);

    /**
      * The checksum_rest method is used to set the running checksum
      * to zero.
      */
    void checksum_reset();

    /**
      * The seek_to_end method is used to move the input position
      * to the end of the file.
      */
    void seek_to_end();

    /**
      * The mode method is used to return a suitable mode string for
      * an fopen call.  The default implementation is "r", assuming
      * a text file.  Other file formats may need to override this
      * if they have other needs (e.g. "rb" for binary).
      */
    virtual const char *mode() const;

private:
    /**
      * The file_name instance variable is used by the filename
      * and filename_and_line methods to report the name of the
      * input file.  This makes for informative error messages.
      */
    std::string file_name;

    /**
      * The line_number instance variable is used by the get_char
      * method to remember the current line number.  It us used by the
      * filename_and_line method to report the current file location.
      */
    int line_number;

    /**
      * The prev_was_newline instance variable is used by the
      * get_char method to know when to increment the line number.
      * It is not done when a newline is seen, but rather on reading
      * the first character after a newline.  In this way, the error
      * messages refer to the correct line, when if (when) it was
      * the the error message must be issued only when the whole
      * line has bean read in, including the newline.  error message.
      */
    bool prev_was_newline;

    /**
      * The vfp instance variable is used by the get_fp method to
      * remember the file pointer.  You need to cast it to FILE* before
      * you use it.  Never access this instance variable directly,
      * always go via the get_fp method.  This ensures the file has
      * been opened first!
      */
    void *vfp;

protected:
    /**
      * The checksum instance variable is used record the running
      * checksum.  NEVER access this variable directly.  Always use
      * the checksum_reset() method to set it mack to its initial state.
      * Always use the checksum_add method to add a byte to it.
      * Always use the checksum_get() or checksum_get16() methods to
      * read its value.
      */
    int checksum;

    /**
      * The use_checksums method is used to determine whether or not to
      * validate checksums when data is read.
      *
      * @returns
      *     bool; true if need to check checksums, false to ignore checksums.
      */
    bool use_checksums() const { return !ignore_checksums; }

private:
    /**
      * The is_text instance variable is used remember if a file mode
      * is text (true) or binary (false).  This information is  used
      * by the get_char method to decide to increment the line number
      * or not, and by the filename_and_line method to decide whether
      * or not to include the line number in the location reported.
      *
      * This instance variable is set by the get_fp method when it
      * opens the file.
      */
    int is_text;

    /**
      * The ignore_checksums instance variable is used to remember
      * whether or not checksums should be ignored (true) or validated
      * (false).
      */
    bool ignore_checksums;

    /**
      * The ignore_checksums_default class variable is used to remember
      * the default checksum validation.  Defaults to false.
      */
    static bool ignore_checksums_default;

    /**
      * The get_fp method is used to get the stdio file pointer
      * associated with this input file.  (By avoiding a FILE*
      * declaration, we avoid having to include <stdio.h> for no
      * particularly good reason.  Take care when casting.)
      *
      * If the file has not been opened yet, it will be opened by
      * this method.
      */
    void *get_fp();

    /**
      * The default constructor.  Do not use.
      */
    srec_input_file();

    /**
      * The copy constructor.  Do not use.
      */
    srec_input_file(const srec_input_file &);

    /**
      * the assignment operator.  Do not use.
      */
    srec_input_file &operator=(const srec_input_file &);
};

#endif // LIB_SREC_INPUT_FILE_H
