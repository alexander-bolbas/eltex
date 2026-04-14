#include <stdio.h>
#include "stats.h"

void stats_init(route_stats_t *s) {
    if (!s) return;
    s->total = 0;
    s->same_subnet = 0;
    s->other_subnet = 0;
}

void stats_record(route_stats_t *s, int is_same) {
    if (!s) return;
    s->total++;
    if (is_same) {
        s->same_subnet++;
    } else {
        s->other_subnet++;
    }
}

// Инициализация статистики
void stats_init(route_stats_t *s) {
    if (!s) return;
    s->total = 0;           // Всего пакетов
    s->same_subnet = 0;     // Пакетов в своей подсети
    s->other_subnet = 0;    // Пакетов в других подсетях
}

// Записать результат - был ли пакет в своей подсети или нет
void stats_record(route_stats_t *s, int is_same) {
    if (!s) return;
    s->total++; // Увеличиваем счетчик всех пакетов
    if (is_same) {
        s->same_subnet++; // Пакет в своей подсети
    } else {
        s->other_subnet++; // Пакет в другой подсети
    }
}

// Вывести статистику и процентные соотношения
void stats_print(const route_stats_t *s) {
    if (!s) return;
    
    printf("\n=== Статистика ===\n");
    printf("Всего пакето: %d\n", s->total);
    printf("Своя подсеть: %d", s->same_subnet);
    
    // Выводим процент
    if (s->total > 0) {
        printf(" (%.1f%%)", (100.0 * s->same_subnet) / s->total);
    }
    printf("\n");
    
    printf("Другие подсети: %d", s->other_subnet);
    
    // Выводим процент
    if (s->total > 0) {
        printf(" (%.1f%%)", (100.0 * s->other_subnet) / s->total);
    }
    printf("\n");
}
