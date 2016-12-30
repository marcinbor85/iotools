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

#include "tools/ihex.h"

#include <stdio.h>

static void data(struct ihex_object *ihex, uint32_t adr, uint8_t *data, uint8_t size);
static void custom(struct ihex_object *ihex, struct ihex_line_data *line);
static void end(struct ihex_object *ihex);

static struct ihex_object ihex;

static struct ihex_line_interface iface = {.data = data, .custom = custom, .end = end};
static struct ihex_line_interface iface1 = {.data = data, .end = end};
static struct ihex_line_interface iface2 = {.data = data};
static struct ihex_line_interface iface3 = {.end = end};
static struct ihex_line_interface iface4 = {};

static uint8_t custom_flag = 0;
static uint8_t end_flag = 0;

static uint8_t data_flag = 0;
static uint8_t data_bytes[256];
static uint8_t data_size;
static uint32_t data_adr;

static struct ihex_line_data *custom_line;

static void data(struct ihex_object *ihex, uint32_t adr, uint8_t *data, uint8_t size)
{
	data_adr = adr;
	data_size = 0;	
	while (size-- > 0) data_bytes[data_size++] = *data++;
	data_flag = 1;
}

static void custom(struct ihex_object *ihex, struct ihex_line_data *line)
{
	custom_line = line;
	custom_flag = 1;
}

static void end(struct ihex_object *ihex)
{
	end_flag = 1;
}

static int ihex_create(void)
{
	ASSERT(ihex_init(NULL, NULL) == -1);
	ASSERT(ihex_init(NULL, &iface) == -1);
	ASSERT(ihex_init(&ihex, NULL) == -1);
	ASSERT(ihex_init(&ihex, &iface4) == -1);
	ASSERT(ihex_init(&ihex, &iface3) == -1);
	ASSERT(ihex_init(&ihex, &iface2) == -1);
	ASSERT(ihex_init(&ihex, &iface1) == 0);

	ASSERT(ihex_init(&ihex, &iface) == 0);

	ASSERT(ihex.address == 0);
	ASSERT(ihex.iface == &iface);

	ASSERT(ihex.line.address == 0);
	ASSERT(ihex.line.data == NULL);
	ASSERT(ihex.line.data_size == 0);
	ASSERT(ihex.line.type == 0);

	return 0;
}

static int ihex_parse_end(void)
{
	uint8_t data[] = {0x00, 0x00, 0x00, 0x01, 0xFF};
	uint8_t data1[] = {0x00, 0x00, 0x00, 0x01, 0xFE};
	uint8_t data2[] = {0x01, 0x00, 0x00, 0x01, 0xFE};
	uint8_t data3[] = {0x00, 0x00, 0x01, 0x01, 0xFE};

	ASSERT(ihex_init(&ihex, &iface) == 0);

	end_flag = 0;
	ASSERT(ihex_parse_line(&ihex, data1, sizeof(data1)) == -1);
	ASSERT(end_flag == 0);
	
	end_flag = 0;
	ASSERT(ihex_parse_line(&ihex, data2, sizeof(data2)) == -1);
	ASSERT(end_flag == 0);
	
	end_flag = 0;
	ASSERT(ihex_parse_line(&ihex, data3, sizeof(data3)) == -1);
	ASSERT(end_flag == 0);
	
	end_flag = 0;
	ASSERT(ihex_parse_line(&ihex, data, sizeof(data)) == 0);
	ASSERT(ihex.line.type == 0x01);
	ASSERT(end_flag == 1);

	return 0;
}

static int ihex_parse_data(void)
{
	uint8_t data[] = {0x02, 0x20, 0x00, 0x00, 0x01, 0xFF, 0xDE};
	uint8_t data1[] = {0x02, 0x20, 0x00, 0x00, 0x01, 0xFF, 0xDD};
	uint8_t data2[] = {0x03, 0x20, 0x00, 0x00, 0x01, 0xFF, 0xDD};
	uint8_t data3[] = {0x01, 0x20, 0x00, 0x00, 0x01, 0xFF, 0xDF};
	uint8_t data4[] = {0x10, 0x01, 0x28, 0x00, 0x80, 0x31, 0xF7, 0x00, 0x77, 0x08, 0x20, 0x00, 0xE5, 0x00, 0x67, 0x08, 0x65, 0x06, 0x03, 0x19, 0xA5};
	
	ASSERT(ihex_init(&ihex, &iface) == 0);

	data_flag = 0;
	ASSERT(ihex_parse_line(&ihex, data, sizeof(data)) == 0);
	ASSERT(ihex.line.type == 0x00);
	ASSERT(data_flag == 1);
	ASSERT(data_adr == 0x00002000);
	ASSERT(data_size == 2);
	ASSERT(data_bytes[0] == 0x01);
	ASSERT(data_bytes[1] == 0xFF);	
	
	data_flag = 0;
	ASSERT(ihex_parse_line(&ihex, data1, sizeof(data1)) == -1);
	ASSERT(data_flag == 0);
	
	ASSERT(ihex_parse_line(&ihex, data2, sizeof(data2)) == -1);
	ASSERT(ihex_parse_line(&ihex, data3, sizeof(data3)) == -1);
	
	data_flag = 0;
	ASSERT(ihex_parse_line(&ihex, data4, sizeof(data4)) == 0);
	ASSERT(data_flag == 1);
	ASSERT(data_adr == 0x00000128);
	ASSERT(data_size == 16);
	ASSERT(data_bytes[0] == 0x80);
	ASSERT(data_bytes[1] == 0x31);	
	ASSERT(data_bytes[14] == 0x03);
	ASSERT(data_bytes[15] == 0x19);	

	return 0;
}

