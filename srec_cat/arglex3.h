//
//      srecord - manipulate eprom load files
//      Copyright (C) 2000-2008, 2010, 2014 Peter Miller
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

#ifndef PROG_SREC_CAT_ARGLEX3_H
#define PROG_SREC_CAT_ARGLEX3_H

#include <srecord/arglex/tool.h>

class srec_cat_arglex3:
    public srecord::arglex_tool
{
public:
    enum
    {
        token_line_length = srecord::arglex_tool::token_MAX,
        token_address_length,
        token_crlf,
        token_data_only,
        token_header,
        token_execution_start_address,
        token_enable,
        token_disable,
        token_line_termination,
        token_output_block_size,
        token_output_block_packing,
        token_output_block_align,
        token_MAX
    };

    /**
      * The destructor.
      */
    ~srec_cat_arglex3() override = default;

    /**
      * The constructor.
      */
    srec_cat_arglex3(int, char **);

    /**
      * The default constructor.
      */
    srec_cat_arglex3() = delete;

    /**
      * The copy constructor.
      */
    srec_cat_arglex3(const srec_cat_arglex3 &) = delete;

    /**
      * The assignment operator.
      */
    srec_cat_arglex3 &operator=(const srec_cat_arglex3 &) = delete;

    /**
      * The move constructor
      */
    srec_cat_arglex3 ( srec_cat_arglex3 && ) = delete;

    /**
      * The move assignment operator
      */
    srec_cat_arglex3& operator=(srec_cat_arglex3&&) = delete;

};

#endif // PROG_SREC_CAT_ARGLEX3_H
