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

#include "tools/queue.h"

static struct queue_object queue;
static uint8_t buffer[256];

static int queue_create(void)
{
	ASSERT(queue_init(NULL, buffer, 16, sizeof(char)) == -1);
	ASSERT(queue_init(&queue, NULL, 16, sizeof(char)) == -1);
	ASSERT(queue_init(&queue, buffer, 0, sizeof(char)) == -1);
	ASSERT(queue_init(&queue, buffer, 16, 0) == -1);

	ASSERT(queue_init(&queue, buffer, 16, sizeof(char)) == 0);

	ASSERT(queue.buf == buffer);
	ASSERT(queue.capacity == 16);
	ASSERT(queue.item_size == sizeof(char));
	ASSERT(queue.count == 0);
	ASSERT(queue.get == NULL);
	ASSERT(queue.put == NULL);

	return 0;
}

static int queue_api(void)
{
	char ch = 0;

	ASSERT(queue_init(&queue, buffer, 16, 1) == 0);

	ASSERT(queue_put(NULL, NULL) == -1);
	ASSERT(queue_put(NULL, &ch) == -1);
	ASSERT(queue_put(&queue, NULL) == -1);
	ASSERT(queue_put(&queue, &ch) == -1);

	ASSERT(queue_get(NULL, NULL) == -1);
	ASSERT(queue_get(NULL, &ch) == -1);
	ASSERT(queue_get(&queue, NULL) == -1);
	ASSERT(queue_get(&queue, &ch) == -1);

	return 0;
}

int test_queue(void)
{
	VERIFY(queue_create);
	VERIFY(queue_api);

	return 0;
}