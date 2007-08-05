//
//      srecord - manipulate eprom load files
//      Copyright (C) 1998-2007 Peter Miller
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


#include <cstdio>
#include <errno.h>
#include <iostream>
using namespace std;
#include <cstring>

#include <lib/srec/input/file.h>

bool srec_input_file::ignore_checksums_default = false;


srec_input_file::srec_input_file() :
    file_name("standard input"),
    line_number(1),
    prev_was_newline(false),
    vfp(stdin),
    checksum(0),
    is_text(0),
    ignore_checksums(ignore_checksums_default)
{
}


const char *
srec_input_file::mode()
    const
{
    return "r";
}


srec_input_file::srec_input_file(const string &a_file_name) :
    file_name(a_file_name),
    line_number(1),
    prev_was_newline(false),
    vfp(0),
    checksum(0),
    is_text(0),
    ignore_checksums(ignore_checksums_default)
{
    if (file_name == string("-"))
    {
        file_name = "standard input";
        vfp = stdin;
    }
    else
    {
        //
        // The call to fopen is deferred until the constructor has
        // completed.  This is so that the virtual mode() method
        // is available (it isn't in the base class constructor).
        //
    }
}


void *
srec_input_file::get_fp()
{
    if (!vfp)
    {
        //
        // The call to fopen is deferred until the constructor has
        // completed.  This is so that the virtual mode() method
        // is available (it isn't in the base class constructor).
        //
        const char *the_mode = mode();
        vfp = fopen(file_name.c_str(), the_mode);
        if (!vfp)
                fatal_error_errno("open");
        is_text = !strchr(the_mode, 'b');
        if (!is_text)
            line_number = 0;
    }
    return vfp;
}


srec_input_file::~srec_input_file()
{
    FILE *fp = (FILE *)get_fp();
    if (fp != stdin && fclose(fp))
        fatal_error_errno("close");
}


string
srec_input_file::filename()
    const
{
    return file_name;
}


string
srec_input_file::filename_and_line()
    const
{
    if (!vfp)
        return file_name;
    char buffer[20];
    if (is_text)
        sprintf(buffer, ": %d", line_number);
    else
        sprintf(buffer, ": 0x%04X", line_number);
    return (file_name + buffer);
}


int
srec_input_file::get_char()
{
    FILE *fp = (FILE *)get_fp();
    if (prev_was_newline)
        ++line_number;
    int c = getc(fp);
    if (c == EOF)
    {
        if (ferror(fp))
            fatal_error_errno("read");

        //
        // If this is a text file, but the last character wasn't
        // a newline, insert one.
        //
        c = ((is_text && !prev_was_newline) ? '\n' : -1);
    }
    else if (c == '\r' && is_text)
    {
        //
        // If this is a text file, turn CRLF into LF.
        // Leave all other sequences containing CR alone.
        //
        c = getc(fp);
        if (c == EOF)
        {
            if (ferror(fp))
                fatal_error_errno("read");
            c = '\r';
        }
        else if (c != '\n')
        {
            ungetc(c, fp);
            c = '\r';
        }
    }
    if (!is_text && c >= 0)
        ++line_number;
    prev_was_newline = (is_text && c == '\n');
    return c;
}


void
srec_input_file::get_char_undo(int c)
{
    if (c >= 0)
    {
        FILE *fp = (FILE *)get_fp();
        prev_was_newline = false;
        if (!is_text)
            --line_number;
        ungetc(c, fp);
    }
}


int
srec_input_file::peek_char()
{
    FILE *fp = (FILE *)get_fp();
    int c = getc(fp);
    if (c == EOF)
    {
        if (ferror(fp))
            fatal_error_errno("read");
        c = -1;
    }
    else
        ungetc(c, fp);
    return c;
}


int
srec_input_file::get_nibble_value(int c)
{
    switch (c)
    {
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
        return (c - '0');

    case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
        return (c - 'a' + 10);

    case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
        return (c - 'A' + 10);
    }
    return -1;
}


int
srec_input_file::get_nibble()
{
    int c = get_char();
    int n = get_nibble_value(c);
    if (n < 0)
        fatal_error("hexadecimal digit expected");
    return n;
}


int
srec_input_file::get_byte()
{
    int c1 = get_nibble();
    int c2 = get_nibble();
    int n = ((c1 << 4) | c2);
    checksum_add(n);
    return n;
}


int
srec_input_file::get_word()
{
    int b1 = get_byte();
    int b2 = get_byte();
    return ((b1 << 8) | b2);
}


unsigned long
srec_input_file::get_3bytes()
{
    unsigned long b1 = get_byte();
    unsigned long b2 = get_byte();
    unsigned long b3 = get_byte();
    return ((((b1 << 8) | b2) << 8) | b3);
}


unsigned long
srec_input_file::get_4bytes()
{
    unsigned long b1 = get_byte();
    unsigned long b2 = get_byte();
    unsigned long b3 = get_byte();
    unsigned long b4 = get_byte();
    return ((((((b1 << 8) | b2) << 8) | b3) << 8) | b4);
}


int
srec_input_file::checksum_get()
    const
{
    return (checksum & 0xFF);
}


int
srec_input_file::checksum_get16()
    const
{
    return (checksum & 0xFFFF);
}


void
srec_input_file::checksum_reset()
{
    checksum = 0;
}


void
srec_input_file::checksum_add(unsigned char n)
{
    checksum += n;
}


void
srec_input_file::seek_to_end()
{
    FILE *fp = (FILE *)get_fp();
    fseek(fp, 0L, SEEK_END);
}


void
srec_input_file::disable_checksum_validation()
{
    ignore_checksums = true;
}
