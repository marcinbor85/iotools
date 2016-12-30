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

#include "net/io.h"

static int8_t put(uint8_t *data, uint32_t size);
static int8_t get(uint8_t *data, uint32_t *size);

static struct io_object io;
static struct io_comm_interface iface3;
static struct io_comm_interface iface2 = {.put = put};
static struct io_comm_interface iface1 = {.get = get};
static struct io_comm_interface iface = {.put = put, .get = get};
static uint8_t data[8];
static uint32_t size = 8;

static int8_t put(uint8_t *data, uint32_t size)
{
	return 0;
}

static int8_t get(uint8_t *data, uint32_t *size)
{
	return 0;
}

static int io_create(void)
{
	ASSERT(io_init(NULL, NULL) == -1);
	ASSERT(io_init(NULL, &iface) == -1);
	ASSERT(io_init(&io, NULL) == -1);
	ASSERT(io_init(&io, &iface3) == -1);
	ASSERT(io_init(&io, &iface2) == -1);
	ASSERT(io_init(&io, &iface1) == -1);
	ASSERT(io_init(&io, &iface) == 0);

	ASSERT(io.comm == &iface);
	ASSERT(io.read == NULL);
	ASSERT(io.write == NULL);

	return 0;
}

static int io_api(void)
{
	ASSERT(io_init(&io, &iface) == 0);

	ASSERT(io_read(&io, data, &size) == -1);
	ASSERT(io_write(&io, data, size) == -1);

	return 0;
}

int test_io(void)
{
	VERIFY(io_create);
	VERIFY(io_api);

	return 0;
}