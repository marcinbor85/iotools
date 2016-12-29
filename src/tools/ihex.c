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

static int8_t check_crc()
{
	
}

int8_t ihex_parse_line(struct ihex_object *ihex, uint8_t *line, uint8_t size)
{
	uint8_t data_len;
	uint8_t type;
	uint8_t line_crc;
	uint16_t line_adr;
	uint32_t adr;
	
	if (size < 5) return 0;	
	
	ihex->line_size = size;
	ihex->line = line;
	
	data_len = ihex->line[0];
	line_adr = (((ihex->line[1] << 8) & 0xFF00) | ((ihex->line[2]) & 0x00FF)) & 0xFFFF;
	type = ihex->line[3];	
	line_crc = ihex->line[4];
	
	check_crc();
	
	switch (type) {
	case 0x00:
		break;
	case 0x01:
		if (data_len != 0) return 0;
		if (line_adr != 0) return 0;
		
		ihex->listeners->end(ihex);
		break;
	case 0x04:
		ihex->address = ((ihex->line[1] << 24) | (ihex->line[2] << 16)) & 0xFFFF0000;
		break;
	case 0x05:
		if (data_len != 4) return 0;
		if (line_adr != 0) return 0;
		
		if (ihex->listeners->start != NULL) {
			adr = ((ihex->line[1] << 24) | (ihex->line[2] << 16)) 
			ihex->listeners->start(ihex, adr);
		}
		break;
	default:
		break;
	}
	
	return 1;
}

void ihex_reset(struct ihex_object *ihex)
{
	ihex->address = 0;
	ihex->line = NULL;
	ihex->line_size = 0;
}

int8_t ihex_init(struct ihex_object *ihex, struct ihex_line_listeners *lst)
{
	if (ihex == NULL) return -1;
	if (lst == NULL) return -1;
	
	if (lst->data == NULL) return -1;
	if (lst->end == NULL) return -1;
	
	ihex->listeners = lst;
	
	ihex_reset(ihex);
}
