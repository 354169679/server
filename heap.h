#ifndef HEAP_H
#define HEAP_H

#include <stddef.h>

#define DEFAULT_HEAP_SIZE 4

typedef struct
{
    int *data;
    size_t heap_size;
    size_t idx;
} heap;

heap *heap_init(size_t heap_size);
void heap_destroy(heap *hp);
heap *heap_insert(heap *hp, int new_data);


#endif
