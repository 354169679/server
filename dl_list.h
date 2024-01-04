#ifndef DL_LIST_H
#define DL_LIST_H

#include <stddef.h>

struct dl_list
{
    struct dl_list *prev;
    struct dl_list *next;
};

#define DL_LIST_INIT(dl_name)  \
    {                          \
        (&dl_name), (&dl_name) \
    }

static inline void dl_list_init(struct dl_list *list)
{
    list->next = list;
    list->prev = list;
}

static inline void dl_list_add(struct dl_list *list, struct dl_list *item)
{
    item->prev = list;
    item->next = list->next;
    list->next->prev = item;
    list->next = item;
}

static inline void dl_list_add_to_tail(struct dl_list *list, struct dl_list *item)
{
    dl_list_add(list->prev, item);
}

#define dl_list_empty(head) \
    (head)->next == head

#define dl_list_entry(item, type, member) \
    ((type *)((char *)item - offsetof(type, member)))

#define dl_list_get_first(list, type, member) \
    dl_list_entry((list)->next, type, member)

#define dl_list_get_tail(list, type, member) \
    dl_list_entry((list)->prev, type, member)

#define dl_list_for_each_node(item, head, type, member)    \
    for (item = dl_list_entry((head)->next, type, member); \
         &item->member != (head);                          \
         item = dl_list_entry(item->member.next, type, member))

#define dl_list_len(item, head, type, member, len)  \
    dl_list_for_each_node(item, head, type, member) \
        len++;

#define dl_list_free(item, head, type, member) \
    for (item = dl_list_entry(item, type, member);\
        &item->member!=(head)\
        item = dl_list_entry(item,)
    )

#endif
