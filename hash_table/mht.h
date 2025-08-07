#ifndef MHT
#define MHT

#include <stddef.h>

struct mht_node {
	void *key;
	void *data;
	struct mht_node *next;
};

struct mht {
	int (*h)(void *key, size_t key_size, size_t table_size);
	void (*destroy)(void *key, void *data);
	size_t table_size;
	struct mht_node *table;
};

#define TABLE_EMPTY(ht) ((ht)->table == NULL)
#define TABLE_SIZE(ht) ((ht)->table_size)

struct mht *mht_init(int (*h)(void *key, size_t key_size, size_t table_size),
		void (*destroy)(void *key, void *data),
		size_t table_size);
struct mht_node *mht_insert(struct mht *ht, void *key, size_t key_size, void *data);
struct mht_node *mht_lookup(struct mht *ht, void *key, size_t key_size);
void mht_destroy(struct mht *ht);
void mht_remove(struct mht *ht, void *key, size_t key_size);

#endif
