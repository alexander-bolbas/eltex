#include <stdio.h>
#include <string.h>
#include "cmd_registry.h"

// Инициализация реестра команд
void registry_init(cmd_registry_t *r) {
    r->count = 0; // Устанавливаем счетчик команд в 0
}

// Регистрация новой команды в реестре
int registry_register(cmd_registry_t *r, calc_cmd_t cmd) {
    if (!r || r->count >= MAX_CMDS) return 0; // Проверка: реестр не пуст и не переполнен
    r->cmds[r->count++] = cmd; // Добавляем команду
    return 1;
}

// Поиск команды в реестре по имени
const calc_cmd_t *registry_find(const cmd_registry_t *r, const char *name) {
    for (int i = 0; i < r->count; i++) {
        if (strcmp(r->cmds[i].name, name) == 0) {
            return &r->cmds[i]; // Найдена команда
        }
    }
    return NULL; // Команда не найдена
}

// Вывод меню доступных операций
void registry_print_menu(const cmd_registry_t *r) {
    printf("\nДоступные операции:\n");
    for (int i = 0; i < r->count; i++) {
        printf("  %-8s - %s (аргументы: %d-%d)\n",
               r->cmds[i].name,
               r->cmds[i].description,
               r->cmds[i].min_args,
               r->cmds[i].max_args);
    }
    printf("  exit     - Выход из программы\n");
}