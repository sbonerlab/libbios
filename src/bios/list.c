#include <stdlib.h>
#include "list.h"

/*
 * Simple doubly linked list implementation
 *
 * Some of the internal functions ("__xxx") are useful when
 * manipulating whole lists rather than single entries, as
 * sometimes we already know the next/prev entries and we can
 * generate better code by using them directly rather than
 * using the generic single-entry routines.
 */
void INIT_LIST_HEAD(struct list_head *list)
{
	list->next = list;
	list->prev = list;
}

/*
 * Insert a new entry between two known consecutive entries.
 *
 * Used for internal list manipulation where we know the prev/next
 * entries already
 */
static void __list_add(struct list_head *new,
		       struct list_head *prev,
		       struct list_head *next)
{
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev->next = new;
}

/*
 * Add anew entry after the specified head.
 * This is good for implementing stacks
 *
 * @param[in] new: new entry to be added
 * @param[in] head: head to add it after
 */
void list_add(struct list_head *new, struct list_head *head)
{
	__list_add(new, head, head->next);
}

/*
 * Add a new entry before the specified head.
 * This is good for implementing queues
 *
 * @param[in] new: new entry to be added
 * @param[in] head: list head to add it before.
 */
void list_add_tail(struct list_head *new, struct list_head *head)
{
	__list_add(new, head->prev, head);
}

/*
 * Delete a list entry by making the prev/next entries
 * point to each other.
 *
 * This is for internal list manipulation for where we know
 * the prev/next entries already.
 */
static void __list_del(struct list_head *prev, struct list_head *next)
{
	next->prev = prev;
	prev->next = next;
}


static void __list_del_entry(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
}

/*
 * Deletes entry from list
 * Note: list_empty() on the entry does not return true
 * after this. The entry is in an undefined state.
 *
 * @param[in] entry: the element to delete from the list
 */
void list_del(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
	entry->next = NULL;
	entry->prev = NULL;
}

/*
 * Replace old entry by a new one.
 * If old was empty, it will be overwritten.
 *
 * @param[in] old: the element to be replaced
 * @param[in] new: the new element to insert
 */
void list_replace(struct list_head *old,
		  struct list_head *new)
{
	new->next = old->next;
	new->next->prev = new;
	new->prev = old->prev;
	new->prev->next = new;
}

void list_replace_init(struct list_head *old,
		       struct list_head *new)
{
	list_replace(old, new);
	INIT_LIST_HEAD(old);
}

/*
 * deletes entry from list and reinitialize it.
 *
 * @param[in] entry: the element to delete from the list.
 */
void list_del_init(struct list_head *entry)
{
	__list_del_entry(entry);
	INIT_LIST_HEAD(entry);
}

/**
 * Delete from one list and add as another's head
 * @param[in] list: the entry to move
 * @param[in] head: the head that will precede our entry
 */
void list_move(struct list_head *list, struct list_head *head)
{
	__list_del_entry(list);
	list_add(list, head);
}

/**
 * list_move_tail - Delete from one list and add as another's tail
 * @param[in] list: the entry to move
 * @param[in] head: the head that will follow our entry
 */
void list_move_tail(struct list_head *list,
		    struct list_head *head)
{
	__list_del_entry(list);
	list_add_tail(list, head);
}

/**
 * list_is_last - Tests whether @list is the last entry in list @head
 * @param[in] list: the entry to test
 * @param[in] head: the head of the list
 */
int list_is_last(const struct list_head *list,
		 const struct list_head *head)
{
	return list->next == head;
}

/**
 * list_empty - Tests whether a list is empty
 * @param[in] head: the list to test.
 */
int list_empty(const struct list_head *head)
{
	return head->next == head;
}

/**
 * Rotate the list to the left
 * @param[in] head: the head of the list
 */
void list_rotate_left(struct list_head *head)
{
	struct list_head *first;

	if (!list_empty(head)) {
		first = head->next;
		list_move_tail(first, head);
	}
}

/**
 * list_is_singular - Tests whether a list has just one entry.
 * @param[in] head: the list to test.
 */
int list_is_singular(const struct list_head *head)
{
	return !list_empty(head) && (head->next == head->prev);
}

static void __list_cut_position(struct list_head *list,
			 struct list_head *head, 
			 struct list_head *entry)
{
	struct list_head *new_first = entry->next;
	list->next = head->next;
	list->next->prev = list;
	list->prev = entry;
	entry->next = list;
	head->next = new_first;
	new_first->prev = head;
}

/**
 * list_cut_position - cut a list into two
 * @param[in] list: a new list to add all removed entries
 * @param[in] head: a list with entries
 * @param[in] entry: an entry within head, could be the head itself
 *	and if so we won't cut the list
 *
 * This helper moves the initial part of @head, up to and
 * including @entry, from @head to @list. You should
 * pass on @entry an element you know is on @head. @list
 * should be an empty list or a list you do not care about
 * losing its data.
 *
 */
void list_cut_position(struct list_head *list,
		       struct list_head *head, 
		       struct list_head *entry)
{
	if (list_empty(head))
		return;
	if (list_is_singular(head) &&
	    (head->next != entry && head != entry))
		return;
	if (entry == head)
		INIT_LIST_HEAD(list);
	else
		__list_cut_position(list, head, entry);
}

static void __list_splice(const struct list_head *list,
			  struct list_head *prev,
			  struct list_head *next)
{
	struct list_head *first = list->next;
	struct list_head *last = list->prev;

	first->prev = prev;
	prev->next = first;

	last->next = next;
	next->prev = last;
}

/**
 * list_splice - Join two lists, this is designed for stacks
 * @param[in] list: the new list to add.
 * @param[in] head: the place to add it in the first list.
 */
void list_splice(const struct list_head *list,
		 struct list_head *head)
{
	if (!list_empty(list))
		__list_splice(list, head, head->next);
}

/**
 * list_splice_tail - Join two lists, each list being a queue
 * @param[in] list: the new list to add.
 * @param[in] head: the place to add it in the first list.
 */
void list_splice_tail(struct list_head *list,
		      struct list_head *head)
{
	if (!list_empty(list))
		__list_splice(list, head->prev, head);
}

/**
 * list_splice_init - join two lists and reinitialise the emptied list.
 * @param[in] list: the new list to add.
 * @param[in] head: the place to add it in the first list.
 *
 * The list at @list is reinitialised
 */
void list_splice_init(struct list_head *list,
		      struct list_head *head)
{
	if (!list_empty(list)) {
		__list_splice(list, head, head->next);
		INIT_LIST_HEAD(list);
	}
}

/**
 * list_splice_tail_init - join two lists and reinitialise the emptied list
 * @param[in] list: the new list to add.
 * @param[in] head: the place to add it in the first list.
 *
 * Each of the lists is a queue.
 * The list at @list is reinitialised
 */
void list_splice_tail_init(struct list_head *list,
			   struct list_head *head)
{
	if (!list_empty(list)) {
		__list_splice(list, head->prev, head);
		INIT_LIST_HEAD(list);
	}
}

