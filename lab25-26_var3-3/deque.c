#include "deque.h"
#include <stdio.h>

void deque_create(deque* d) {
    d->front = d->back = 0;
    d->size = 0;
}

bool deque_is_empty(const deque* d) {
    return d->size == 0;
}

void deque_push_front(deque* d, int value) {
    if (d->size == MAX_DEQUE_SIZE) {
        printf("Ошибка: дек переполнен\n");
        return;
    }
    d->front = (d->front - 1 + MAX_DEQUE_SIZE) % MAX_DEQUE_SIZE;
    d->data[d->front].value = value;
    d->size++;
}

void deque_push_back(deque* d, int value) {
    if (d->size == MAX_DEQUE_SIZE) {
        printf("Ошибка: дек переполнен\n");
        return;
    }
    d->data[d->back].value = value;
    d->back = (d->back + 1) % MAX_DEQUE_SIZE;
    d->size++;
}

void deque_pop_front(deque* d) {
    if (deque_is_empty(d)) {
        printf("Ошибка: дек пуст\n");
        return;
    }
    d->front = (d->front + 1) % MAX_DEQUE_SIZE;
    d->size--;
}

void deque_pop_back(deque* d) {
    if (deque_is_empty(d)) {
        printf("Ошибка: дек пуст\n");
        return;
    }
    d->back = (d->back - 1 + MAX_DEQUE_SIZE) % MAX_DEQUE_SIZE;
    d->size--;
}

void deque_print(const deque* d) {
    if (deque_is_empty(d)) {
        printf("Дек пуст\n");
        return;
    }
    printf("Дек: ");
    int index = d->front;
    for (int i = 0; i < d->size; i++) {
        printf("%d ", d->data[index].value);
        index = (index + 1) % MAX_DEQUE_SIZE;
    }
    printf("\n");
}

int deque_size(const deque* d) {
    return d->size;
}

int deque_front_value(const deque* d) {
    if (deque_is_empty(d)) {
        printf("Ошибка: дек пуст\n");
        exit(EXIT_FAILURE);
    }
    return d->data[d->front].value;
}

int deque_back_value(const deque* d) {
    if (deque_is_empty(d)) {
        printf("Ошибка: дек пуст\n");
        exit(EXIT_FAILURE);
    }
    int index = (d->back - 1 + MAX_DEQUE_SIZE) % MAX_DEQUE_SIZE;
    return d->data[index].value;
}

void deque_concat(deque* dest, deque* src) {
    if (src->size == 0) {
        return;
    }
    if (dest->size + src->size > MAX_DEQUE_SIZE) {
        printf("Ошибка: недостаточно места для конкатенации деков\n");
        exit(EXIT_FAILURE);
    }
    // Копируем элементы из src в dest
    int index = src->front;
    for (int i = 0; i < src->size; i++) {
        int value = src->data[index].value;
        deque_push_back(dest, value);
        index = (index + 1) % MAX_DEQUE_SIZE;
    }
    // Очищаем src
    deque_create(src);
}