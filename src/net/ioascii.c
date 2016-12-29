/*
The MIT License (MIT)

Copyright (c) 2016 Marcin Borowicz <marcinbor85@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include "ioascii.h"

#include <string.h>
#include <stddef.h>

#define IOASCII_BYTE_START	':'
#define IOASCII_BYTE_END        '\n'

static int8_t put_char(struct io_comm_interface *comm, uint8_t ch)
{
	return comm->put(&ch, 1);
}

static int8_t get_char(struct io_comm_interface *comm, uint8_t *ch)
{
	uint32_t i;
	return comm->get(ch, &i);
}

static int8_t parse_high_nibble(struct ioascii_object *ascii, uint8_t ch, uint8_t *data)
{
	if (ch >= '0' && ch <= '9') {
		data[ascii->rx_count] = (ch-'0') << 4;
		ascii->rx_state = 2;
	} else if (ch >= 'a' && ch <= 'f') {
		data[ascii->rx_count] = (ch-'a'+10) << 4;
		ascii->rx_state = 2;
	} else if (ch >= 'A' && ch <= 'F') {
		data[ascii->rx_count] = (ch-'A'+10) << 4;
		ascii->rx_state = 2;
	} else if (ch == IOASCII_BYTE_START) {
		ascii->rx_count = 0;
	} else if (ch == IOASCII_BYTE_END) {
		ascii->rx_state = 0;
		return 1;
	} else {
		ascii->rx_state = 0;
	}
	return 0;
}

static void parse_low_nibble(struct ioascii_object *ascii, uint8_t ch, uint8_t *data)
{
	if (ch >= '0' && ch <= '9') {
		data[ascii->rx_count] |= (ch-'0');
		if (++ascii->rx_count >= ascii->max_rx_size) ascii->rx_state = 0;
		else ascii->rx_state = 1;
	} else if (ch >= 'a' && ch <= 'f') {
		data[ascii->rx_count] |= (ch-'a'+10);
		if (++ascii->rx_count >= ascii->max_rx_size) ascii->rx_state = 0;
		else ascii->rx_state = 1;
	} else if (ch >= 'A' && ch <= 'F') {
		data[ascii->rx_count] |= (ch-'A'+10);
		if (++ascii->rx_count >= ascii->max_rx_size) ascii->rx_state = 0;
		else ascii->rx_state = 1;
	} else if (ch == IOASCII_BYTE_START) {
		ascii->rx_count = 0;
		ascii->rx_state = 1;
	} else {
		ascii->rx_state = 0;
	}
}

static int8_t read(void *self, uint8_t *data, uint32_t *size)
{
        struct ioascii_object *ascii = self;
        struct io_object *io = &ascii->io;
	struct io_comm_interface *comm = io->comm;
	
	uint8_t ch;
	
	while (get_char(comm, &ch) > 0) {
		switch (ascii->rx_state) {
		case 0:
			if (ch == IOASCII_BYTE_START) {
				ascii->rx_count = 0;
				ascii->rx_state = 1;
			}
			break;
		case 1:
			if (parse_high_nibble(ascii, ch, data) != 0) {
				*size = ascii->rx_count;
				return 1;
			}
			break;
		case 2:
			parse_low_nibble(ascii, ch, data);
			break;
		}
	}

        return 0;
}

static int8_t write(void *self, uint8_t *data, uint32_t size)
{
        struct ioascii_object *ascii = self;
        struct io_object *io = &ascii->io;
	struct io_comm_interface *comm = io->comm;
	uint8_t n;

	if (put_char(comm, IOASCII_BYTE_START) != 1) return 0;
	
	while (size > 0) {
		n = (*data)>>4;
		if (n < 10) {
			if (put_char(comm, '0' + n) != 1) return 0;
		} else {
			if (put_char(comm, 'A' + n - 10) != 1) return 0;
		}
		n = (*data)&0x0F;
		if (n < 10) {
			if (put_char(comm, '0' + n) != 1) return 0;
		} else {
			if (put_char(comm, 'A' + n - 10) != 1) return 0;
		}
		data++;
		size--;
	}
	
	if (put_char(comm, IOASCII_BYTE_END) != 1) return 0;
	
        return 1;
}

int8_t ioascii_init(void *self, struct io_comm_interface *comm, uint32_t max_rx_size)
{
        struct ioascii_object *ascii = self;
        struct io_object *io = &ascii->io;
        
        if (io_init(self, comm) != 0) return -1;
        
	if (max_rx_size == 0) return -1;
        if (comm == NULL) return -1;
        
	ascii->max_rx_size = max_rx_size;
        ascii->rx_count = 0;
	ascii->rx_state = 0;
        
        io->read = read;
        io->write = write;
        
        return 0;
}