static int ihex_parse_address(void)
{
	uint8_t data_adr0[] = {0x02, 0x00, 0x00, 0x04, 0xF0, 0x10, 0xFA};
	uint8_t data_adr1[] = {0x02, 0x00, 0x01, 0x04, 0xF0, 0x10, 0xF9};
	uint8_t data_adr2[] = {0x01, 0x00, 0x00, 0x04, 0xF0, 0x10, 0xFB};
	
	uint8_t data1[] = {0x02, 0xF0, 0x00, 0x00, 0x01, 0xFF, 0x0E};
	uint8_t data2[] = {0x02, 0x11, 0x00, 0x00, 0x02, 0xFE, 0xED};

	ASSERT(ihex_init(&ihex, &iface) == 0);

	ASSERT(ihex.address == 0);
	ASSERT(ihex_parse_line(&ihex, data_adr0, sizeof(data_adr0)) == 0);
	ASSERT(ihex.address == 0xF0100000);
	ASSERT(ihex.line.type == 0x04);
	
	ASSERT(ihex_parse_line(&ihex, data_adr1, sizeof(data_adr1)) == -1);
	ASSERT(ihex_parse_line(&ihex, data_adr2, sizeof(data_adr2)) == -1);
	
	data_flag = 0;
	ASSERT(ihex_parse_line(&ihex, data1, sizeof(data1)) == 0);
	ASSERT(data_flag == 1);
	ASSERT(data_adr == 0xF010F000);
	ASSERT(data_size == 2);
	ASSERT(data_bytes[0] == 0x01);
	ASSERT(data_bytes[1] == 0xFF);	
	
	data_flag = 0;
	ASSERT(ihex_parse_line(&ihex, data2, sizeof(data2)) == 0);
	ASSERT(data_flag == 1);
	ASSERT(data_adr == 0xF0101100);
	ASSERT(data_size == 2);
	ASSERT(data_bytes[0] == 0x02);
	ASSERT(data_bytes[1] == 0xFE);	

	return 0;
}

static int ihex_parse_custom(void)
{
	uint8_t data[] = {0x02, 0x20, 0x00, 0x00, 0x00, 0x00, 0xDE};
	uint8_t data_custom1[] = {0x02, 0x20, 0x00, 0x10, 0x01, 0xFF, 0xCE};
	uint8_t data_custom2[] = {0x01, 0x20, 0x00, 0x00, 0x01, 0xFF, 0xDF};
	uint8_t data_custom3[] = {0x03, 0x20, 0x00, 0x00, 0x01, 0xFF, 0xDD};
	uint8_t data_custom4[] = {0x03, 0x20, 0x00, 0x00, 0x01, 0xFF, 0xDA};
	
	ASSERT(ihex_init(&ihex, &iface) == 0);

	custom_flag = 0;
	ASSERT(ihex_parse_line(&ihex, data, sizeof(data)) == 0);
	ASSERT(custom_flag == 0);
	ASSERT(ihex.line.type == 0x00);
	
	custom_flag = 0;
	ASSERT(ihex_parse_line(&ihex, data_custom2, sizeof(data_custom2)) == -1);
	ASSERT(custom_flag == 0);
	
	ASSERT(ihex_parse_line(&ihex, data_custom2, sizeof(data_custom3)) == -1);
	ASSERT(ihex_parse_line(&ihex, data_custom2, sizeof(data_custom4)) == -1);
	
	custom_flag = 0;
	custom_line = NULL;
	ASSERT(ihex_parse_line(&ihex, data_custom1, sizeof(data_custom1)) == 0);
	ASSERT(custom_flag == 1);
	ASSERT(custom_line == &ihex.line);
	ASSERT(custom_line->data[0] == 0x01);
	ASSERT(custom_line->data[1] == 0xFF);
	ASSERT(custom_line->data_size == 2);
	ASSERT(custom_line->address == 0x2000);
	
	ASSERT(ihex_init(&ihex, &iface1) == 0);
	
	custom_flag = 0;
	ASSERT(ihex_parse_line(&ihex, data_custom1, sizeof(data_custom1)) == 0);
	ASSERT(custom_flag == 0);
	
	return 0;
}

int test_ihex(void)
{
	VERIFY(ihex_create);
	VERIFY(ihex_parse_data);
	VERIFY(ihex_parse_address);
	VERIFY(ihex_parse_custom);
	VERIFY(ihex_parse_end);

	return 0;
}