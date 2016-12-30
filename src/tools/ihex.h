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

#ifndef IHEX_H
#define IHEX_H

#include <stdint.h>

struct ihex_line_data {
	uint8_t *data;
	uint8_t data_size;
	uint16_t address;
	uint8_t type;
};

struct ihex_object {
	uint32_t address;
	struct ihex_line_data line;
	struct ihex_line_interface *iface;
};

struct ihex_line_interface {
	void (*data)(struct ihex_object *ihex, uint32_t adr, uint8_t *data, uint8_t size);
	void (*custom)(struct ihex_object *ihex, struct ihex_line_data *line);
	void (*end)(struct ihex_object *ihex);
};

int8_t ihex_init(struct ihex_object *ihex, struct ihex_line_interface *iface);
int8_t ihex_parse_line(struct ihex_object *ihex, uint8_t *line, uint32_t size);
void ihex_reset(struct ihex_object *ihex);


#endif /* IHEX_H */

