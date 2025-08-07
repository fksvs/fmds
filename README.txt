FMDS - Minimal Data Structure Implementations in C
===================================================

A collection of fundamental data structures implemented in C, providing 
efficient and lightweight implementations of hash tables and linked lists.

Table of Contents
-----------------
- Overview
- Installation  
- Hash Table
- Linked List
- Building and Usage
- License

Overview
--------

I actually don't remember when I wrote this project and what I included. I saw
it in a directory I forgot about. Of course I did check what I did with curiosity. :D

This project provides two main data structure implementations:

1. Hash Table (MHT) - A hash table implementation using separate chaining 
   for collision resolution
2. Linked List - A doubly-linked circular list with stack and queue interfaces

Both data structures are designed to be generic and work with any data type 
through void pointers.

Installation
------------

1. Clone the repository:
   
   git clone <repository-url>
   cd fmds

2. Include the header files in your project:
   
   #include "hash_table/mht.h"     // For hash table
   #include "list/list.h"          // For linked list

3. Compile the source files with your project:
   
   gcc your_program.c hash_table/mht.c list/list.c -o your_program

Hash Table
----------

The hash table implementation (MHT - Minimal Hash Table) uses separate chaining 
to handle collisions and supports generic key-value pairs.

Features:
- Generic key-value storage using void pointers
- Separate chaining for collision resolution
- Customizable hash function
- Custom destroy function for memory management

Basic Usage:

Initialization:
    #include "hash_table/mht.h"

    // Define a hash function
    int my_hash_function(void *key, size_t key_size, size_t table_size) {
        // Simple hash for string keys
        char *str_key = (char *)key;
        unsigned long hash = 5381;
        int c;
        while ((c = *str_key++))
            hash = ((hash << 5) + hash) + c;
        return hash % table_size;
    }

    // Define a destroy function (optional - can be NULL)
    void my_destroy_function(void *key, void *data) {
        free(key);
        free(data);
    }

    // Create hash table with 100 buckets
    struct mht *ht = mht_init(my_hash_function, my_destroy_function, 100);

Inserting Data:
    char *key = strdup("example_key");
    int *value = malloc(sizeof(int));
    *value = 42;

    struct mht_node *node = mht_insert(ht, key, strlen(key), value);
    if (node == NULL) {
        printf("Failed to insert data\n");
    }

Retrieving Data:
    char *search_key = "example_key";
    struct mht_node *found = mht_lookup(ht, search_key, strlen(search_key));

    if (found != NULL) {
        int *retrieved_value = (int *)found->data;
        printf("Found value: %d\n", *retrieved_value);
    } else {
        printf("Key not found\n");
    }

Removing Data:
    char *key_to_remove = "example_key";
    mht_remove(ht, key_to_remove, strlen(key_to_remove));

Cleanup:
    mht_destroy(ht);  // Calls destroy function for all remaining elements

Hash Table API Reference:

Function             Description
--------             -----------
mht_init()          Initialize a new hash table
mht_insert()        Insert a key-value pair
mht_lookup()        Search for a key and return the node
mht_remove()        Remove a key-value pair
mht_destroy()       Clean up and free the hash table

Macros:
- TABLE_EMPTY(ht) - Check if hash table is empty
- TABLE_SIZE(ht) - Get the table size

Linked List
-----------

The linked list implementation provides a doubly-linked circular list with 
additional stack and queue interfaces.

Features:
- Doubly-linked circular structure
- Generic data storage using void pointers
- Stack interface (LIFO)
- Queue interface (FIFO)
- Iterator support for traversal
- Custom destroy function for memory management

Basic Usage:

List Operations:

Initialization:
    #include "list/list.h"

    // Define a destroy function (optional - can be NULL)
    void my_destroy(void *data) {
        free(data);
    }

    // Create a new list
    list_t *list = list_init(my_destroy);

Inserting Data:
    // Insert at head
    int *data1 = malloc(sizeof(int));
    *data1 = 10;
    struct node_t *node1 = list_insert_head(list, data1);

    // Insert at tail
    int *data2 = malloc(sizeof(int));
    *data2 = 20;
    struct node_t *node2 = list_insert_tail(list, data2);

    // Insert before/after a specific node
    int *data3 = malloc(sizeof(int));
    *data3 = 15;
    struct node_t *node3 = list_insert_after(list, node1, data3);

