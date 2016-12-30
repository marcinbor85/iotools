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

#include "iocantp.h"

#include <string.h>
#include <stddef.h>

static int8_t recv_single_frame(struct iocantp_object *cantp, uint8_t *data)
{
	cantp->rx_size = cantp->buf[0] & 0x3F;
	if (cantp->rx_size > cantp->max_rx_size) return 0;
	if (cantp->rx_size > (cantp->max_frame_size - 1)) return 0;
	if (cantp->rx_size > (cantp->data_len - 1)) return 0;

	memcpy(data, &cantp->buf[1], cantp->rx_size);
	
	return 1;
}

static int8_t recv_multi_frame(struct iocantp_object *cantp, uint8_t *data)
{
	cantp->rx_size = (((cantp->buf[0] & 0x3F) << 8) | ((cantp->buf[1]) & 0xFF)) & 0x3FFF;
	if (cantp->rx_size > cantp->max_rx_size) return 0;
	if (cantp->rx_size < cantp->max_frame_size) return 0;
	if (cantp->data_len < cantp->max_frame_size) return 0;

	memcpy(data, &cantp->buf[2], cantp->max_frame_size - 2);
	cantp->rx_count = cantp->max_frame_size - 2;
	cantp->rx_cntr = 1;
	
	return 1;
}

static int8_t recv_next_frame(struct iocantp_object *cantp, uint8_t *data)
{
	uint32_t to_receive;
	
	if ((cantp->buf[0] & 0x3F) != cantp->rx_cntr) {
		cantp->rx_state = 0;
		return 0;
	}

	to_receive = cantp->rx_size - cantp->rx_count;
	if (to_receive > (cantp->max_frame_size - 1)) to_receive = cantp->max_frame_size - 1;

	if ((cantp->data_len - 1) < to_receive) {
		cantp->rx_state = 0;
		return 0;
	}

	memcpy(&data[cantp->rx_count], &cantp->buf[1], to_receive);
	cantp->rx_count += to_receive;
	cantp->rx_cntr++;
	cantp->rx_cntr &= 0x3F;

	if (cantp->rx_count == cantp->rx_size) {
		cantp->rx_state = 0;
		return 1;
	}
	
	return 0;
}


static int8_t parse_first_frame(struct iocantp_object *cantp, uint8_t *data)
{
	switch (cantp->buf[0] & 0xC0) {
	case 0x00:
		if (recv_single_frame(cantp, data) != 0) return 1;
		break;
	case 0x40:
		if (recv_multi_frame(cantp, data) != 0) cantp->rx_state = 1;
		break;
	default:
		break;
	}
	return 0;
}

static int8_t parse_next_frame(struct iocantp_object *cantp, uint8_t *data)
{
	switch (cantp->buf[0] & 0xC0) {
	case 0x80:
		if (recv_next_frame(cantp, data) != 0) return 1;
		break;
	case 0x40:
		recv_multi_frame(cantp, data);
		break;
	case 0x00:
		if (recv_single_frame(cantp, data) != 0) {
			cantp->rx_state = 0;
			return 1;
		}
		break;
	default:
		cantp->rx_state = 0;
	}
	return 0;
}

static int8_t read(void *self, uint8_t *data, uint32_t *size)
{
	struct iocantp_object *cantp = self;
	struct io_object *io = &cantp->io;
	struct io_comm_interface *comm = io->comm;

	while (comm->get(cantp->buf, &cantp->data_len) > 0) {
		if (cantp->data_len == 0) continue;

		switch (cantp->rx_state) {
		case 0:
			if (parse_first_frame(cantp, data) != 0) {
				*size = cantp->rx_size;
				return 1;
			}
			break;
		case 1:
			if (parse_next_frame(cantp, data) != 0) {
				*size = cantp->rx_size;
				return 1;
			}
			break;
		}
	}
	return 0;
}

static int8_t write(void *self, uint8_t *data, uint32_t size)
{
	struct iocantp_object *cantp = self;
	struct io_object *io = &cantp->io;
	struct io_comm_interface *comm = io->comm;

	uint32_t i;
	uint32_t sended;
	uint32_t to_send;

	if (size <= (cantp->max_frame_size - 1)) {
		cantp->buf[0] = size;
		memcpy(&cantp->buf[1], data, size);
		if (comm->put(cantp->buf, size + 1) != 1) return 0;
	} else if (size <= 0x3FFF) {
		sended = 0;
		cantp->buf[0] = 0x40 | ((size >> 8) & 0x3F);
		cantp->buf[1] = size & 0xFF;
		memcpy(&cantp->buf[2], data, cantp->max_frame_size - 2);
		sended = cantp->max_frame_size - 2;
		if (comm->put(cantp->buf, cantp->max_frame_size) != 1) return 0;
		i = 1;
		while (sended < size) {
			cantp->buf[0] = 0x80 | (i & 0x3F);
			if (sended + (cantp->max_frame_size - 1) > size) {
				to_send = size - sended;
			} else {
				to_send = (cantp->max_frame_size - 1);
			}
			memcpy(&cantp->buf[1], &data[sended], to_send);
			if (comm->put(cantp->buf, to_send + 1) != 1) return 0;
			sended += to_send;
			i++;
		}
	} else {
		return 0;
	}

	return 1;
}

int8_t iocantp_init(void *self, struct io_comm_interface *comm, uint8_t *buf, uint32_t max_frame_size, uint32_t max_rx_size)
{
	struct iocantp_object *cantp = self;
	struct io_object *io = &cantp->io;

	if (io_init(self, comm) != 0) return -1;

	if (buf == NULL) return -1;
	if ((max_frame_size < 8) || (max_frame_size > 64)) return -1;
	if (max_rx_size == 0) return -1;

	cantp->buf = buf;
	cantp->max_frame_size = max_frame_size;
	cantp->max_rx_size = max_rx_size;
	cantp->data_len = 0;
	cantp->rx_state = 0;
	cantp->rx_count = 0;
	cantp->rx_size = 0;
	cantp->rx_cntr = 0;

	io->read = read;
	io->write = write;

	return 0;
}