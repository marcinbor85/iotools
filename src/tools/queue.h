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

#ifndef QUEUE_H
#define QUEUE_H

#include <stdint.h>

struct queue_object {
        void *buf;
        uint32_t capacity;
        uint32_t item_size;
        uint32_t count;
        int8_t (*put)(void *queue, void *item);
        int8_t (*get)(void *queue, void *item);
};

int8_t queue_init(void *self, void *buf, uint32_t capacity, uint32_t item_size);
int8_t queue_put(void *self, void *item);
int8_t queue_get(void *self, void *item);

#endif /* QUEUE_H */
