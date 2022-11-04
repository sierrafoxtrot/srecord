//
// srecord - manipulate eprom load files
// Copyright (C) 1998, 1999, 2002, 2003, 2006-2013 Peter Miller
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at
// your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//

#include <cassert>
#include <cctype>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <unistd.h>

#include <srecord/arglex.h>
#include <srecord/progname.h>
#include <srecord/quit.h>
#include <srecord/versn_stamp.h>

// Cygwin's mingw has the execvp prototype in the wrong place.
#ifdef __MSVCRT__
#include <process.h>
#endif

static const srecord::arglex::table_ty default_table[] =
{
    { "-",            srecord::arglex::token_stdio,       },
    { "-Help",        srecord::arglex::token_help,        },
    { "-LICense",     srecord::arglex::token_license,     },
    { "-Page_Length", srecord::arglex::token_page_length, },
    { "-Page_Width",  srecord::arglex::token_page_width,  },
    { "-TRACIng",     srecord::arglex::token_tracing,     },
    { "-Verbose",     srecord::arglex::token_verbose,     },
    { "-VERSion",     srecord::arglex::token_version,     },
    SRECORD_ARGLEX_END_MARKER
};


srecord::arglex::arglex() :
    usage_tail_(nullptr)
{
    table_set(default_table);
}


srecord::arglex::arglex(int ac, char **av) :
    usage_tail_(nullptr)
{
    progname_set(av[0]);
    for (int j = 1; j < ac; ++j)
    {
        if (av[j][0] == '@') {
            read_arguments_file(av[j] + 1);
        } else {
            arguments.emplace_back(av[j]);
}
    }
    table_set(default_table);
}


void
srecord::arglex::read_arguments_file(const char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (fp == nullptr) {
        quit_default.fatal_error_errno("open \"%s\"", filename);
}
    for (;;)
    {
        int sc = getc(fp);
        if (sc == EOF) {
            break;
}
        unsigned char c = sc;

        //
        // Ignore white space between words.
        //
        if (isspace(c) != 0) {
            continue;
}

        //
        // Ignore comments
        //
        if (c == '#')
        {
            for (;;)
            {
                sc = getc(fp);
                if (sc == EOF || sc == '\n') {
                    break;
}
            }
            continue;
        }
        char buffer[1000];
        char *bp = buffer;
        for (;;)
        {
            if (bp < buffer + sizeof(buffer) - 1) {
                *bp++ = c;
}
            sc = getc(fp);
            if (sc == EOF) {
                break;
}
            c = sc;
            if (isspace(c) != 0) {
                break;
}
            if (c == '#')
            {
                ungetc(c, fp);
                break;
            }
        }
        *bp = 0;
        if (buffer[0] == '@') {
            read_arguments_file(buffer + 1);
        } else {
            arguments.emplace_back(buffer, bp - buffer);
}
    }
    fclose(fp);
}


srecord::arglex::~arglex()
= default;


void
srecord::arglex::table_set(const table_ty *tp)
{
    tables.push_back(tp);
}


static const char *partial;


auto
srecord::arglex::compare(const char *formal, const char *actual) -> bool
{
    for (;;)
    {
        unsigned char ac = *actual++;
        if (isupper(ac) != 0) {
            ac = tolower(ac);
}
        unsigned char fc = *formal++;
        switch (fc)
        {
        case 0:
            return ac == 0u;

        case '_':
            if (ac == '-') {
                break;
}
            // fall through...

        case 'a': case 'b': case 'c': case 'd': case 'e':
        case 'f': case 'g': case 'h': case 'i': case 'j':
        case 'k': case 'l': case 'm': case 'n': case 'o':
        case 'p': case 'q': case 'r': case 's': case 't':
        case 'u': case 'v': case 'w': case 'x': case 'y':
        case 'z':
            //
            // optional characters
            //
            if (ac == fc && compare(formal, actual)) {
                return true;
}

            //
            // skip forward to next
            // mandatory character, or after '_'
            //
            while (islower(*formal) != 0) {
                ++formal;
}
            if (*formal == '_')
            {
                ++formal;
                if (ac == '_' || ac == '-') {
                    ++actual;
}
            }
            --actual;
            break;

        case '*':
            //
            // This is incomplete, it should really
            // check for a match match of the stuff after
            // the '*', too, a la glob.
            //
            if (ac == 0u) {
                return false;
}
            partial = actual - 1;
            return true;

        case '\\':
            if (actual[-1] != *formal++) {
                return false;
}
            break;

        case 'A': case 'B': case 'C': case 'D': case 'E':
        case 'F': case 'G': case 'H': case 'I': case 'J':
        case 'K': case 'L': case 'M': case 'N': case 'O':
        case 'P': case 'Q': case 'R': case 'S': case 'T':
        case 'U': case 'V': case 'W': case 'X': case 'Y':
        case 'Z':
            fc = tolower(fc);
            // fall through...

        default:
            //
            // mandatory characters
            //
            if (fc != ac) {
                return false;
}
            break;
        }
    }
}


