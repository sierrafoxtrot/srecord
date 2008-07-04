//
//      srecord - manipulate eprom load files
//      Copyright (C) 1998-2002, 2005-2008 Peter Miller
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

#include <cerrno>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>

#include <lib/srec/output/file.h>


bool srec_output_file::data_only_flag = false;
bool srec_output_file::crlf_flag = false;


srec_output_file::srec_output_file() :
    file_name("standard output"),
    line_number(1),
    vfp(0),
    checksum(0),
    position(0),
    is_regular(true)
{
    vfp = stdout;
    set_is_regular();
}


const char *
srec_output_file::mode()
    const
{
    return (crlf_flag ? "wb" : "w");
}


srec_output_file::srec_output_file(const std::string &a_file_name) :
    file_name(a_file_name),
    line_number(1),
    vfp(0),
    checksum(0),
    position(0),
    is_regular(true)
{
    if (file_name == "-")
    {
        file_name = "standard output";
        vfp = stdout;
        set_is_regular();
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
srec_output_file::get_fp()
{
    if (!vfp)
    {
        //
        // The call to fopen is deferred until the constructor has
        // completed.  This is so that the virtual mode() method
        // is available (it isn't in the base class constructor).
        //
        vfp = fopen(file_name.c_str(), mode());
        if (!vfp)
            fatal_error_errno("open");
        set_is_regular();
    }
    return vfp;
}


srec_output_file::~srec_output_file()
{
    FILE *fp = (FILE *)get_fp();
    if (fflush(fp))
        fatal_error_errno("write");
    if (fp != stdout && fclose(fp))
        fatal_error_errno("close");
}


const std::string
srec_output_file::filename()
    const
{
    char buffer[20];
    sprintf(buffer, ": %d", line_number);
    return (file_name + buffer);
}


void
srec_output_file::put_char(int c)
{
    FILE *fp = (FILE *)get_fp();
    if (crlf_flag && c == '\n')
    {
        putc('\r', fp);
        ++position;
    }
    putc(c, fp);
    if (ferror(fp))
        fatal_error_errno("write");
    if (c == '\n')
        ++line_number;
    ++position;
}


void
srec_output_file::put_nibble(int n)
{
    put_char("0123456789ABCDEF"[n & 15]);
}


void
srec_output_file::put_byte(unsigned char n)
{
    put_nibble(n >> 4);
    put_nibble(n);
    checksum_add(n);
}


void
srec_output_file::put_word(int n)
{
    put_byte(n >> 8);
    put_byte(n);
}


void
srec_output_file::put_3bytes(unsigned long n)
{
    put_byte(n >> 16);
    put_byte(n >> 8);
    put_byte(n);
}


void
srec_output_file::put_4bytes(unsigned long n)
{
    put_byte(n >> 24);
    put_byte(n >> 16);
    put_byte(n >> 8);
    put_byte(n);
}


int
srec_output_file::checksum_get()
{
    return (checksum & 0xFF);
}


int
srec_output_file::checksum_get16()
{
    return (checksum & 0xFFFF);
}


void
srec_output_file::checksum_reset()
{
    checksum = 0;
}

void
srec_output_file::checksum_add(unsigned char n)
{
    checksum += n;
}


void
srec_output_file::seek_to(unsigned long address)
{
    //
    // Seeking on non-regular files is problematic.  Avoid doing
    // this if possible.  (Usually we can, srec_cat emits records
    // in ascending address order.)
    //
    if (!is_regular)
    {
        while (position < address)
            put_char(0);
    }
    if (address == position)
        return;

    //
    // We'll have to try a seek.
    //
    FILE *fp = (FILE *)get_fp();
    errno = 0;
    if (fseek(fp, address, 0) < 0)
    {
        if (errno == EINVAL && address >= 0x80000000uL)
        {
            warning
            (
                "It appears that the implementation of fseek on your "
                "system is unable to cope with addresses which have "
                "the most significant bit set (this is POSIX and ANSI "
                "C conforming behaviour).  You probably did not intend "
                "to create a %3.1fGB file.  See the manual for a "
                "description of the --offset filter, remembering that "
                "you can give negative offsets.",
                ((double)address / (double)(1uL << 30))
            );
        }
        fatal_error_errno("seek 0x%lX", address);
    }
    position = address;
}


void
srec_output_file::put_string(const char *s)
{
    while (*s)
        put_char(*s++);
}


void
srec_output_file::put_string(const std::string &s)
{
    const char *cp = s.c_str();
    const char *ep = cp + s.size();
    while (cp < ep)
        put_char(*cp++);
}


void
srec_output_file::put_stringf(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    char buffer[1000];
    vsprintf(buffer, fmt, ap);
    va_end(ap);
    put_string(buffer);
}


void
srec_output_file::data_only()
{
    data_only_flag = true;
}


void
srec_output_file::crlf()
{
    crlf_flag = true;
}


void
srec_output_file::set_is_regular()
{
    FILE *fp = (FILE *)vfp;
    struct stat st;
    is_regular = fstat(fileno(fp), &st) == 0 && S_ISREG(st.st_mode);
}


void
srec_output_file::fatal_alignment_error(int multiple)
{
    if (multiple > 4)
    {
        fatal_error
        (
            "The %s output format uses %d-byte alignment, but unaligned "
                "data is present.  Use a \"--fill 0xNN --within <input> "
                "--range-padding %d\" filter to fix this problem.",
            format_name(),
            multiple,
            multiple
        );
    }
    else
    {
        fatal_error
        (
            "The %s output format uses %d-bit data, but unaligned "
                "data is present.  Use a \"--fill 0xNN --within "
                "<input> --range-padding %d\" filter to fix this "
                "problem.",
            format_name(),
            multiple * 8,
            multiple
        );
    }
}
