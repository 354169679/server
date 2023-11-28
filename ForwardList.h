#pragma once

#include <exception>
#include <iostream>

namespace x
{
    template <typename T>
    struct Node
    {
        T data_;
        Node *next_;
    };

    template <typename T>
    class ForwardList
    {
    private:
        Node<T> *head_node_ = nullptr;

    public:
        ForwardList() = default;
        ForwardList(const ForwardList &);
        void insert_front(const T &);
        inline void clear();
        inline bool empty();
    };

    /* fix bug */
    template <typename T>
    ForwardList<T>::ForwardList(const ForwardList &list)
    {
        try
        {
            Node<T> *temp = list.head_node_;
            while (temp)
            {
                Node<T> *node = new Node<T>;
                node->data_ = temp->data_;
                node->next_ = temp;
                head_node_ = node;
                temp = temp->next_;
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
    }

    template <typename T>
    void ForwardList<T>::insert_front(const T &val)
    {
        try
        {
            Node<T> *node = new Node<T>;
            node->data_ = val;
            node->next_ = head_node_;
            head_node_ = node;
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
    }

    template <typename T>
    void ForwardList<T>::clear()
    {
        while (head_node_)
        {
            Node<T> *temp = head_node_->next_;
            delete head_node_;
            head_node_ = temp;
        }
        head_node_ = nullptr;
    }

    template <typename T>
    bool ForwardList<T>::empty()
    {
        return head_node_ == nullptr ? true : false;
    }
}
