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

static inline void dl_list_replace(struct dl_list *new, struct dl_list *old)
{
    new->next = old->next;
    new->prev = old->prev;
    new->next->prev = new;
    new->prev->next = new;
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

static inline int dl_list_empty(struct dl_list *list)
{
    return list->next == list;
}

static inline int dl_list_len(struct dl_list *list)
{
    int len = 0;
    for (struct dl_list *temp = list->next; temp != list; temp = temp->next)
    {
        len++;
    }
    return len;
}
#define dl_list_entry(item, type, member) \
    ((type *)((char *)item - offsetof(type, member)))

#define dl_list_get_first(list, type, member) \
    dl_list_entry((list)->next, type, member)

#define dl_list_get_tail(list, type, member) \
    dl_list_entry((list)->prev, type, member)

#define dl_list_for_each(item, head, type, member)         \
    for (item = dl_list_entry((head)->next, type, member); \
         &item->member != (head);                          \
         item = dl_list_entry(item->member.next, type, member))

#define dl_list_free(head, type, member)          \
    do                                            \
    {                                             \
        struct dl_list *temp = (head)->next;      \
        while (temp != (head))                    \
        {                                         \
            struct dl_list *n = temp;             \
            temp = temp->next;                    \
            free(dl_list_entry(n, type, member)); \
        }                                         \
        (head)->next = (head)->prev = (head);     \
    } while (0)

#endif