//
// NAME
//      is_a_number
//
// SYNOPSIS
//      int is_a_number(char *s);
//
// DESCRIPTION
//      The is_a_number function is used to determine if the
//      argument is a number.
//
//      The value is placed in arglex_value.alv_number as
//      a side effect.
//
//      Negative and positive signs are accepted.
//      The C conventions for decimal, octal and hexadecimal are understood.
//
//      There may be no white space anywhere in the string,
//      and the string must end after the last digit.
//      Trailing garbage will be interpreted to mean it is not a string.
//
// ARGUMENTS
//      s       - string to be tested and evaluated
//
// RETURNS
//      int;    zero if not a number,
//              non-zero if is a number.
//

static auto
is_a_number(const char *s, long &n) -> int
{
    int         sign;

    n = 0;
    switch (*s)
    {
    case '-':
        ++s;
        sign = -1;
        break;

    case '+':
        ++s;
        sign = 1;
        break;

    default:
        sign = 1;
        break;
    }
    switch (*s)
    {
    case '0':
        if ((s[1] == 'x' || s[1] == 'X') && (s[2] != 0))
        {
            s += 2;
            for (;;)
            {
                switch (*s)
                {
                case '0': case '1': case '2': case '3':
                case '4': case '5': case '6': case '7':
                case '8': case '9':
                    n = n * 16 + *s++ - '0';
                    continue;

                case 'A': case 'B': case 'C':
                case 'D': case 'E': case 'F':
                    n = n * 16 + *s++ - 'A' + 10;
                    continue;

                case 'a': case 'b': case 'c':
                case 'd': case 'e': case 'f':
                    n = n * 16 + *s++ - 'a' + 10;
                    continue;
                }
                break;
            }
        }
        else
        {
            for (;;)
            {
                switch (*s)
                {
                case '0': case '1': case '2': case '3':
                case '4': case '5': case '6': case '7':
                    n = n * 8 + *s++ - '0';
                    continue;
                }
                break;
            }
        }
        break;

    case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
        for (;;)
        {
            switch (*s)
            {
            case '0': case '1': case '2': case '3':
            case '4': case '5': case '6': case '7':
            case '8': case '9':
                n = n * 10 + *s++ - '0';
                continue;
            }
            break;
        }
        break;

default:
        return 0;
    }
    if (*s != 0) {
        return 0;
}
    n *= sign;
    return 1;
}


static auto
starts_with(const std::string &haystack, const std::string &needle) -> bool
{
    return
        (
            haystack.size() >= needle.size()
        &&
            0 == memcmp(haystack.c_str(), needle.c_str(), needle.size())
        );
}


static auto
ends_with(const std::string &haystack, const std::string &needle) -> bool
{
    return
        (
            haystack.size() >= needle.size()
        &&
            (
                0
            ==
                memcmp
                (
                    haystack.c_str() + haystack.size() - needle.size(),
                    needle.c_str(),
                    needle.size()
                )
            )
        );
}


static void
deprecated_warning(const char *deprecated_name, const char *preferred_name)
{
    srecord::quit_default.warning
    (
        R"(option "%s" is deprecated, please use "%s" instead)",
        deprecated_name,
        preferred_name
    );
}


