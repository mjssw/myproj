#ifndef _LINUX_LIST_H_
#define _LINUX_LIST_H_

struct list_head 
{
    struct list_head *next, *prev;
};

#define LIST_HEAD_INIT(name) { &(name), &(name) }
#define LIST_HEAD(name) struct list_head name = LIST_HEAD_INIT(name)

static void INIT_LIST_HEAD(struct list_head *list)
{
    list->next = list;
    list->prev = list;
}

static void __list_add(struct list_head *_new, struct list_head *prev, struct list_head *next)
{
    next->prev = _new;
    _new->next = next;
    _new->prev = prev;
    prev->next = _new;
}

static void list_add(struct list_head *_new, struct list_head *head)
{
    __list_add(_new, head, head->next);
}

static void list_add_tail(struct list_head *_new, struct list_head *head)
{
    __list_add(_new, head->prev, head);
}

static void __list_add_rcu(struct list_head *_new, struct list_head *prev, struct list_head *next)
{
    _new->next = next;
    _new->prev = prev;
    next->prev = _new;
    prev->next = _new;
}

static void list_add_rcu(struct list_head *_new, struct list_head *head)
{
    __list_add_rcu(_new, head, head->next);
}

static void list_add_tail_rcu(struct list_head *_new, struct list_head *head)
{
    __list_add_rcu(_new, head->prev, head);
}

static void __list_del(struct list_head *prev, struct list_head *next)
{
    next->prev = prev;
    prev->next = next;
}

static void list_del_init(struct list_head *entry)
{
    __list_del(entry->prev, entry->next);
    INIT_LIST_HEAD(entry);
}

static int list_is_last(struct list_head *list, struct list_head *head)
{
    return list->next == head;
}

static int list_empty(struct list_head *head)
{
    return head->next == head;
}

static int list_empty_careful(struct list_head *head)
{
    struct list_head *next = head->next;
    return (next == head) && (next == head->prev);
}

#define list_entry(ptr, type, member) ((type*)((char*)(ptr)-(unsigned long)(&((type*)0)->member)))

#define list_for_each(pos, head) \
    for(pos=(head)->next; pos!=(head); pos=pos->next)
                  
                  


#endif
