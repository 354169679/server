#include "heap.h"
#include "util.h"

static inline void swap(int *n, int *m)
{
    int temp;
    temp = *n;
    *n = *m;
    *m = temp;
}

static void adjust_up(heap *hp, size_t child_idx)
{
    size_t root_idx = (child_idx - 1) / 2; /* 根据子节点推断父节点 */
    int *root_node = &hp->data[root_idx], *child_node = &hp->data[child_idx];
    while (*root_node < *child_node)
    {
        swap(root_node, child_node);
        child_idx = root_idx;
        root_idx = (child_idx - 1) / 2;
        root_node = &hp->data[root_idx];
        child_node = &hp->data[child_idx];
    }
}

static void adjust_down(heap *hp, size_t child_idx)
{
    size_t root_idx = (child_idx - 1) / 2; /* 根据子节点推断父节点 */
    int *root_node = &hp->data[root_idx], *child_node = &hp->data[child_idx];
    while (*root_node > *child_node)
    {
        swap(root_node, child_node);
        child_idx = root_idx;
        root_idx = (child_idx - 1) / 2;
        root_node = &hp->data[root_idx];
        child_node = &hp->data[child_idx];
    }
}

heap *heap_init(size_t heap_size)
{
    heap *heap_ptr = zalloc(sizeof(heap));

    if (!heap_ptr)
    {
        print_error("allocte heap");
        return NULL;
    }

    if (heap_size == 0)
    {
        heap_ptr->data = zalloc(sizeof(int) * DEFAULT_HEAP_SIZE);
        heap_ptr->heap_size = DEFAULT_HEAP_SIZE;
    }
    else
    {
        heap_ptr->data = zalloc(sizeof(int) * heap_size);
        heap_ptr->heap_size = heap_size;
    }

    if (!heap_ptr->data)
    {
        free(heap_ptr);
        return NULL;
    }

    heap_ptr->idx = 0;

    return heap_ptr;
}

void heap_destroy(heap *hp)
{
    if (hp)
    {
        if (hp->data)
            free(hp->data);
        free(hp);
    }
}

heap *heap_insert(heap *hp, int new_data)
{
    if (hp->heap_size == hp->idx)
    {
        hp->data = realloc(hp->data, hp->heap_size * 2);
        if (!hp->data)
        {
            heap_destroy(hp);
            return NULL;
        }
        hp->heap_size *= 2;
    }

    int child_idx = hp->idx;
    hp->data[child_idx] = new_data;
    hp->heap_size++;

    adjust_down(hp, child_idx);
    hp->idx++;
}

heap *heap_top(heap *hp)
{
    return hp->data[]
}
