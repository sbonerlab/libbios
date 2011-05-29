/**
 * \file rbmap.h
 * \author Created by David Z. Chen (david.chen@yale.edu)
 */

#ifndef RBMAP_H
#define RBMAP_H

#include "rbtree.h"

struct rbmap {
	struct rb_root tree;
};


struct rbmap_entry {
	char *key;
	char *val;
	struct rb_node tree_node;
};

struct rbmap *rbmap_new(void);

struct rbmap_entry *rbmap_entry_new(char *key, char *val);
void rbmap_entry_free(struct rbmap_entry *entry);

struct rbmap_entry *rbmap_find(struct rbmap *map, char *key);
char *rbmap_get(struct rbmap *map, char *key);
struct rbmap_entry *rbmap_insert(struct rbmap *map, 
				 char *key, 
				 struct rbmap_entry *new);
char *rbmap_set(struct rbmap *map, char *key, char *val);
void rbmap_delete(struct rbmap *map, char *key);
void rbmap_destroy(struct rbmap *map);

#endif
