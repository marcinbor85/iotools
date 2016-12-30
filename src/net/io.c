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

#include "io.h"

#include <stddef.h>

int8_t io_write(void *self, uint8_t *data, uint32_t size)
{
	struct io_object *io = self;

	if (self == NULL)
		return -1;
	if (data == NULL)
		return -1;
	if (size == 0)
		return -1;

	if (io->write == NULL)
		return -1;

	return io->write(io, data, size);
}

int8_t io_read(void *self, uint8_t *data, uint32_t *size)
{
	struct io_object *io = self;

	if (self == NULL)
		return -1;
	if (data == NULL)
		return -1;
	if (size == NULL)
		return -1;

	if (io->read == NULL)
		return -1;

	return io->read(io, data, size);
}

int8_t io_init(void *self, struct io_comm_interface *comm)
{
	struct io_object *io = self;

	if (self == NULL)
		return -1;
	if (comm == NULL)
		return -1;
	if (comm->get == NULL)
		return -1;
	if (comm->put == NULL)
		return -1;

	io->read = NULL;
	io->write = NULL;

	io->comm = comm;

	return 0;
}