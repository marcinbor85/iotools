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

#include "ioslip.h"

#include <string.h>
#include <stddef.h>

#define IOSLIP_BYTE_START       0xC0
#define IOSLIP_BYTE_END         0xC1
#define IOSLIP_BYTE_ESCAPE      0xC2

#define IOSLIP_ESCAPED_START    0xCA
#define IOSLIP_ESCAPED_END      0xCB
#define IOSLIP_ESCAPED_ESCAPE   0xCC

static int8_t put_char(struct io_comm_interface *comm, uint8_t ch)
{
	return comm->put(&ch, 1);
}

static int8_t get_char(struct io_comm_interface *comm, uint8_t *ch)
{
	uint32_t i;
	return comm->get(ch, &i);
}

static int8_t parse_data_byte(struct ioslip_object *slip, uint8_t ch, uint8_t *data)
{
	switch (ch) {
	case IOSLIP_BYTE_START:
		slip->rx_count = 0;
		break;
	case IOSLIP_BYTE_END:		
		slip->rx_state = 0;
		return 1;
	case IOSLIP_BYTE_ESCAPE:
		slip->rx_state = 2;
		break;
	default:
		data[slip->rx_count] = ch;
		if (++slip->rx_count >= slip->max_rx_size) slip->rx_state = 0;
		break;
	}
	return 0;
}

static int8_t parse_escaped_byte(struct ioslip_object *slip, uint8_t ch, uint8_t *data)
{
	switch (ch) {
	case IOSLIP_ESCAPED_START:
		data[slip->rx_count] = IOSLIP_BYTE_START;
		if (++slip->rx_count >= slip->max_rx_size) slip->rx_state = 0;
		else slip->rx_state = 1;
		break;
	case IOSLIP_ESCAPED_END:
		data[slip->rx_count] = IOSLIP_BYTE_END;
		if (++slip->rx_count >= slip->max_rx_size) slip->rx_state = 0;
		else slip->rx_state = 1;
		break;
	case IOSLIP_ESCAPED_ESCAPE:
		data[slip->rx_count] = IOSLIP_BYTE_ESCAPE;
		if (++slip->rx_count >= slip->max_rx_size) slip->rx_state = 0;
		else slip->rx_state = 1;
		break;
	default:
		slip->rx_state = 0;
		break;
	}
}

static int8_t read(void *self, uint8_t *data, uint32_t *size)
{
        struct ioslip_object *slip = self;
        struct io_object *io = &slip->io;
	struct io_comm_interface *comm = io->comm;
	
	uint8_t ch;
	
	while (get_char(comm, &ch) > 0) {
		switch (slip->rx_state) {
		case 0:
			if (ch == IOSLIP_BYTE_START) {
				slip->rx_count = 0;
				slip->rx_state = 1;
			}
			break;
		case 1:
			if (parse_data_byte(slip, ch, data) != 0) {
				*size = slip->rx_count;
				return 1;
			}
			break;
		case 2:
			parse_escaped_byte(slip, ch, data);
			break;
		}
	}

        return 0;
}

static int8_t write(void *self, uint8_t *data, uint32_t size)
{
        struct ioslip_object *slip = self;
        struct io_object *io = &slip->io;
        struct io_comm_interface *comm = io->comm;
	
        uint32_t i;
        
        if (put_char(comm, IOSLIP_BYTE_START) != 1) return 0;
                
        for (i = 0; i < size; i++) {
                switch (data[i]) {
                case IOSLIP_BYTE_START:
                        if (put_char(comm, IOSLIP_BYTE_ESCAPE) != 1) return 0;
                        if (put_char(comm, IOSLIP_ESCAPED_START) != 1) return 0;
                        break;
                case IOSLIP_BYTE_END:
                        if (put_char(comm, IOSLIP_BYTE_ESCAPE) != 1) return 0;
                        if (put_char(comm, IOSLIP_ESCAPED_END) != 1) return 0;
                        break;
                case IOSLIP_BYTE_ESCAPE:
                        if (put_char(comm, IOSLIP_BYTE_ESCAPE) != 1) return 0;
                        if (put_char(comm, IOSLIP_ESCAPED_ESCAPE) != 1) return 0;
                        break;
                default:
                        if (put_char(comm, data[i]) != 1) return 0;
                        break;
                }
        }
        
        if (put_char(comm, IOSLIP_BYTE_END) != 1) return 0;
        
        return 1;
}

int8_t ioslip_init(void *self, struct io_comm_interface *comm, uint32_t max_rx_size)
{
        struct ioslip_object *slip = self;
        struct io_object *io = &slip->io;
        
        if (io_init(self, comm) != 0) return -1;
        
	if (max_rx_size == 0) return -1;
        if (comm == NULL) return -1;
                
	slip->max_rx_size = max_rx_size;
        slip->rx_count = 0;
	slip->rx_state = 0;
        
        io->read = read;
        io->write = write;
        
        return 0;
}