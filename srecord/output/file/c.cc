//
// srecord - manipulate eprom load files
// Copyright (C) 1998-2003, 2006-2010 Peter Miller
// Copyright (C) 2014 Scott Finneran
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see
// <http://www.gnu.org/licenses/>.
//

#include <cinttypes>
#include <cstdio>
#include <cstring>

#include <srecord/interval.h>
#include <srecord/arglex/tool.h>
#include <srecord/output/file/c.h>
#include <srecord/record.h>


static std::string
toupper(const std::string &s)
{
    char *buffer = new char[s.size() + 1];
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
    std::string result(buffer, bp - buffer);
    delete [] buffer;
    return result;
}


static std::string
identifier(const std::string &s)
{
    char *buffer = new char[s.size() + 1];
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
    std::string result(buffer, bp - buffer);
    delete [] buffer;
    return result;
}


srecord::output_file_c::~output_file_c()
{
    //
    // Finish initializing the data array.
    //
    emit_header();
    if (range.empty())
    {
        emit_word(max_word_value());
    }
    if (column)
    {
        put_char('\n');
        column = 0;
    }
    put_string("};\n");

    int nsections = 0;
    if (section_style)
    {
        //
        // emit list of section addresses
        //
        put_string("\n");
        if (constant)
            put_string("const ");
        put_stringf("unsigned long %s_address[] =\n{\n", prefix.c_str());
        interval x = range;
        while (!x.empty())
        {
            interval x2 = x;
            x2.first_interval_only();
            x -= x2;
            unsigned long address = x2.get_lowest();

            std::string s = format_address(address);
            int len = s.size();

            if (column && column + len + 2 > line_length)
            {
                put_char('\n');
                column = 0;
            }
            if (column)
            {
                put_char(' ');
                ++column;
            }
            put_string(s);
            column += len;
            put_char(',');
            ++column;
        }
        if (column)
        {
            put_char('\n');
            column = 0;
        }
        put_stringf("};\n");

        //
        // emit list of section word addresses
        // (assuming memory addresses are word-wide).
        //
        if (output_type != output_byte)
        {
            if (constant)
                put_string("const ");
            put_stringf("unsigned long %s_word_address[] =\n{\n",
                        prefix.c_str());
            x = range;
            while (!x.empty())
            {
                interval x2 = x;
                x2.first_interval_only();
                x -= x2;
                unsigned long address = x2.get_lowest() / bytes_per_word();

                std::string s = format_address(address);
                int len = s.size();

                if (column && column + len + 2 > line_length)
                {
                    put_char('\n');
                    column = 0;
                }
                if (column)
                {
                    put_char(' ');
                    ++column;
                }
                put_string(s);
                column += len;
                put_char(',');
                ++column;
            }
            if (column)
            {
                put_char('\n');
                column = 0;
            }
            put_stringf("};\n");
        }

        //
        // emit list of section lengths
        //
        if (constant)
            put_string("const ");
        put_stringf
        (
            "unsigned long %s_length_of_sections[] =\n{\n",
            prefix.c_str()
        );
        x = range;
        while (!x.empty())
        {
            interval x2 = x;
            x2.first_interval_only();
            x -= x2;
            unsigned long length = x2.get_highest() - x2.get_lowest();
            ++nsections;

            length /= bytes_per_word();
            std::string s = format_address(length);
            int len = s.size();

            if (column && column + len + 2 > line_length)
            {
                put_char('\n');
                column = 0;
            }
            if (column)
            {
                put_char(' ');
                ++column;
            }
            put_string(s);
            column += len;
            put_char(',');
            ++column;
        }
        if (column)
        {
            put_char('\n');
            column = 0;
        }
        put_string("};\n");

        //
        // emit the number of sections
        //
        if (constant)
            put_string("const ");
        put_string("unsigned long ");
        put_string(prefix.c_str());
        put_string("_sections    = ");
        put_string(format_address(nsections));
        put_string(";\n");
    }

    if (enable_goto_addr_flag)
    {
        if (constant)
            put_string("const ");
        put_stringf
        (
            "unsigned long %s_termination = %s;\n",
            prefix.c_str(),
            format_address(taddr).c_str()
        );
    }
    if (enable_footer_flag)
    {
        if (constant)
            put_string("const ");
        put_stringf
        (
            "unsigned long %s_start       = %s;\n",
            prefix.c_str(),
            format_address(range.get_lowest()).c_str()
        );
        if (constant)
            put_string("const ");
        put_stringf
        (
            "unsigned long %s_finish      = %s;\n",
            prefix.c_str(),
            format_address(range.get_highest()).c_str()
        );
    }

    if (constant)
        put_string("const ");
    put_stringf
    (
        "unsigned long %s_length      = %s;\n",
        prefix.c_str(),
        format_address(range.get_highest() - range.get_lowest()).c_str()
    );

    //
    // Some folks prefer #define instead
    //
    put_char('\n');

    std::string PREFIX = toupper(prefix);
    put_stringf
    (
        "#define %s_TERMINATION %s\n",
        PREFIX.c_str(),
        format_address(taddr).c_str()
    );
    put_stringf
    (
        "#define %s_START       %s\n",
        PREFIX.c_str(),
        format_address(range.get_lowest()).c_str()
    );
    put_stringf
    (
        "#define %s_FINISH      %s\n",
        PREFIX.c_str(),
        format_address(range.get_highest()).c_str()
    );
    put_stringf
    (
        "#define %s_LENGTH      %s\n",
        PREFIX.c_str(),
        format_address(range.get_highest() - range.get_lowest()).c_str()
    );
    if (section_style)
    {
        put_stringf
        (
            "#define %s_SECTIONS    %s\n",
            PREFIX.c_str(),
            format_address(nsections).c_str()
        );
    }

    if (include)
    {
        std::string insulation = identifier(include_file_name);
        FILE *fp = fopen(include_file_name.c_str(), "w");
        if (!fp)
            fatal_error_errno("open %s", include_file_name.c_str());
        fprintf(fp, "#ifndef %s\n", insulation.c_str());
        fprintf(fp, "#define %s\n", insulation.c_str());
        if((output_type != output_byte) && (output_type != output_unsigned_short))
            fprintf(fp, "#include <stdint.h>\n");
        fprintf(fp, "\n");
        if (enable_goto_addr_flag)
        {
            fprintf(fp, "extern ");
            if (constant)
                fprintf(fp, "const ");
            fprintf
            (
                fp,
                "unsigned long %s_termination;\n",
                prefix.c_str()
            );
        }
        if (enable_footer_flag)
        {
            fprintf(fp, "extern ");
            if (constant)
                fprintf(fp, "const ");
            fprintf(fp, "unsigned long %s_start;\n", prefix.c_str());
            fprintf(fp, "extern ");
            if (constant)
                fprintf(fp, "const ");
            fprintf(fp, "unsigned long %s_finish;\n", prefix.c_str());
        }
        fprintf(fp, "extern ");
        if (constant)
            fprintf(fp, "const ");
        fprintf(fp, "unsigned long %s_length;\n", prefix.c_str());
        if (section_style)
        {
            fprintf(fp, "extern ");
            if (constant)
                fprintf(fp, "const ");
            fprintf(fp, "unsigned long %s_sections;\n", prefix.c_str());
        }
        fprintf(fp, "extern ");
        if (constant)
            fprintf(fp, "const ");
        fprintf(fp, "%s %s[];\n", output_c_type(), prefix.c_str());
        if (section_style)
        {
            fprintf(fp, "extern ");
            if (constant)
                fprintf(fp, "const ");
            fprintf(fp, "unsigned long");
            fprintf(fp, " %s_address[];\n", prefix.c_str());
            if (output_type != output_byte)
            {
                fprintf(fp, "extern ");
                if (constant)
                    fprintf(fp, "const ");
                fprintf(fp, "unsigned long");
                fprintf(fp, " %s_word_address[];\n", prefix.c_str());
            }
            fprintf(fp, "extern ");
            if (constant)
                fprintf(fp, "const ");
            fprintf(fp, "unsigned long");
            fprintf(fp, " %s_length_of_sections[];\n", prefix.c_str());
        }
        fprintf(fp, "\n");
        fprintf(fp, "#endif /* %s */\n", insulation.c_str());

        if (fclose(fp))
            fatal_error_errno("write %s", include_file_name.c_str());
    }
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


static std::string
build_include_file_name(const std::string &filename)
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
    return (std::string(fn, ep - fn) + ".h");
}


