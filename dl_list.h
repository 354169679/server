#ifndef DOUBLY_LIST_H
#define DOUBLY_LIST_H

#include <stddef.h>

/* doubly-linked list */

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

/// @brief 链表头部添加节点(链表像栈一样工作)
/// @param list list
/// @param item item
static inline void dl_list_add(struct dl_list *list, struct dl_list *item)
{
    item->prev = list;
    item->next = list->next;
    list->next->prev = item;
    list->next = item;
}

/// @brief 链表尾部添加节点(链表像队列一样工作)
/// @param list list
/// @param item item
static inline void dl_list_add_to_tail(struct dl_list *list, struct dl_list *item)
{
    dl_list_add(list->prev, item);
}

/// @brief 链表判空
/// @param list list
/// @return list空返回1,非空返回0
static inline int dl_list_empty(struct dl_list *list)
{
    return list == list->next;
}

static inline int dl_list_len(struct dl_list *list)
{
    int len = 0;
    for (struct dl_list *temp = list; temp != list; temp = temp->next)
    {
        len++;
    }
    return len;
}

#define dl_list_entry(item, type, member) \
    ((type *)((char *)item - offsetof(type, member)))

/* 获取list链表所指向的第一个元素 */
#define dl_list_first(list, type, member) \
    dl_list_entry((list)->next, type, member)

/* 获取list链表所指向的最后一个元素 */
#define dl_list_tail(list, type, member) \
    dl_list_entry((list)->prev, type, member)

/* 遍历所有节点 */
#define dl_list_for_each(item, head, type, member) \
    for (item = dl_list_first(head, type, member); \
         &item->member != (head);                  \
         item = dl_list_entry(item->member.next, type, member))

#define dl_list_for_each_reverse(item, head, type, member) \
    for (item = dl_list_tail(head, type, member);          \
         &item->member != head;                            \
         item = dl_list_entry((item->member.prev), type, member))

#endif
