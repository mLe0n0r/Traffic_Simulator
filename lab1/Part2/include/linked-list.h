#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdio.h>
#include <stdlib.h>

// Definition of the struct of the linked list
typedef struct list {
    int type;
    double time;
    struct list* next;
} list;

typedef struct queue_list {
    double time;
    struct queue_list* next;
} queue_list;

// Function prototypes
list* __remove(list* pointer);
list* __add(list* pointer, int n_type, double n_time);
void __print(list* pointer);

queue_list* __add_queue(queue_list* pointer, double n_time);
queue_list * __remove_queue(queue_list * pointer, double * arrival_time);

#endif
