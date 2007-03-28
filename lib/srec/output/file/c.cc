//
//      srecord - manipulate eprom load files
//      Copyright (C) 1998-2003, 2006, 2007 Peter Miller
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
//      Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111, USA.
//
// MANIFEST: functions to impliment the srec_output_file_c class
//


#include <cstdio>
#include <cstring>

#include <lib/interval.h>
#include <lib/srec/arglex.h>
#include <lib/srec/output/file/c.h>
#include <lib/srec/record.h>


srec_output_file_c::srec_output_file_c() :
    srec_output_file("-"),
    prefix("eprom"),
    taddr(0),
    header_done(false),
    column(0),
    current_address(0),
    line_length(75),
    address_length(4),
    constant(true),
    include(false),
    include_file_name("eprom.h")
{
}


static const char *
memrchr(const char *data, char c, size_t len)
{
    if (!data)
        return 0;
    const char *result = 0;
    while (len > 0)
    {
        const char *p = (const char *)memchr(data, c, len);
        if (!p)
            break;
        result = p;
        size_t chunk = p - data + 1;
        data += chunk;
        len -= chunk;
    }
    return result;
}


static string
build_include_file_name(const string &filename)
{
    const char *fn = filename.c_str();
    // Watch out for out base class adding a line number.
    const char *colon = strstr(fn, ": ");
    if (!colon)
        colon = fn + strlen(fn);
    const char *slash = memrchr(fn, '/', colon - fn);
    if (!slash)
        slash = memrchr(fn, '\\', colon - fn);
    if (slash)
        slash++;
    else
        slash = fn;
    const char *ep = memrchr(slash, '.', colon - slash);
    if (!ep)
        ep = colon;
    return (string(fn, ep - fn) + ".h");
}


srec_output_file_c::srec_output_file_c(const string &a_file_name) :
    srec_output_file(a_file_name),
    prefix("eprom"),
    taddr(0),
    header_done(false),
    column(0),
    current_address(0),
    line_length(75),
    address_length(4),
    constant(true),
    include(false),
    include_file_name(build_include_file_name(a_file_name))
{
}


void
srec_output_file_c::command_line(srec_arglex *cmdln)
{
    if (cmdln->token_cur() == arglex::token_string)
    {
        prefix = cmdln->value_string();
        cmdln->token_next();
    }
    for (;;)
    {
        switch (cmdln->token_cur())
        {
        case srec_arglex::token_constant:
            cmdln->token_next();
            constant = true;
            break;

        case srec_arglex::token_constant_not:
            cmdln->token_next();
            constant = false;
            break;

        case srec_arglex::token_include:
            cmdln->token_next();
            include = true;
            break;

        case srec_arglex::token_include_not:
            cmdln->token_next();
            include = false;
            break;

        default:
            return;
        }
    }
}


void
srec_output_file_c::emit_header()
{
    if (header_done)
        return;
    if (constant)
        put_stringf("const ");
    put_stringf("unsigned char %s[] =\n{\n", prefix.c_str());
    header_done = true;
}


void
srec_output_file_c::emit_byte(int n)
{
    if (column >= line_length)
    {
        put_char('\n');
        column = 0;
    }
    else if (column)
    {
        put_char(' ');
        ++column;
    }
    put_stringf("0x%02X,", (unsigned char)n);
    column += 5;
}


static string
toupper(const string &s)
{
    char *buffer = new char [s.size() + 1];
    char *bp = buffer;
    const char *cp = s.c_str();
    while (*cp)
    {
        unsigned char c = *cp++;
        if (islower(c))
            *bp++ = toupper(c);
        else
            *bp++ = c;
    }
    string result(buffer, bp - buffer);
    delete buffer;
    return result;
}


static string
identifier(const string &s)
{
    char *buffer = new char [s.size() + 1];
    char *bp = buffer;
    const char *cp = s.c_str();
    while (*cp)
    {
        unsigned char c = *cp++;
        if (islower(c))
            *bp++ = toupper(c);
        else if (isalnum(c))
            *bp++ = c;
        else
            *bp++ = '_';
    }
    string result(buffer, bp - buffer);
    delete buffer;
    return result;
}


