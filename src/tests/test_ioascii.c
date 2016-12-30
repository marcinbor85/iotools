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

#include "tests.h"

#include "net/ioascii.h"

#include <string.h>

static int8_t put(uint8_t *data, uint32_t size);
static int8_t get(uint8_t *data, uint32_t *size);

static struct ioascii_object io;
static struct io_comm_interface iface = {.put = put, .get = get};
static uint8_t buf_tx[8];
static uint32_t buf_tx_size = 8;

static int8_t put(uint8_t *data, uint32_t size)
{
	buf_tx[buf_tx_size++] = *data;
	return 1;
}

static int8_t get(uint8_t *data, uint32_t *size)
{
	static uint32_t i = 0;
	static uint8_t buf[] = ":0p3a\n:01A28F\n:39b0fe1122dd\n:33a:33116\n:9a3E\n:0102030405060708090A\n:0102\n:\n";

	if (i < (uint32_t) strlen(buf)) {
		*data = buf[i];
		i++;
		return 1;
	}

	return 0;
}

static int ioascii_create(void)
{
	ASSERT(ioascii_init(&io, &iface, 0) == -1);
	ASSERT(ioascii_init(&io, &iface, 8) == 0);

	ASSERT(io.io.comm == &iface);
	ASSERT(io.io.read != NULL);
	ASSERT(io.io.write != NULL);

	ASSERT(io.max_rx_size == 8);
	ASSERT(io.rx_state == 0);
	ASSERT(io.rx_count == 0);

	return 0;
}

static int ioascii_api_read(void)
{
	uint8_t buf[8];
	uint32_t size;

	ASSERT(ioascii_init(&io, &iface, 8) == 0);

	ASSERT(io_read(&io, NULL, NULL) == -1);
	ASSERT(io_read(&io, buf, NULL) == -1);
	ASSERT(io_read(&io, NULL, &size) == -1);

	ASSERT(io_read(&io, buf, &size) == 1);
	ASSERT(size == 3);
	ASSERT(buf[0] == 0x01);
	ASSERT(buf[1] == 0xA2);
	ASSERT(buf[2] == 0x8F);

	ASSERT(io_read(&io, buf, &size) == 1);
	ASSERT(size == 6);
	ASSERT(buf[0] == 0x39);
	ASSERT(buf[1] == 0xB0);
	ASSERT(buf[2] == 0xFE);
	ASSERT(buf[3] == 0x11);
	ASSERT(buf[4] == 0x22);
	ASSERT(buf[5] == 0xDD);

	ASSERT(io_read(&io, buf, &size) == 1);
	ASSERT(size == 2);
	ASSERT(buf[0] == 0x9A);
	ASSERT(buf[1] == 0x3E);

	ASSERT(io_read(&io, buf, &size) == 1);
	ASSERT(size == 2);
	ASSERT(buf[0] == 0x01);
	ASSERT(buf[1] == 0x02);

	ASSERT(io_read(&io, buf, &size) == 1);
	ASSERT(size == 0);

	return 0;
}

static int ioascii_api_write(void)
{
	uint8_t buf[8];
	uint32_t size;

	ASSERT(ioascii_init(&io, &iface, 8) == 0);

	ASSERT(io_write(&io, NULL, 0) == -1);
	ASSERT(io_write(&io, NULL, 1) == -1);
	ASSERT(io_write(&io, buf, 0) == -1);

	buf[0] = 0x1D;
	buf[1] = 0xA9;
	size = 2;
	buf_tx_size = 0;
	ASSERT(io_write(&io, buf, size) == 1);
	ASSERT(buf_tx_size == 6);
	ASSERT(buf_tx[0] == ':');
	ASSERT(buf_tx[1] == '1');
	ASSERT(buf_tx[2] == 'D');
	ASSERT(buf_tx[3] == 'A');
	ASSERT(buf_tx[4] == '9');
	ASSERT(buf_tx[5] == '\n');

	return 0;
}

int test_ioascii(void)
{
	VERIFY(ioascii_create);
	VERIFY(ioascii_api_read);
	VERIFY(ioascii_api_write);

	return 0;
}