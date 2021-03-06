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

#include "queue.h"

#include <stddef.h>

int8_t queue_init(void *self, void *buf, uint32_t capacity, uint32_t item_size)
{
	struct queue_object *queue = self;

	if (self == NULL)
		return -1;
	if (buf == NULL)
		return -1;
	if (capacity == 0)
		return -1;
	if (item_size == 0)
		return -1;

	queue->buf = buf;
	queue->count = 0;
	queue->capacity = capacity;
	queue->item_size = item_size;
	queue->put = NULL;
	queue->get = NULL;

	return 0;
}

int8_t queue_put(void *self, void *item)
{
	struct queue_object *queue = self;

	if (self == NULL)
		return -1;
	if (item == NULL)
		return -1;

	if (queue->put == NULL)
		return -1;

	return queue->put(queue, item);
}

int8_t queue_get(void *self, void *item)
{
	struct queue_object *queue = self;

	if (self == NULL)
		return -1;
	if (item == NULL)
		return -1;

	if (queue->get == NULL)
		return -1;

	return queue->get(queue, item);
}