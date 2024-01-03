#ifndef DL_LIST_H
#define DL_LIST_H

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

static inline void dl_list_tail(struct dl_list *list, struct dl_list *item)
{
    dl_list_add(list->prev, item);
}

#endif
