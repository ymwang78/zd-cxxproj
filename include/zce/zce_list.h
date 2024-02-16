#pragma once
// ***************************************************************
//  zce_list   version:  1.0   -  date: 2002/11/14
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This file is a part of ZCE, which inherited from ubeda/utiny.
//  Copyright (C) 2002 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#ifndef __zce_list_h__
#define __zce_list_h__

struct zce_list_node
{
    struct zce_list_node *next, *prev;
};

#define ZCE_LIST_INIT_HEAD(ptr) do { \
    (ptr)->next = (ptr); (ptr)->prev = (ptr); \
    } while (0)

#define INIT_LIST_HEAD(ptr) ZCE_LIST_INIT_HEAD(ptr)

static inline void __list_add(struct zce_list_node *item, struct zce_list_node *prev, struct zce_list_node *next)
{
    next->prev = item;
    item->next = next;
    item->prev = prev;
    prev->next = item;
};

static inline void zce_list_add(struct zce_list_node *item, struct zce_list_node *head)
{
    __list_add(item, head, head->next);
};

static inline void zce_list_add_tail(struct zce_list_node* item, struct zce_list_node* head)
{
	__list_add(item, head->prev, head);
};

static inline bool zce_list_empty(const struct zce_list_node *head)
{
    return head->next == head;
};

static inline void __list_del(struct zce_list_node * prev, struct zce_list_node * next)
{
    next->prev = prev;
    prev->next = next;
};

static inline void zce_list_del(struct zce_list_node *entry)
{
    __list_del(entry->prev, entry->next);
};

inline zce_list_node* zce_list_del_head(zce_list_node* head)
{
    zce_list_node* p = head->next;
    zce_list_del(p);
    return p;
}

inline zce_list_node* zce_list_del_tail(zce_list_node* head)
{
    zce_list_node* p = head->prev;
    zce_list_del(p);
    return p;
}

inline void __list_splice(const zce_list_node* list, zce_list_node* prev, zce_list_node* next)
{
    zce_list_node* first = list->next;
    zce_list_node* last = list->prev;
    first->prev = prev;
    prev->next = first;
    last->next = next;
    next->prev = last;
}

inline void zce_list_splice(const zce_list_node* list, zce_list_node* head)
{
    if (!zce_list_empty(list))
        __list_splice(list, head, head->next);
}

inline void zce_list_splice_tail(zce_list_node* list, zce_list_node* head)
{
    if (!zce_list_empty(list))
        __list_splice(list, head->prev, head);
}

inline void zce_list_splice_init(zce_list_node* list, zce_list_node* head)
{
    if (!zce_list_empty(list)) {
        __list_splice(list, head, head->next);
        INIT_LIST_HEAD(list);
    }
}

inline void zce_list_splice_tail_init(zce_list_node* list, zce_list_node* head)
{
    if (!zce_list_empty(list)) {
        __list_splice(list, head->prev, head);
        INIT_LIST_HEAD(list);
    }
}

#define zce_container_of(ptr, type, member) \
  ((type *) ((char *) (ptr) - offsetof(type, member)))

#define zce_list_entry(ptr, type, member) \
    zce_container_of(ptr, type, member)

#define zce_list_first_entry(ptr, type, member) \
    zce_list_entry((ptr)->next, type, member)

#define zce_list_last_entry(ptr, type, member) \
    zce_list_entry((ptr)->prev, type, member)

#define zce_list_next_entry(pos, member) \
    zce_list_entry((pos)->member.next, typeof(*(pos)), member)

#define zce_list_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); pos = pos->next)

#define zce_list_for_each_safe(pos, n, head) \
    for (pos = (head)->next, n = pos->next; pos != (head); \
        pos = n, n = pos->next)

#define zce_list_for_each_prev(pos, head) \
    for (pos = (head)->prev; pos != (head); pos = pos->prev)

#define zce_list_for_each_prev_safe(pos, n, head) \
    for (pos = (head)->prev, n = pos->prev; \
         pos != (head); \
         pos = n, n = pos->prev)

#endif // __zce_list_h__
