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

#include "net/ioslip.h"

#include <string.h>

static int8_t put(uint8_t *data, uint32_t size);
static int8_t get(uint8_t *data, uint32_t *size);

static struct ioslip_object io;
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
	static uint8_t buf[] = {0x12, 0xC0, 0x10, 0xC2, 0xCC, 0xC1, 0xFE, 0xC1, 0xC0, 0xFA, 0xC0, 0xAA, 0xC2, 0xCB, 0xC1, 0xC0, 0xC1, 0xC0, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xC1, 0xC0, 0xC2, 0xCB, 0xC1};
	
	if (i < sizeof(buf)) {
		*data = buf[i];
		i++;	
		return 1;
	}
	
	return 0;
}

static int ioslip_create(void)
{
	ASSERT(ioslip_init(&io, &iface, 0) == -1);
	ASSERT(ioslip_init(&io, &iface, 8) == 0);
	
	ASSERT(io.io.comm == &iface);
	ASSERT(io.io.read != NULL);
	ASSERT(io.io.write != NULL);
	
	ASSERT(io.max_rx_size == 8);
	ASSERT(io.rx_state == 0);
	ASSERT(io.rx_count == 0);
	
	return 0;
}

static int ioslip_api_read(void)
{
	uint8_t buf[8];
	uint32_t size;
	
	ASSERT(ioslip_init(&io, &iface, 8) == 0);
	
	ASSERT(io_read(&io, NULL, NULL) == -1);
	ASSERT(io_read(&io, buf, NULL) == -1);
	ASSERT(io_read(&io, NULL, &size) == -1);
	
	ASSERT(io_read(&io, buf, &size) == 1);
	ASSERT(size == 2);
	ASSERT(buf[0] == 0x10);
	ASSERT(buf[1] == 0xC2);
	
	ASSERT(io_read(&io, buf, &size) == 1);
	ASSERT(size == 2);
	ASSERT(buf[0] == 0xAA);
	ASSERT(buf[1] == 0xC1);
	
	ASSERT(io_read(&io, buf, &size) == 1);
	ASSERT(size == 0);
	
	ASSERT(io_read(&io, buf, &size) == 1);
	ASSERT(size == 1);
	ASSERT(buf[0] == 0xC1);

	return 0;
}

static int ioslip_api_write(void)
{
	uint8_t buf[8];
	uint32_t size;
	
	ASSERT(ioslip_init(&io, &iface, 8) == 0);
	
	ASSERT(io_write(&io, NULL, 0) == -1);
	ASSERT(io_write(&io, NULL, 1) == -1);
	ASSERT(io_write(&io, buf, 0) == -1);
	
	buf[0] = 0xC3;
	buf[1] = 0x01;
	size = 2;
	buf_tx_size = 0;
	ASSERT(io_write(&io, buf, size) == 1);
	ASSERT(buf_tx_size == 4);
	ASSERT(buf_tx[0] == 0xC0);
	ASSERT(buf_tx[1] == 0xC3);
	ASSERT(buf_tx[2] == 0x01);
	ASSERT(buf_tx[3] == 0xC1);
	
	buf[0] = 0xC0;
	buf[1] = 0xC1;
	buf[2] = 0xC2;	
	size = 3;
	buf_tx_size = 0;
	ASSERT(io_write(&io, buf, size) == 1);
	ASSERT(buf_tx_size == 8);
	ASSERT(buf_tx[0] == 0xC0);
	ASSERT(buf_tx[1] == 0xC2);
	ASSERT(buf_tx[2] == 0xCA);
	ASSERT(buf_tx[3] == 0xC2);
	ASSERT(buf_tx[4] == 0xCB);
	ASSERT(buf_tx[5] == 0xC2);
	ASSERT(buf_tx[6] == 0xCC);
	ASSERT(buf_tx[7] == 0xC1);
	
	return 0;
}

int test_ioslip(void)
{
	VERIFY(ioslip_create);
	VERIFY(ioslip_api_read);
	VERIFY(ioslip_api_write);

	return 0;
}