#include "qsort.h"

void quicksort_deque(deque* d) {
    if (d->size <= 1) {
        return; // База
    }

    // Выбираем опорный элемент
    int pivot_index = rand() % d->size;
    int index = (d->front + pivot_index) % MAX_DEQUE_SIZE;
    int pivot = d->data[index].value;

    // Создаем 3 дека
    deque less, equal, greater;
    deque_create(&less);
    deque_create(&equal);
    deque_create(&greater);

    // Разделяем элементы исходного дека на 3 дека
    while (!deque_is_empty(d)) {
        int value = deque_front_value(d);
        deque_pop_front(d);

        if (value < pivot) {
            deque_push_back(&less, value);
        } else if (value == pivot) {
            deque_push_back(&equal, value);
        } else {
            deque_push_back(&greater, value);
        }
    }

    // Рекурсивно сортируем
    quicksort_deque(&less);
    quicksort_deque(&greater);

    // Конкатенируем деки
    deque_concat(d, &less);
    deque_concat(d, &equal);
    deque_concat(d, &greater);
}
