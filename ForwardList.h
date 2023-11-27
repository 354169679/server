#pragma once

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

struct Node
{
    int data_;
    struct Node *next_;
};

struct ForwardList
{
    struct Node *head_node;
};

#define alloc_node (struct Node *)malloc(sizeof(struct Node))

#define init_list(list)                  \
    do                                   \
    {                                    \
        (list)->head_node = alloc_node;  \
        (list)->head_node->next_ = NULL; \
    } while (0);

#define insert_front_list(list, val)                    \
    do                                              \
    {                                               \
        struct Node *new_node = alloc_node;         \
        assert(new_node);                           \
        new_node->data_ = val;                      \
        new_node->next_ = (list)->head_node->next_; \
        (list)->head_node->next_ = new_node;        \
    } while (0);

#define for_each_list(list)                           \
    do                                                \
    {                                                 \
        struct Node *temp = (list)->head_node->next_; \
        while (temp)                                  \
        {                                             \
            printf("%d\t", temp->data_);              \
            temp = temp->next_;                       \
        }                                             \
    } while (0);

#define clear_list(list) \
    do                                                \
    {                                                 \
        struct Node *temp = (list)->head_node->next_; \
        while (temp)                                  \
        {                                             \
            temp->head_node->next_=
        }                                             \
    } while (0);

#define forward_list_is_empty(list) \
    ((list)->head_node->next_ ? 0 : 1)