srecord::output_file_c::output_file_c(const std::string &a_file_name) :
    srecord::output_file(a_file_name),
    include_file_name(build_include_file_name(a_file_name))
{
}


srecord::output::pointer
srecord::output_file_c::create(const std::string &a_file_name)
{
    return pointer(new srecord::output_file_c(a_file_name));
}


void
srecord::output_file_c::command_line(srecord::arglex_tool *cmdln)
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
        case srecord::arglex_tool::token_constant:
            cmdln->token_next();
            constant = true;
            break;

        case srecord::arglex_tool::token_constant_not:
            cmdln->token_next();
            constant = false;
            break;

        case srecord::arglex_tool::token_include:
            cmdln->token_next();
            include = true;
            break;

        case srecord::arglex_tool::token_include_not:
            cmdln->token_next();
            include = false;
            break;

        case srecord::arglex_tool::token_c_compressed:
            cmdln->token_next();
            hex_style = true;
            section_style = true;
            break;

        case srecord::arglex_tool::token_output_word:
            cmdln->token_next();
            if (cmdln->token_cur() == arglex::token_number)
            {
                int word_len = cmdln->value_number();
                cmdln->token_next();
                switch(word_len){
                    case 2:
                    case 16:
                        output_type = output_uint16_t;
                        break;
                    case 4:
                    case 32:
                        output_type = output_uint32_t;
                        break;
                    case 8:
                    case 64:
                        output_type = output_uint64_t;
                        break;
                    default:
                        fatal_error("word length %d not understood", word_len);
                        // NOTREACHED
                }
            } else {
                output_type = output_unsigned_short;
            }
            break;

        case srecord::arglex_tool::token_style_hexadecimal:
            cmdln->token_next();
            hex_style = true;
            break;

        case srecord::arglex_tool::token_style_hexadecimal_not:
            cmdln->token_next();
            hex_style = false;
            break;

        case srecord::arglex_tool::token_prefix:
            cmdln->token_next();
            if (cmdln->token_cur() == arglex::token_string)
            {
                header_prefix = cmdln->value_string();
                cmdln->token_next();
            }
            break;

        case srecord::arglex_tool::token_postfix:
            cmdln->token_next();
            if (cmdln->token_cur() == arglex::token_string)
            {
                header_postfix = cmdln->value_string();
                cmdln->token_next();
            }
            break;

        case srecord::arglex_tool::token_style_section:
        case srecord::arglex_tool::token_a430:
        case srecord::arglex_tool::token_cl430:
            cmdln->token_next();
            section_style = true;
            break;

        default:
            return;
        }
    }
}

