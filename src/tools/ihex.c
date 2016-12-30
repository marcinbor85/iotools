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

#include "ihex.h"

#include <stddef.h>

static uint8_t check_crc(uint8_t *data, uint32_t size)
{
	uint8_t sum = 0;
	while (size-- > 0)
		sum += *data++;
	return sum;
}

int8_t ihex_parse_line(struct ihex_object *ihex, uint8_t *line_data, uint32_t line_size)
{
	uint32_t adr;

	struct ihex_line_data *line = &ihex->line;

	if (line_size < 5)
		return -1;

	if (check_crc(line_data, line_size) != 0)
		return -1;

	line->data_size = line_data[0];
	line->address = (((line_data[1] << 8) & 0xFF00) | ((line_data[2]) & 0x00FF)) & 0xFFFF;
	line->type = line_data[3];

	if (line_size == 5) {
		line->data = NULL;
	} else {
		line->data = &line_data[4];
	}
	
	if (line->data_size != line_size - 5)
		return -1;

	switch (line->type) {
	case 0x00:
		if (line->data_size == 0)
			return -1;

		adr = line->address + ihex->address;
		ihex->iface->data(ihex, adr, ihex->line.data, ihex->line.data_size);
		break;
	case 0x01:
		if (line->data_size != 0)
			return -1;
		if (line->address != 0)
			return -1;

		ihex->iface->end(ihex);
		break;
	case 0x04:
		if (line->data_size != 2)
			return -1;
		if (line->address != 0)
			return -1;
		
		ihex->address = (line->data[0] << 24) | (line->data[1] << 16);
		break;
	default:
		if (ihex->iface->custom != NULL)
			ihex->iface->custom(ihex, line);
		break;
	}

	return 0;
}

void ihex_reset(struct ihex_object *ihex)
{
	ihex->address = 0;
	ihex->line.address = 0;
	ihex->line.data = NULL;
	ihex->line.data_size = 0;
	ihex->line.type = 0;
}

int8_t ihex_init(struct ihex_object *ihex, struct ihex_line_interface *iface)
{
	if (ihex == NULL) return -1;
	if (iface == NULL) return -1;

	if (iface->data == NULL) return -1;
	if (iface->end == NULL) return -1;

	ihex->iface = iface;

	ihex_reset(ihex);

	return 0;
}
