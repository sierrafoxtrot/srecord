//
//      srecord - manipulate eprom load files
//      Copyright (C) 1998-2002, 2005-2010 Peter Miller
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
#include <cstdio>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>

#include <srecord/arglex.h>
#include <srecord/sizeof.h>
#include <srecord/output/file.h>
#include <srecord/record.h>


bool srec_output_file::enable_header_flag = true;
bool srec_output_file::enable_data_count_flag = true;
bool srec_output_file::enable_goto_addr_flag = true;
bool srec_output_file::enable_footer_flag = true;


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
    line_termination = line_termination_binary;
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
        line_termination = line_termination_binary;
    }
    else
    {
        //
        // The call to fopen is deferred until the constructor has
        // completed.  This is so that the line_termination can be set
        // correctly.
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
        // completed.  This is so that the line_termination can be set
        // correctly.
        //
#ifdef __CYGWIN__
        if (line_termination == line_termination_native)
        {
            vfp = fopen(file_name.c_str(), "w");
            if (!vfp)
                fatal_error_errno("open");
            line_termination = line_termination_binary;
        }
        else
#endif
        {
            vfp = fopen(file_name.c_str(), "wb");
            if (!vfp)
                fatal_error_errno("open");
        }
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
    if (c == '\n')
    {
        ++line_number;
        for (;;)
        {
            switch (line_termination)
            {
            case line_termination_native:
                line_termination = line_termination_guess();
                continue;

            case line_termination_primos:
                putc('\n', fp);
                ++position;
                if (position & 1)
                {
                    putc(0, fp);
                    ++position;
                }
                break;

            case line_termination_nl:
                putc('\n', fp);
                ++position;
                break;

            case line_termination_cr:
                putc('\r', fp);
                ++position;
                break;

            case line_termination_crlf:
                putc('\r', fp);
                ++position;
                putc('\n', fp);
                ++position;
                break;
            }
            break;
        }
    }
    else
    {
        putc(c, fp);
        ++position;
    }
    if (ferror(fp))
        fatal_error_errno("write");
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
srec_output_file::enable_header(bool yesno)
{
    enable_header_flag = yesno;
}


void
srec_output_file::enable_data_count(bool yesno)
{
    enable_data_count_flag = yesno;
}


void
srec_output_file::enable_goto_addr(bool yesno)
{
    enable_goto_addr_flag = yesno;
}


void
srec_output_file::enable_footer(bool yesno)
{
    enable_footer_flag = yesno;
}


bool
srec_output_file::enable_by_name(const std::string &name, bool yesno)
{
    struct table_t
    {
        const char *name;
        void (*func)(bool);
    };

    static const table_t table[] =
    {
        { "Header", &srec_output_file::enable_header },
        { "Data_Count", &srec_output_file::enable_data_count },
        { "Execution_Start_Address", &srec_output_file::enable_goto_addr },
        { "Footer", &srec_output_file::enable_footer },
    };

    for (const table_t *tp = table; tp < ENDOF(table); ++tp)
    {
        if (arglex::compare(tp->name, name.c_str()))
        {
            tp->func(yesno);
            return true;
        }
    }
    return false;
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


void
srec_output_file::data_address_too_large(const srec_record &record)
    const
{
    unsigned long lo = record.get_address();
    unsigned long hi = lo + record.get_length() - 1;
    fatal_error("data address (0x%lX..0x%lX) too large", lo, hi);
}