char const *
srecord::output_file_c::output_c_type() const
{
    switch(output_type){
        case output_uint16_t:
            return "uint16_t";
        case output_uint32_t:
            return "uint32_t";
        case output_uint64_t:
            return "uint64_t";
        case output_unsigned_short:
            return "unsigned short";
        default:
            return "unsigned char";
    }
}

void
srecord::output_file_c::emit_header()
{
    if (header_done)
        return;
    if((output_type != output_byte) && (output_type != output_unsigned_short))
        put_string("#include <stdint.h>\n");

    if (header_prefix.length() > 0)
    {
        put_string(header_prefix.c_str());
        put_string(" ");
    }
    if (constant)
        put_stringf("const ");
    put_string(output_c_type());
    put_char(' ');
    put_string(prefix.c_str());
    put_string("[] ");
    if (header_postfix.length() > 0)
    {
        put_string(header_postfix.c_str());
        put_string(" ");
    }
    put_string("=\n{\n");
    header_done = true;
    column = 0;
}

uint64_t 
srecord::output_file_c::max_word_value() const
{
    switch(output_type){
    case output_unsigned_short:
    case output_uint16_t:
        return UINT16_MAX;
    case output_uint32_t:
        return UINT32_MAX;
    case output_uint64_t:
        return UINT64_MAX;
    default:
    case output_byte:
        return UINT8_MAX;
    }
}

