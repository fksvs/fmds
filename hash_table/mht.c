#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "mht.h"

static struct mht_node *list_insert(struct mht_node *list, void *key, void *data)
{
	struct mht_node *node = malloc(sizeof(struct mht_node));
	
	node->key = key;
	node->data = data;
	node->next = list->next;
	list->next = node;
	
	return node;
}

static struct mht_node *list_search(struct mht_node *list, void *key)
{
	struct mht_node *ptr = list;

	while (ptr != NULL) {
		if (ptr->key == key) {
			return ptr;
		}
		ptr = ptr->next;
	}

	return NULL;
}

static void list_destroy(struct mht *ht, struct mht_node *list)
{
	struct mht_node *ptr = list;

	while (ptr != NULL) {
		struct mht_node *temp = ptr->next;
		ht->destroy(ptr->key, ptr->data);
		free(ptr);
		ptr = temp;
	}
}

static void list_remove(struct mht *ht, struct mht_node *list, void *key)
{
	struct mht_node *ptr = list;

	while (ptr != NULL) {
		if (ptr->next->key == key) {
			struct mht_node *temp = ptr->next;
			ptr->next = ptr->next->next;
			ht->destroy(temp->key, temp->data);
			free(temp);
			break;
		}

		ptr = ptr->next;
	}
}

struct mht *mht_init(int (*h)(void *key, size_t key_size, size_t table_size),
		void (*destroy)(void *key, void *data),
		size_t table_size)
{
	struct mht *ht = malloc(sizeof(struct mht));
	
	ht->h = h;
	ht->destroy = destroy;
	ht->table_size = table_size;
	ht->table = malloc(sizeof(struct mht_node) * table_size);
	memset(ht->table, 0, sizeof(struct mht_node) * table_size);
	
	return ht;
}

struct mht_node *mht_insert(struct mht *ht, void *key, size_t key_size, void *data)
{
	int ind = ht->h(key, key_size, ht->table_size);

	if (!ht->table[ind].key && !ht->table[ind].data){
		ht->table[ind].key = key;
		ht->table[ind].data = data;
		ht->table[ind].next = NULL;
		return &ht->table[ind];
	} else {
		return list_insert(&ht->table[ind], key, data);
	}
}


struct mht_node *mht_lookup(struct mht *ht, void *key, size_t key_size)
{
	int ind = ht->h(key, key_size, ht->table_size);

	if (ht->table[ind].key == key) {
		return &ht->table[ind];
	} else {
		return list_search(&ht->table[ind], key);
	}
}

void mht_destroy(struct mht *ht)
{
	for (int ind = 0; ind < ht->table_size; ind++) {
		if (ht->table[ind].next != NULL) {
			list_destroy(ht, ht->table[ind].next);
		}
		ht->destroy(ht->table[ind].key, ht->table[ind].data);
	}

	free(ht->table);
	free(ht);
}

void mht_remove(struct mht *ht, void *key, size_t key_size)
{
	int ind = ht->h(key, key_size, ht->table_size);

	if (ht->table[ind].key == key) {
		ht->destroy(ht->table[ind].key, ht->table[ind].data);
		ht->table[ind].key = NULL;
		ht->table[ind].data = NULL;
		if (ht->table[ind].next != NULL) {
			struct mht_node *temp = ht->table[ind].next;
			ht->table[ind].key = ht->table[ind].next->key;
			ht->table[ind].data = ht->table[ind].next->data;
			ht->table[ind].next = ht->table[ind].next->next;
			free(temp);
		}
	} else if (ht->table[ind].next != NULL){
		list_remove(ht, &ht->table[ind], key);
	}
}
