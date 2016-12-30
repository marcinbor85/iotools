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

#include "tools/queuefifo.h"

static struct queuefifo_object fifo;
static uint8_t buffer[256];

static int queuefifo_create(void)
{
	ASSERT(queuefifo_init(NULL, buffer, 16, sizeof(uint8_t)) == -1);
	ASSERT(queuefifo_init(&fifo, NULL, 16, sizeof(uint8_t)) == -1);
	ASSERT(queuefifo_init(&fifo, buffer, 0, sizeof(uint8_t)) == -1);
	ASSERT(queuefifo_init(&fifo, buffer, 16, 0) == -1);

	ASSERT(queuefifo_init(&fifo, buffer, 16, sizeof(uint8_t)) == 0);

	ASSERT(fifo.queue.buf == buffer);
	ASSERT(fifo.queue.capacity == 16);
	ASSERT(fifo.queue.item_size == sizeof(uint8_t));
	ASSERT(fifo.queue.count == 0);
	ASSERT(fifo.queue.get != NULL);
	ASSERT(fifo.queue.put != NULL);
	
	ASSERT(fifo.head == 0);
	ASSERT(fifo.tail == 0);

	return 0;
}

static int queuefifo_put(void)
{
	uint8_t ch = 0;

	ASSERT(queuefifo_init(&fifo, buffer, 4, sizeof(uint8_t)) == 0);
	
	ASSERT(queue_put(NULL, NULL) == -1);
	ASSERT(queue_put(NULL, &ch) == -1);
	ASSERT(queue_put(&fifo, NULL) == -1);
	
	ch = 0xAA;
	ASSERT(queue_put(&fifo, &ch) == 1);
	ASSERT(fifo.queue.count == 1);
	ASSERT(fifo.tail == 1);
	ASSERT(fifo.head == 0);
	ASSERT(buffer[0] == 0xAA);
	
	ch = 0xBB;
	ASSERT(queue_put(&fifo, &ch) == 1);
	ASSERT(fifo.queue.count == 2);
	ASSERT(fifo.tail == 2);
	ASSERT(fifo.head == 0);
	ASSERT(buffer[1] == 0xBB);
	
	ch = 0xCC;
	ASSERT(queue_put(&fifo, &ch) == 1);
	ASSERT(fifo.queue.count == 3);
	ASSERT(fifo.tail == 3);
	ASSERT(fifo.head == 0);
	ASSERT(buffer[2] == 0xCC);
	
	ch = 0xDD;
	ASSERT(queue_put(&fifo, &ch) == 1);
	ASSERT(fifo.queue.count == 4);
	ASSERT(fifo.tail == 0);
	ASSERT(fifo.head == 0);
	ASSERT(buffer[3] == 0xDD);
	
	ch = 0xEE;
	ASSERT(queue_put(&fifo, &ch) == 0);
	ASSERT(fifo.queue.count == 4);
	ASSERT(fifo.tail == 1);
	ASSERT(fifo.head == 1);
	ASSERT(buffer[0] == 0xEE);
	
	return 0;
}

static int queuefifo_get(void)
{
	uint8_t ch = 0;
	
	ch = 0;
	ASSERT(queue_get(&fifo, &ch) == 1);
	ASSERT(fifo.queue.count == 3);
	ASSERT(fifo.tail == 1);
	ASSERT(fifo.head == 2);
	ASSERT(ch == 0xBB);
	
	ch = 0;
	ASSERT(queue_get(&fifo, &ch) == 1);
	ASSERT(fifo.queue.count == 2);
	ASSERT(fifo.tail == 1);
	ASSERT(fifo.head == 3);
	ASSERT(ch == 0xCC);
	
	ch = 0;
	ASSERT(queue_get(&fifo, &ch) == 1);
	ASSERT(fifo.queue.count == 1);
	ASSERT(fifo.tail == 1);
	ASSERT(fifo.head == 0);
	ASSERT(ch == 0xDD);
	
	ch = 0;
	ASSERT(queue_get(&fifo, &ch) == 1);
	ASSERT(fifo.queue.count == 0);
	ASSERT(fifo.tail == 1);
	ASSERT(fifo.head == 1);
	ASSERT(ch == 0xEE);
	
	ch = 0x11;
	ASSERT(queue_get(&fifo, &ch) == 0);
	ASSERT(fifo.queue.count == 0);
	ASSERT(fifo.tail == 1);
	ASSERT(fifo.head == 1);
	ASSERT(ch == 0x11);
	
	return 0;
}

static int queuefifo_misc(void)
{
	int i;

	ASSERT(queuefifo_init(&fifo, buffer, 2, sizeof(int)) == 0);
	
	i = -5;
	ASSERT(queue_put(&fifo, &i) == 1);
	i = 0x1122;
	ASSERT(queue_put(&fifo, &i) == 1);
	i = -35;
	ASSERT(queue_put(&fifo, &i) == 0);
	
	i = 0;
	ASSERT(queue_get(&fifo, &i) == 1);
	ASSERT(i == 0x1122);
	
	i = 0;
	ASSERT(queue_get(&fifo, &i) == 1);
	ASSERT(i == -35);
	
	i = 0;
	ASSERT(queue_get(&fifo, &i) == 0);
	ASSERT(i == 0);
	
	return 0;
}

int test_queuefifo(void)
{
	VERIFY(queuefifo_create);
	VERIFY(queuefifo_put);
	VERIFY(queuefifo_get);
	VERIFY(queuefifo_misc);

	return 0;
}