//
// NAME
//      arglex
//
// SYNOPSIS
//      int arglex::token_next(void);
//
// DESCRIPTION
//      The arglex function is used to perform lexical analysis
//      on the command line arguments.
//
//      Unrecognised options are returned as arglex_token_option
//      for anything starting with a '-', or
//      arglex_token_string otherwise.
//
// RETURNS
//      The next token in the token stream.
//      When the end is reached, arglex_token_eoln is returned forever.
//
// CAVEAT
//      Must call arglex_init before this function is called.
//

auto
srecord::arglex::token_next() -> int
{
    const table_ty  *tp;
    const table_ty  *hit[20];
    int             nhit;

    std::string arg;
    if (!pushback.empty())
    {
        //
        // the second half of a "-foo=bar" style argument.
        //
        arg = pushback.back();
        pushback.pop_back();
    }
    else
    {
        if (arguments.empty())
        {
            value_string_ = "";
            token = token_eoln;
            return token;
        }
        arg = arguments.front();
        arguments.pop_front();

        //
        // See if it looks like a GNU "-foo=bar" option.
        // Split it at the '=' to make it something the
        // rest of the code understands.
        //
        if (arg[0] == '-' && arg[1] != '=')
        {
            const char *eqp = strchr(arg.c_str(), '=');
            if (eqp != nullptr)
            {
                pushback.emplace_back(eqp + 1);
                arg = std::string(arg.c_str(), eqp - arg.c_str());
            }
        }

        //
        // Turn the GNU-style leading "--"
        // into "-" if necessary.
        //
        if
        (
            arg.size() > 2
        &&
            arg[0] == '-'
        &&
            arg[1] == '-'
        &&
            (is_a_number(arg.c_str() + 1, value_number_) == 0)
        ) {
            arg = std::string(arg.c_str() + 1);
}
    }
    value_string_ = arg;

    //
    // see if it is a number
    //
    if (is_a_number(arg.c_str(), value_number_) != 0)
    {
        token = arglex::token_number;
        return token;
    }

    //
    // scan the tables to see what it matches
    //
    nhit = 0;
    partial = nullptr;
    for
    (auto & table : tables)
    {
        for (tp = table; tp->name != nullptr; ++tp)
        {
            if (compare(tp->name, arg.c_str())) {
                hit[nhit++] = tp;
}

            // big endian deprecated variants
            assert(!starts_with(tp->name, "-Big_Endian_"));
            if (ends_with(tp->name, "_Big_Endian"))
            {
                std::string name2 =
                    "-Big_Endian_" +
                    std::string(tp->name + 1, strlen(tp->name) - 12);
                if (compare(name2.c_str(), arg.c_str()))
                {
                    hit[nhit++] = tp;
                    deprecated_warning(name2.c_str(), tp->name);
                }
            }

            // little endian deprecated variants
            assert(!starts_with(tp->name, "-Little_Endian_"));
            if (ends_with(tp->name, "_Little_Endian"))
            {
                std::string name3 =
                    "-Little_Endian_" +
                    std::string(tp->name + 1, strlen(tp->name) - 15);
                if (compare(name3.c_str(), arg.c_str()))
                {
                    hit[nhit++] = tp;
                    deprecated_warning(name3.c_str(), tp->name);
                }
            }
        }
    }

    //
    // deal with unknown or ambiguous options
    //
    switch (nhit)
    {
    case 0:
        //
        // not found in the tables
        //
        if (value_string_[0] == '-') {
            token = arglex::token_option;
        } else {
            token = arglex::token_string;
}
        break;

    default:
        //
        // if all the hits are the same, it isn't ambiguous
        //
        {
            bool all_same = true;
            std::string possibilities = hit[0]->name;
            for (int k = 1; k < nhit; ++k)
            {
                if (hit[0]->token != hit[k]->token) {
                    all_same = false;
}
                possibilities += ", ";
                possibilities += hit[k]->name;
            }
            if (!all_same)
            {
                fatal_error
                (
                    "option \"%s\" is ambiguous, did you mean one of: %s?",
                    value_string_.c_str(),
                    possibilities.c_str()
                );
                // NOTREACHED
            }
        }
        // fall through...

    case 1:
        if (partial != nullptr)
        {
            pushback.emplace_back(partial);
            partial = nullptr;
        }
        value_string_ = hit[0]->name;
        token = hit[0]->token;
        check_deprecated(arg);
        break;
    }
    return token;
}


