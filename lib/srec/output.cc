//
//      srecord - manipulate eprom load files
//      Copyright (C) 1998, 1999, 2001-2003, 2006-2008 Peter Miller
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
#include <cstring>

#include <lib/quit.h>
#include <lib/srec/output.h>
#include <lib/srec/record.h>


srec_output::srec_output()
{
}


srec_output::~srec_output()
{
}


void
srec_output::fatal_error(const char *fmt, ...)
    const
{
    va_list ap;
    va_start(ap, fmt);
    fatal_error_v(fmt, ap);
    va_end(ap);
}


void
srec_output::fatal_error_v(const char *fmt, va_list ap)
    const
{
    char buf[1024];
    vsnprintf(buf, sizeof(buf), fmt, ap);
    quit_default.fatal_error("%s: %s", filename().c_str(), buf);
}


void
srec_output::fatal_error_errno(const char *fmt, ...)
    const
{
    va_list ap;
    va_start(ap, fmt);
    fatal_error_errno_v(fmt, ap);
    va_end(ap);
}


void
srec_output::fatal_error_errno_v(const char *fmt, va_list ap)
    const
{
    int n = errno;
    char buf[1024];
    vsnprintf(buf, sizeof(buf), fmt, ap);
    quit_default.fatal_error_errno
    (
        "%s: %s: %s [%d]",
        filename().c_str(),
        buf,
        strerror(n),
        n
    );
}


void
srec_output::warning(const char *fmt, ...)
    const
{
    va_list ap;
    va_start(ap, fmt);
    warning_v(fmt, ap);
    va_end(ap);
}


void
srec_output::warning_v(const char *fmt, va_list ap)
    const
{
    char buf[1024];
    vsnprintf(buf, sizeof(buf), fmt, ap);
    quit_default.warning("%s: %s", filename().c_str(), buf);
}


void
srec_output::write_header(const srec_record *rp)
{
    if (rp)
    {
        // Make sure we are writing a header record
        srec_record record(*rp);
        record.set_type(srec_record::type_header);
        write(record);
    }
    else
    {
        //
        // This is the default header record.
        // If you want to change it, this is the place.
        //
        static char hdr[] = "http://srecord.sourceforge.net/";
        srec_record record
        (
            srec_record::type_header,
            (srec_record::address_t)0,
            (const srec_record::data_t *)hdr,
            strlen(hdr)
        );
        write(record);
    }
}


void
srec_output::write_data(unsigned long address, const void *data, size_t length)
{
    const srec_record::data_t *data_p = (const srec_record::data_t *)data;
    size_t block_size = preferred_block_size_get();
    while (length > 0)
    {
        int nbytes = (length > block_size ? block_size : length);
        srec_record record(srec_record::type_data, address, data_p, nbytes);
        write(record);
        address += nbytes;
        data_p += nbytes;
        length -= nbytes;
    }
}


void
srec_output::write_execution_start_address(const srec_record *rp)
{
    if (rp)
    {
        // Make sure we are writing an execution start address record
        srec_record record(*rp);
        record.set_type(srec_record::type_execution_start_address);
        write(record);
    }
    else
    {
        //
        // This is the default execution start address record.
        // If you want to change it, this is the place.
        //
        srec_record record(srec_record::type_execution_start_address, 0, 0, 0);
        write(record);
    }
}


void
srec_output::notify_upper_bound(unsigned long)
{
}


void
srec_output::command_line(srec_arglex *)
{
    // Do nothing.
}
