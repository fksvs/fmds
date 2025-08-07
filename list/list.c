#include <stdlib.h>
#include "list.h"

list_t *list_init(void (*destroy)(void *data))
{
	list_t *l = malloc(sizeof(list_t));
	if (!l) {
		return NULL;
	}
	l->list_size = 0;
	l->destroy = destroy;
	l->head = NULL;
	l->tail = NULL;
	return l;
}

static struct node_t *init_list_data(void *data)
{
	struct node_t *node = malloc(sizeof(struct node_t));
	node->data = data;
	node->prev = NULL;
	node->next = NULL;
	return node;
}

struct node_t *list_insert_before(list_t *l, struct node_t *p, void *data)
{
	struct node_t *node = init_list_data(data);

	if (LIST_EMPTY(l)) {
		l->head = node;
		l->tail = node;
	} else if (p == l->head){
		node->next = p;
		p->prev = node;
		l->head = node;
	} else {
		node->prev = p->prev;
		node->next = p;
		p->prev->next = node;
		p->prev = node;
	}

	l->list_size++;
	return node;
}

struct node_t *list_insert_after(list_t *l, struct node_t *p, void *data)
{
	struct node_t *node = init_list_data(data);

	if (LIST_EMPTY(l)) {
		l->head = node;
		l->tail = node;
	} else if (p == l->tail) {
		node->prev = p;
		p->next = node;
		l->tail = node;
	} else {
		node->prev = p;
		node->next = p->next;
		p->next->prev = node;
		p->next = node;
	}

	l->list_size++;
	return node;
}

struct node_t *list_insert_head(list_t *l, void *data)
{
	return list_insert_before(l, l->head, data);
}

struct node_t *list_insert_tail(list_t *l, void *data)
{
	return list_insert_after(l, l->tail, data);
}

static void remove_data(list_t *l, struct node_t *p)
{
	l->list_size--;
	l->destroy(p->data);
	free(p);
}

void list_destroy(list_t *l)
{
        struct node_t *node = l->head;

        while (node != NULL) {
                struct node_t *temp = node->next;
                remove_data(l, node);
		node = temp;
        }
        free(l);
}

void list_remove_head(list_t *l)
{
	if (l->head != NULL) {
		struct node_t *temp = l->head;
		if (l->head->next == NULL) {
			l->head = NULL;
			l->tail = NULL;
		} else {
			l->head = l->head->next;
			l->head->prev = NULL;
		}
		remove_data(l, temp);
	}
}

void list_remove_tail(list_t *l)
{
	if (l->tail != NULL) {
		struct node_t *temp = l->tail;
		if (l->tail->prev == NULL) {
			l->head = NULL;
			l->tail = NULL;
		} else {
			l->tail = l->tail->prev;
			l->tail->next = NULL;
		}
		remove_data(l, temp);
	}
}

void list_remove_data(list_t *l, struct node_t *p)
{
	if (l->head == p) {
		list_remove_head(l);
		return;
	} else if (l->tail == p) {
		list_remove_tail(l);
		return;
	} else if (p->prev != NULL && p->next != NULL) {
                p->prev->next = p->next;
                p->next->prev = p->prev;
		remove_data(l, p);
	}
}

struct node_t *list_search_data(list_t *l, void *data)
{
        struct node_t *node = l->head;

        while (node != NULL) {
		if (node->data == data) {
			return node;
		}
                node = node->next;
	}

	return NULL;
}

iter_t *iter_init(list_t *l)
{
	iter_t *i = malloc(sizeof(iter_t));
	i->cur = l->head;
	return i;
}

void iter_destroy(iter_t *i)
{
	free(i);
}

struct node_t *list_prev(iter_t *i)
{
	if (i->cur != NULL) {
		struct node_t *temp = i->cur;
		i->cur = i->cur->prev;
		return temp;
	} else {
		return NULL;
	}
}

struct node_t *list_next(iter_t *i)
{
	if (i->cur != NULL) {
		struct node_t *temp = i->cur;
		i->cur = i->cur->next;
		return temp;
	} else {
		return NULL;
	}
}

list_t *stack_init(void (*destroy)(void *data))
{
	return list_init(destroy);
}

void stack_destroy(list_t *l)
{
	list_destroy(l);
}

struct node_t *stack_push(list_t *l, void *data)
{
	list_insert_head(l, data);
}

void *stack_pop(list_t *l)
{
        if (l->head != NULL) {
                struct node_t *node_temp = l->head;
		void *data_temp = l->head->data;
                if (l->head->next == NULL) {
                        l->head = NULL;
                } else {
                        l->head = l->head->next;
                        l->head->prev = NULL;
                }
		free(node_temp);
                return data_temp;
        }
	return NULL;
}

void *stack_peek(list_t *l)
{
	return l->head->data;
}

list_t *queue_init(void (*destroy)(void *data))
{
	return list_init(destroy);
}

void queue_destroy(list_t *l)
{
	list_destroy(l);
}

struct node_t *queue_enqueue(list_t *l, void *data)
{
	return list_insert_tail(l, data);
}

void *queue_dequeue(list_t *l)
{
        if (l->head != NULL) {
                struct node_t *node_temp = l->head;
                void *data_temp = l->head->data;
		if (l->head->next == NULL) {
                        l->head = NULL;
                } else {
                        l->head = l->head->next;
                        l->head->prev = NULL;
                }
		free(node_temp);
                return data_temp;
        }
        return NULL;
}

void *queue_peek(list_t *l)
{
	return l->head->data;
}