void
srecord::output_file_c::emit_word(uint64_t n)
{
    char buffer[30];
    if (hex_style) {
        if(bytes_per_word() == 8) {
            snprintf(buffer, sizeof(buffer), "0x%16.16" PRIX64, n);
        } else if (bytes_per_word() == 4) {
            snprintf(buffer, sizeof(buffer), "0x%8.8" PRIX64, n);
        } else if (bytes_per_word() == 2) {
            snprintf(buffer, sizeof(buffer), "0x%4.4" PRIX64, n);
        } else {
            snprintf(buffer, sizeof(buffer), "0x%2.2" PRIX64, n);
        }
    } else {
        snprintf(buffer, sizeof(buffer), "%" PRIu64, n);
    }
    int len = strlen(buffer);

    if (column && column + 2 + len > line_length)
    {
        put_char('\n');
        column = 0;
    }
    if (column)
    {
        put_char(' ');
        ++column;
    }
    put_string(buffer);
    column += len;
    put_char(',');
    ++column;
}


std::string
srecord::output_file_c::format_address(unsigned long addr) const
{
    char buffer[30];
    if (hex_style)
        snprintf(buffer, sizeof(buffer), "0x%0*lX", address_length * 2, addr);
    else
        snprintf(buffer, sizeof(buffer), "%lu", addr);
    return buffer;
}


void
srecord::output_file_c::write(const srecord::record &record)
{
    switch (record.get_type())
    {
    default:
        // ignore
        break;

    case srecord::record::type_header:
        // emit header records as comments in the file
        {
            put_string("/* ");
            if (record.get_address() != 0)
            {
                unsigned long addr = record.get_address();
                put_stringf("%08lX: ", addr);
            }
            const unsigned char *cp = record.get_data();
            const unsigned char *ep = cp + record.get_length();
            while (cp < ep)
            {
                unsigned char c = *cp++;
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

    case srecord::record::type_data:
        {
            emit_header();
            unsigned const word_size = bytes_per_word();
            if ((record.get_address() & (word_size - 1)) || (record.get_length() & (word_size - 1)))
                fatal_alignment_error(word_size);

            unsigned long min = record.get_address();
            unsigned long max = record.get_address() + record.get_length();
            if (!section_style && !range.empty())
            {
                // Fill gaps in data with words with all bits set
                while (current_address < min)
                {
                    emit_word(max_word_value());
                    current_address += word_size;
                }
            }

            range += interval(min, max);

            for (size_t j = 0; j < record.get_length(); j += word_size)
            {
                // Read a word as little endian
                uint64_t w = 0u;
                for(size_t k = 0u; k < word_size; k++){
                    uint64_t shiftin = record.get_data(j + k);
                    w = w | (shiftin << (8 * k));
                }

                emit_word(w);
            }
            current_address = max;
        }
        break;

    case srecord::record::type_execution_start_address:
        taddr = record.get_address();
        break;
    }
}


void
srecord::output_file_c::line_length_set(int n)
{
    line_length = n;
}


void
srecord::output_file_c::address_length_set(int n)
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

    case 16:
        address_length = 2;
        break;

    case 32:
        address_length = 4;
        break;
    }
}


bool
srecord::output_file_c::preferred_block_size_set(int nbytes)
{
    if (nbytes < 1 || nbytes > record::max_data_length)
        return false;
    if (nbytes & (bytes_per_word()-1))
        return false;
    return true;
}

unsigned
srecord::output_file_c::bytes_per_word() const
{
    switch(output_type) {
    case output_unsigned_short:
    case output_uint16_t:
        return 2;
    case output_uint32_t:
        return 4;
    case output_uint64_t:
        return 8;
    case output_byte:
    default:
        return 1;
    }
}

int
srecord::output_file_c::preferred_block_size_get()
    const
{
    //
    // Use the largest we can get,
    // but be careful about words.
    //
    return (srecord::record::max_data_length & ~(bytes_per_word()-1));
}


const char *
srecord::output_file_c::format_name()
    const
{
    switch(output_type){
    case output_unsigned_short:
        return "C-Array (16-bit)";
    case output_uint16_t:
        return "C-Array (uint16_t)";
    case output_uint32_t:
        return "C-Array (uint32_t)";
    case output_uint64_t:
        return "C-Array (uint64_t)";
    case output_byte:
    default:
        return "C-Array (8-bit)";
    }
}
