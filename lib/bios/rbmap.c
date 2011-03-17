
#include <stdlib.h>
#include <string.h>
#include "rbmap.h"

struct rbmap *rbmap_new()
{
	struct rbmap *new = (struct rbmap *) malloc(sizeof(*new));

	new->tree.rb_node = NULL;
	return new;
}

struct rbmap_entry *rbmap_entry_new(char *key, char *val)
{
	struct rbmap_entry *new;

	new = (struct rbmap_entry *) malloc(sizeof(*new));

	new->key = (char *) malloc((strlen(key) + 1) * sizeof(char));
	new->val = (char *) malloc((strlen(val) + 1) * sizeof(char));
	strcpy(new->key, key);
	strcpy(new->val, val);

	return new;
}

void rbmap_entry_free(struct rbmap_entry *entry)
{
	free(entry->key);
	free(entry->val);
	free(entry);
}

struct rbmap_entry *rbmap_find(struct rbmap *map, char *key)
{
	struct rb_node *n = map->tree.rb_node;
	struct rbmap_entry *entry = NULL;
	int cmp;

	while (n) {
		entry = rb_entry(n, struct rbmap_entry, tree_node);

		if ((cmp = strcmp(key, entry->key)) > 0)
			n = n->rb_right;
		else if (cmp < 0)
			n = n->rb_left;
		else
			return entry;
	}
	return NULL;
}

char *rbmap_get(struct rbmap *map, char *key)
{
	struct rbmap_entry *entry = rbmap_find(map, key);

	return (entry) ? (entry->val) : NULL;
}

struct rbmap_entry *rbmap_insert(struct rbmap *map, 
				 char *key, 
				 struct rbmap_entry *new)
{
	struct rb_node **p = &map->tree.rb_node;
	struct rb_node *parent = NULL;
	struct rbmap_entry *entry = NULL;
	int cmp;

	while (*p) {
		parent = *p;
		entry = rb_entry(parent, struct rbmap_entry, tree_node);

		if ((cmp = strcmp(key, entry->key)) > 0)
			p = &(*p)->rb_right;
		else if (cmp < 0)
			p = &(*p)->rb_left;
		else
			return entry;
	}

	rb_link_node(&new->tree_node, parent, p);

	return NULL;
}

char *rbmap_set(struct rbmap *map, char *key, char *val)
{
	struct rbmap_entry *ret;
	struct rbmap_entry *new;

	new = rbmap_entry_new(key, val);

	if ((ret = rbmap_insert(map, key, new))) {
		rb_replace_node(&ret->tree_node, &new->tree_node, &map->tree);
		rbmap_entry_free(ret);
		goto out;
	}
	rb_insert_color(&new->tree_node, &map->tree);

out:
	return new->val;
}

void rbmap_delete(struct rbmap *map, char *key)
{
	struct rbmap_entry *entry;

	entry = rbmap_find(map, key);

	if (entry) {
		rb_erase(&entry->tree_node, &map->tree);
		rbmap_entry_free(entry);
	}
}

static void rbmap_recur_destroy(struct rb_node *node)
{
	struct rbmap_entry *entry;

	if (node->rb_left)
		rbmap_recur_destroy(node->rb_left);
	if (node->rb_right)
		rbmap_recur_destroy(node->rb_right);

	entry = rb_entry(node, struct rbmap_entry, tree_node);
	rbmap_entry_free(entry);
}

void rbmap_destroy(struct rbmap *map)
{
	if (map->tree.rb_node)
		rbmap_recur_destroy(map->tree.rb_node);
}	