srec_output_file_c::~srec_output_file_c()
{
    emit_header();
    if (range.empty())
        emit_byte(0xFF);
    if (column)
        put_char('\n');
    put_string("};\n");

    int width = 2 * address_length;
    if (!data_only_flag)
    {
        if (constant)
            put_stringf("const ");
        put_stringf("unsigned long %s_termination = 0x%0*lX;\n",
            prefix.c_str(), width, taddr);
        if (constant)
            put_stringf("const ");
        put_stringf("unsigned long %s_start       = 0x%0*lX;\n",
            prefix.c_str(), width, range.get_lowest());
        if (constant)
            put_stringf("const ");
        put_stringf("unsigned long %s_finish      = 0x%0*lX;\n",
            prefix.c_str(), width, range.get_highest());
    }

    if (constant)
        put_stringf("const ");
    put_stringf("unsigned long %s_length      = 0x%0*lX;\n",
        prefix.c_str(), width, range.get_highest() - range.get_lowest());
    put_stringf("\n");

    //
    // Some folks prefer #define instead
    //
    string PREFIX = toupper(prefix);
    put_stringf("#define %s_TERMINATION 0x%0*lX\n",
        PREFIX.c_str(), width, taddr);
    put_stringf("#define %s_START       0x%0*lX\n",
        PREFIX.c_str(), width, range.get_lowest());
    put_stringf("#define %s_FINISH      0x%0*lX\n",
        PREFIX.c_str(), width, range.get_highest());
    put_stringf("#define %s_LENGTH      0x%0*lX\n",
        PREFIX.c_str(), width, range.get_highest() - range.get_lowest());

    if (include)
    {
        string insulation = identifier(include_file_name);
        FILE *fp = fopen(include_file_name.c_str(), "w");
        if (!fp)
            fatal_error_errno("open %s", include_file_name.c_str());
        fprintf(fp, "#ifndef %s\n", insulation.c_str());
        fprintf(fp, "#define %s\n", insulation.c_str());
        fprintf(fp, "\n");
        if (!data_only_flag)
        {
            if (constant)
                fprintf(fp, "const ");
            fprintf(fp, "extern unsigned long %s_termination;\n",
                prefix.c_str());
            if (constant)
                fprintf(fp, "const ");
            fprintf(fp, "extern unsigned long %s_start;\n", prefix.c_str());
            if (constant)
                fprintf(fp, "const ");
            fprintf(fp, "extern unsigned long %s_finish;\n", prefix.c_str());
        }
        if (constant)
            fprintf(fp, "const ");
        fprintf(fp, "extern unsigned long %s_length;\n", prefix.c_str());
        if (constant)
            fprintf(fp, "const ");
        fprintf(fp, "extern unsigned char %s[];\n", prefix.c_str());
        fprintf(fp, "\n");
        fprintf(fp, "#endif /* %s */\n", insulation.c_str());

        if (fclose(fp))
            fatal_error_errno("write %s", include_file_name.c_str());
    }
}


void
srec_output_file_c::write(const srec_record &record)
{
    switch (record.get_type())
    {
    default:
        // ignore
        break;

    case srec_record::type_header:
        // emit header records as comments in the file
        {
            put_string("/* ");
            if (record.get_address() != 0)
                put_stringf("%08lX: ", record.get_address());
            const unsigned char *cp = record.get_data();
            const unsigned char *ep = cp + record.get_length();
            while (cp < ep)
            {
                int c = *cp++;
                if (isprint(c) || isspace(c))
                    put_char(c);
                else
                    put_stringf("\\%o", c);
                // make sure we don't end the comment
                if (c == '*' && cp < ep && *cp == '/')
                    put_char(' ');
            }
            put_string(" */\n");
        }
        break;

    case srec_record::type_data:
        if (range.empty())
            current_address = record.get_address();
        range +=
            interval
            (
                record.get_address(),
                record.get_address() + record.get_length()
            );
        emit_header();
        while (current_address < record.get_address())
        {
            emit_byte(0xFF);
            ++current_address;
        }
        for (int j = 0; j < record.get_length(); ++j)
        {
            if (record.get_address() + j < current_address)
                continue;
            emit_byte(record.get_data(j));
            ++current_address;
        }
        break;

    case srec_record::type_start_address:
        taddr = record.get_address();
        break;
    }
}


void
srec_output_file_c::line_length_set(int n)
{
    n = (n + 1) / 6;
    if (n < 1)
        n = 1;
    n = n * 6 - 1;
    line_length = n;
}


void
srec_output_file_c::address_length_set(int n)
{
    switch (n)
    {
    default:
        address_length = 4;
        break;

    case 1:
    case 2:
    case 3:
    case 4:
        address_length = n;
        break;
    }
}


int
srec_output_file_c::preferred_block_size_get()
    const
{
    //
    // Irrelevant.  Use the largest we can get.
    //
    return srec_record::max_data_length;
}
