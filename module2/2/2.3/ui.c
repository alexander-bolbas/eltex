#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "cmd_registry.h"
#include "ops.h"

// Главный цикл калькулятора - читает команду и ее аргументы, вызывает нужную функцию
void ui_run_calculator(const cmd_registry_t *registry) {
    char input[256];         // Буфер для строки ввода
    double argv[MAX_ARGS];   // Массив аргументов

    while (1) {
        // Показываем меню доступных команд
        registry_print_menu(registry);
        printf("> ");
        if (!fgets(input, sizeof(input), stdin)) break;

        // Извлекаем название команды
        char name[32];
        int argc = 0;
        sscanf(input, "%31s", name);

        // Проверяем команду выхода
        if (strcmp(name, "exit") == 0) break;

        // Ищем команду в реестре
        const calc_cmd_t *cmd = registry_find(registry, name);
        if (!cmd) {
            printf("Неизвестная команда: %s\n", name);
            continue;
        }

        // Парсим аргументы командной строки
        char *input_copy = strdup(input);
        char *ptr = input_copy + strlen(name);
        char *token = strtok(ptr, " \t\n"); // Разделитель: пробелы и табуляция
        
        // Читаем каждый аргумент и преобразуем в double
        while (token && argc < MAX_ARGS) {
            argv[argc++] = atof(token);
            token = strtok(NULL, " \t\n");
        }
        free(input_copy);

        // Проверяем количество аргументов
        if (argc < cmd->min_args || argc > cmd->max_args) {
            printf("Требуется %d-%d аргументов\n", cmd->min_args, cmd->max_args);
            continue;
        }

        // Вызываем функцию операции и получаем результат
        double result = cmd->fn(argc, argv);
        if (isnan(result))
            printf("Ошибка: недопустимая операция\n");
        else
            printf("= %.6g\n", result);
    }
}