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

#define init_list(list)                                             \
    (list)->head_node = (struct Node *)malloc(sizeof(struct Node)); \
    (list)->head_node->next_ = NULL

void insert_element(struct ForwardList *list, const int val)
{
    struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));
    assert(new_node);
    new_node->data_ = val;
    new_node->next_ = list->head_node->next_;
    list->head_node->next_ = new_node;
}

void for_each_list(struct ForwardList *list)
{
    struct Node *temp = list->head_node->next_;
    while (temp)
    {
        printf("%d\t", temp->data_);
        temp = temp->next_;
    }
}

#define forward_list_is_empty(list) \
    ((list)->head_node->next_ ? 0 : 1)
