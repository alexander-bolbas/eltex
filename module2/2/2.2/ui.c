#include <stdio.h>
#include "ops.h"
#include "ui.h"

// Вспомогательная функция для чтения двух аргументов
static void read_args(double *a, double *b) {
    printf("Введите два числа: ");
    scanf("%lf %lf", a, b);
    while (getchar() != '\n'); // Очистка буфера ввода
}

// Основной интерфейс калькулятора - отображает меню и обрабатывает выбор операции
void ui_run_calculator(void) {
    double a, b, result; // Операнды и результат
    int choice;          // Выбор пользователя

    while (1) {
        printf("\n1. Сложение    2. Вычитание   3. Умножение\n");
        printf("4. Деление     5. Возведение  6. Корень\n");
        printf("0. Выход\n> ");
        scanf("%d", &choice);
        while (getchar() != '\n');

        // Выход из программы
        if (choice == 0) break;

        // Читаем аргументы
        read_args(&a, &b);

        // Выполняем операцию в зависимости от выбора
        int status = 0;
        switch (choice) {
            case 1: status = op_add(a, b, &result);  break;
            case 2: status = op_sub(a, b, &result);  break;
            case 3: status = op_mul(a, b, &result);  break;
            case 4: status = op_div(a, b, &result);  break;
            case 5: status = op_pow(a, b, &result);  break;
            case 6: status = op_sqrt(a, b, &result); break;
            default: printf("Неверный выбор\n"); continue;
        }

        // Выводим результат или ошибку
        if (status == 0)
            printf("= %.6g\n", result);
        else
            printf("Ошибка при выполнении операции\n");
    }
}