void
srecord::arglex::deprecated_option(const std::string &old_fashioned)
{
    deprecated_options.push_back(old_fashioned);
}


void
srecord::arglex::check_deprecated(const std::string &actual)
    const
{
    for
    (auto formal : deprecated_options)
    {
        if (compare(formal.c_str(), actual.c_str()))
        {
            deprecated_warning(formal.c_str(), token_name(token));
        }
    }
}


auto
srecord::arglex::token_name(int n)
    const -> const char *
{
    switch (n)
    {
    case token_eoln:
        return "end of command line";

    case token_number:
        return "number";

    case token_option:
        return "option";

    case token_stdio:
        return "standard input or output";

    case token_string:
        return "string";

    default:
        break;
    }
    for
    (auto tp : tables)
    {
        for (; tp->name != nullptr; ++tp)
        {
            if (tp->token == n) {
                return tp->name;
}
        }
    }
    return "unknown command line token";
}


void
srecord::arglex::help(const char *name)
    
{
    if (name == nullptr) {
        name = progname_get();
}
    const char *cmd[3] = { "man", name, nullptr };
    execvp(cmd[0], (char *const *)cmd);
    std::cerr << cmd[0] << ": " << strerror(errno) << std::endl;
    exit(1);
}


void
srecord::arglex::version()
    
{
    print_version();
    exit(0);
}


void
srecord::arglex::license()
    const
{
    help("srec_license");
}


void
srecord::arglex::bad_argument()
    const
{
    switch (token_cur())
    {
    case token_string:
        std::cerr << "misplaced file name (\"" << value_string()
            << "\") on command line" << std::endl;
        break;

    case token_number:
        std::cerr << "misplaced number (" << value_string()
            << ") on command line" << std::endl;
        break;

    case token_option:
        std::cerr << "unknown \"" << value_string() << "\" option" << std::endl;
        break;

    case token_eoln:
        std::cerr << "command line too short" << std::endl;
        break;

    default:
        std::cerr << "misplaced \"" << value_string() << "\" option"
            << std::endl;
        break;
    }
    usage();
    // NOTREACHED
}


auto
srecord::arglex::token_first() -> int
{
#if 1
    // We probably don't need to do this all the time.
    // How do we distinguish Peter's build from a package build?
    test_ambiguous();
#endif

    switch (token_next())
    {
    default:
        return token_cur();

    case token_help:
        if (token_next() != token_eoln) {
            bad_argument();
}
        help();
        break;

    case token_version:
        if (token_next() != token_eoln) {
            bad_argument();
}
        version();
        break;

    case token_license:
        if (token_next() != token_eoln) {
            bad_argument();
}
        license();
        break;
    }
    exit(0);
}


void
srecord::arglex::usage_tail_set(const char *s)
{
    usage_tail_ = s;
}


auto
srecord::arglex::usage_tail_get()
    const -> const char *
{
    if (usage_tail_ == nullptr) {
        usage_tail_ = "<filename>...";
}
    return usage_tail_;
}


void
srecord::arglex::usage()
    const
{
    std::cerr << "Usage: " << progname_get() << " [ <option>... ] "
        << usage_tail_get() << std::endl;
    std::cerr << "       " << progname_get() << " -Help" << std::endl;
    std::cerr << "       " << progname_get() << " -VERSion" << std::endl;
    std::cerr << "       " << progname_get() << " -LICense" << std::endl;
    exit(1);
    // NOTREACHED
}


void
srecord::arglex::default_command_line_processing()
{
    bad_argument();
}


void
srecord::arglex::fatal_error(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    quit_default.fatal_error_v(fmt, ap);
    // NOTREACHED
    va_end(ap);
}
