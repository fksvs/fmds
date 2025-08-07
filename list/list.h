#ifndef LIST
#define LIST

/* data types for list */

struct node_t {
	void *data;
	struct node_t *prev;
	struct node_t *next;
};

typedef struct {
	unsigned int list_size;
	void (*destroy)(void *data);
	struct node_t *head;
	struct node_t *tail;
} list_t;

typedef struct {
	struct node_t *cur;
} iter_t;

/* list interface */

#define LIST_EMPTY(list) ((list)->head == NULL || (list)->tail == NULL)
#define LIST_HEAD(list) ((list)->head)
#define LIST_TAIL(list) ((list)->tail)
#define LIST_SIZE(list) ((list)->list_size)

list_t *list_init(void (*destroy)(void *data));

struct node_t *list_insert_before(list_t *l, struct node_t *p, void *data);
struct node_t *list_insert_after(list_t *l, struct node_t *p, void *data);
struct node_t *list_insert_head(list_t *l, void *data);
struct node_t *list_insert_tail(list_t *l, void *data);

void list_destroy(list_t *l);
void list_remove_head(list_t *l);
void list_remove_tail(list_t *l);
void list_remove_data(list_t *l, struct node_t *p);

struct node_t *list_search_data(list_t *l, void *data);

iter_t *iter_init(list_t *l);
void iter_destroy(iter_t *i);
struct node_t *list_prev(iter_t *i);
struct node_t *list_next(iter_t *i);

/* stack interface */

#define STACK_EMPTY(list) ((list)->head == NULL || (list)->tail == NULL)
#define STACK_HEAD(list) ((list)->head)
#define STACK_TAIL(list) ((list)->tail)
#define STACK_SIZE(list) ((list)->list_size)

list_t *stack_init(void (*destroy)(void *data));
void stack_destroy(list_t *l);
struct node_t *stack_push(list_t *l, void *data);
void *stack_pop(list_t *l);
void *stack_peek(list_t *l);

/* queue interface */

#define QUEUE_EMPTY(list) ((list)->head == NULL || (list)->tail == NULL)
#define QUEUE_HEAD(list) ((list)->head)
#define QUEUE_TAIL(list) ((list)->tail)
#define QUEUE_SIZE(list) ((list)->list_size)

list_t *queue_init(void (*destroy)(void *data));
void queue_destroy(list_t *l);
struct node_t *queue_enqueue(list_t *l, void *data);
void *queue_dequeue(list_t *l);
void *queue_peek(list_t *l);

#endif
