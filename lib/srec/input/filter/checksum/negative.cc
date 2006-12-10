//
//	srecord - manipulate eprom load files
//	Copyright (C) 2000-2002, 2006 Peter Miller;
//	All rights reserved.
//
//	This program is free software; you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published by
//	the Free Software Foundation; either version 2 of the License, or
//	(at your option) any later version.
//
//	This program is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.
//
//	You should have received a copy of the GNU General Public License
//	along with this program; if not, write to the Free Software
//	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111, USA.
//
// MANIFEST: functions to impliment the srec_input_filter_checksum_negative class
//


#include <lib/srec/input/filter/checksum/negative.h>


srec_input_filter_checksum_negative::srec_input_filter_checksum_negative(
		srec_input *a1, int a2, int a3, int a4, int a5) :
	srec_input_filter_checksum(a1, a2, a3, a4, a5)
{
}


srec_input_filter_checksum_negative::~srec_input_filter_checksum_negative()
{
}


srec_input_filter_checksum::sum_t
srec_input_filter_checksum_negative::calculate()
{
	return -sum;
}