Searching and Removing:
    // Search for data
    int search_value = 20;
    struct node_t *found = list_search_data(list, &search_value);

    // Remove specific elements
    list_remove_head(list);
    list_remove_tail(list);
    list_remove_data(list, found);  // Remove specific node

List Traversal with Iterator:
    // Create iterator
    iter_t *iter = iter_init(list);

    // Traverse forward
    struct node_t *current = list_next(iter);
    while (current != NULL) {
        int *value = (int *)current->data;
        printf("Value: %d\n", *value);
        current = list_next(iter);
    }

    // Cleanup iterator
    iter_destroy(iter);

Cleanup:
    list_destroy(list);

Stack Interface (LIFO):
    // Initialize stack (same as list)
    list_t *stack = stack_init(my_destroy);

    // Push elements
    int *data1 = malloc(sizeof(int));
    *data1 = 10;
    stack_push(stack, data1);

    int *data2 = malloc(sizeof(int));
    *data2 = 20;
    stack_push(stack, data2);

    // Peek at top element
    int *top = (int *)stack_peek(stack);
    printf("Top element: %d\n", *top);

    // Pop elements
    int *popped = (int *)stack_pop(stack);
    printf("Popped: %d\n", *popped);
    free(popped);

    // Cleanup
    stack_destroy(stack);

Queue Interface (FIFO):
    // Initialize queue (same as list)
    list_t *queue = queue_init(my_destroy);

    // Enqueue elements
    int *data1 = malloc(sizeof(int));
    *data1 = 10;
    queue_enqueue(queue, data1);

    int *data2 = malloc(sizeof(int));
    *data2 = 20;
    queue_enqueue(queue, data2);

    // Peek at front element
    int *front = (int *)queue_peek(queue);
    printf("Front element: %d\n", *front);

    // Dequeue elements
    int *dequeued = (int *)queue_dequeue(queue);
    printf("Dequeued: %d\n", *dequeued);
    free(dequeued);

    // Cleanup
    queue_destroy(queue);

List API Reference:

List Functions:
Function                Description
--------                -----------
list_init()            Initialize a new list
list_insert_head()     Insert at the beginning
list_insert_tail()     Insert at the end
list_insert_before()   Insert before a node
list_insert_after()    Insert after a node
list_remove_head()     Remove first element
list_remove_tail()     Remove last element
list_remove_data()     Remove specific node
list_search_data()     Search for data
list_destroy()         Clean up the list

Stack Functions:
Function           Description
--------           -----------
stack_init()      Initialize stack
stack_push()      Push element onto stack
stack_pop()       Pop element from stack
stack_peek()      Peek at top element
stack_destroy()   Clean up stack

Queue Functions:
Function           Description
--------           -----------
queue_init()      Initialize queue
queue_enqueue()   Add element to queue
queue_dequeue()   Remove element from queue
queue_peek()      Peek at front element
queue_destroy()   Clean up queue

Macros:
- LIST_EMPTY(list) - Check if list is empty
- LIST_SIZE(list) - Get list size
- LIST_HEAD(list) - Get head node
- LIST_TAIL(list) - Get tail node
- Similar macros available for STACK_* and QUEUE_*

Building and Usage
------------------

Simple Example Program:
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "hash_table/mht.h"
    #include "list/list.h"

    int main() {
        // Hash table example
        struct mht *ht = mht_init(my_hash_function, NULL, 10);
        
        char *key = "test";
        int value = 100;
        mht_insert(ht, key, strlen(key), &value);
        
        struct mht_node *node = mht_lookup(ht, key, strlen(key));
        if (node) {
            printf("Hash table value: %d\n", *(int *)node->data);
        }
        
        mht_destroy(ht);
        
        // List example
        list_t *list = list_init(NULL);
        
        int data = 42;
        list_insert_head(list, &data);
        
        printf("List size: %d\n", LIST_SIZE(list));
        
        list_destroy(list);
        
        return 0;
    }

Compilation:
    gcc -o example example.c hash_table/mht.c list/list.c

License
-------

See LICENSE file for details.
