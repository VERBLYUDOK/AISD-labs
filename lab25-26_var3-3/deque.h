#ifndef DEQUE_H
#define DEQUE_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define MAX_DEQUE_SIZE 256

typedef struct item {
    int value;
} item;

typedef struct {
    int front; // Индекс начала дека
    int back;  // Индекс конца дека
    item data[MAX_DEQUE_SIZE];
    int size;
} deque;

void deque_create(deque* d);
bool deque_is_empty(const deque* d);
void deque_push_front(deque* d, int value);
void deque_push_back(deque* d, int value);
void deque_pop_front(deque* d);
void deque_pop_back(deque* d);
void deque_print(const deque* d);
int deque_size(const deque* d);
int deque_front_value(const deque* d);
int deque_back_value(const deque* d);
void deque_concat(deque* dest, deque* src);

#endif