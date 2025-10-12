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

// Function prototypes
list* __remove(list* pointer);
list* __add(list* pointer, int n_type, double n_time);
void __print(list* pointer);

#endif
