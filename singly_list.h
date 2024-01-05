#ifndef SINGLY_LIST_H
#define SINGLY_LIST_H

#include <stddef.h>

/* singly-linked list */

struct sl_list
{
    struct sl_list *next;
};

static inline void sl_list_init(struct sl_list *list)
{
    list->next = list;
}

static inline void sl_list_add(struct sl_list *list, struct sl_list *item)
{
    item->next = list->next;
    list->next = item;
}

#define sl_list_entry(item, type, member) \
    (type *)((char *)(item)-offsetof(type, member))

#define sl_list_for_each(item, head, type, member)         \
    for (item = sl_list_entry((head)->next, type, member); \
         &(item->member) != (head);                        \
         item = sl_list_entry((item->member).next, type, member))

#endif
