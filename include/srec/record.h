/*
 *	srecord - manipulate eprom load files
 *	Copyright (C) 1998 Peter Miller;
 *	All rights reserved.
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program; if not, write to the Free Software
 *	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * MANIFEST: interface definition for lib/srec/record.cc
 */

#ifndef INCLUDE_SREC_RECORD_H
#define INCLUDE_SREC_RECORD_H

#pragma interface "srec_record"

class srec_record
{
public:
	enum type
	{
		type_unknown,
		type_header,
		type_data,
		type_data_count,
		type_termination
	};
	typedef enum type type;
	typedef unsigned long address_t;
	typedef unsigned char data_t;
	srec_record();
	srec_record(const srec_record &);
	srec_record(type);
	srec_record(type, address_t);
	srec_record(type, address_t, const data_t *, int);
	srec_record &operator=(const srec_record &);
	~srec_record();

	address_t get_address() const { return address; }
	address_t get_address_end() const { return (address + length); }
	void set_address(address_t arg) { address = arg; }
	int get_length() const { return length; }
	void set_length(int arg) { if (arg < 0) arg = 0;
		if (arg < length) length = arg; }
	const data_t *get_data() const { return data; }
	int get_data(int n) const { return data[n]; }
	void set_data(int n, int d) { data[n] = d; }
	type get_type() const { return itype; }
	void set_type(type arg) { itype = arg; }

	static int maximum_data_length(address_t);
	static address_t decode_big_endian(data_t *, int);
	static void encode_big_endian(data_t *, address_t, int);
	static void encode_little_endian(data_t *, address_t, int);

private:
	type		itype;
	address_t	address;
	int		length;
	data_t		data[255];
};

#endif /* INCLUDE_SREC_RECORD_H */
