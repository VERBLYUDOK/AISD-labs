#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "deque.h"
#include "qsort.h"


int main() {
    deque d;
    deque_create(&d);
    int choice;
    int value;

    // Инициализация генератора случайных чисел для выбора опорного элемента
    srand(time(NULL));

    do {
        printf("\nМеню:\n");
        printf("1. Добавить элемент в начало дека\n");
        printf("2. Добавить элемент в конец дека\n");
        printf("3. Удалить элемент из начала дека\n");
        printf("4. Удалить элемент из конца дека\n");
        printf("5. Вывести дек\n");
        printf("6. Размер дека\n");
        printf("7. Сортировать дек\n");
        printf("8. Очистить дек\n");
        printf("0. Выход\n");
        printf("Выберите действие: ");

        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Введите значение: ");
                scanf("%d", &value);
                deque_push_front(&d, value);
                printf("Элемент %d добавлен в начало дека.\n", value);
                break;
            case 2:
                printf("Введите значение: ");
                scanf("%d", &value);
                deque_push_back(&d, value);
                printf("Элемент %d добавлен в конец дека.\n", value);
                break;
            case 3:
                if (!deque_is_empty(&d)) {
                    value = deque_front_value(&d);
                    deque_pop_front(&d);
                    printf("Элемент %d удален из начала дека.\n", value);
                } else {
                    printf("Дек пуст.\n");
                }
                break;
            case 4:
                if (!deque_is_empty(&d)) {
                    value = deque_back_value(&d);
                    deque_pop_back(&d);
                    printf("Элемент %d удален из конца дека.\n", value);
                } else {
                    printf("Дек пуст.\n");
                }
                break;
            case 5:
                deque_print(&d);
                break;
            case 6:
                printf("Размер дека: %d\n", deque_size(&d));
                break;
            case 7:
                quicksort_deque(&d);
                printf("Дек отсортирован.\n");
                break;
            case 8:
                deque_create(&d);
                printf("Дек очищен.\n");
                break;
            case 0:
                printf("Выход из программы...\n");
                break;
            default:
                printf("Некорректный ввод.\n");
                break;
        }
    } while (choice != 0);

    return 0;
}
