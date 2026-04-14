#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pq.h"

void pq_init(pq_t *pq) {
    if (!pq) return;
    memset(pq, 0, sizeof(*pq));
}

// Очистка всей очереди с приоритетами
void pq_free(pq_t *pq) {
    if (!pq) return;
    
    // Проходим по всем уровням приоритета
    for (int p = 0; p < PQ_MAX_PRIORITY; p++) {
        pq_level_t *lvl = &pq->levels[p];
        pq_node_t *node = lvl->head;
        while (node) {
            pq_node_t *next = node->next; // Сохраняем следующий узел, так как будем удалять текущий
            free(node);
            node = next;
        }
        lvl->head = NULL;
        lvl->tail = NULL;
    }
    pq->total_count = 0;
}

// Добавить элемент в соответствующую очередь с заданным приоритетом
int pq_enqueue(pq_t *pq, msg_t msg) {
    if (!pq) return -1;
    if (msg.priority < 0 || msg.priority >= PQ_MAX_PRIORITY) return -1; // Проверка диапазона приоритета
    
    // Создаем новый узел для элемента
    pq_node_t *node = malloc(sizeof(pq_node_t));
    if (!node) return -1; // Ошибка выделения памяти
    
    node->msg = msg;
    node->next = NULL;
    
    pq_level_t *lvl = &pq->levels[msg.priority]; // Получаем очередь с нужным приоритетом
    
    // Присоединяем в конец очереди
    if (lvl->tail) 
        lvl->tail->next = node; // Меняем чтение предыдущего последнего элемента
    else           
        lvl->head = node; // Если очередь была пуста - это первый элемент
    
    lvl->tail = node; // Обновляем последний элемент
    pq->total_count++; // Увеличиваем счетчик
    
    return 0;
}

// Извлечь с наивысшим приоритетом (наименьший индекс)
int pq_dequeue_first(pq_t *pq, msg_t *out) {
    if (!pq || !out) return -1;
    
    for (int p = 0; p < PQ_MAX_PRIORITY; p++) {
        pq_level_t *lvl = &pq->levels[p];
        if (!lvl->head) continue;
        
        pq_node_t *node = lvl->head;
        *out = node->msg;
        
        lvl->head = node->next;
        if (!lvl->head) lvl->tail = NULL;
        
        free(node);
        pq->total_count--;
        return 0;
    }
    
    return -1;  // пусто
}

// Отключить элемент с конкретным приоритетом
int pq_dequeue_by_priority(pq_t *pq, int priority, msg_t *out) {
    if (!pq || !out) return -1;
    if (priority < 0 || priority >= PQ_MAX_PRIORITY) return -1; // Проверка диапазона
    
    pq_level_t *lvl = &pq->levels[priority];
    if (!lvl->head) return -1; // очередь пуста
    
    pq_node_t *node = lvl->head;
    *out = node->msg;
    
    lvl->head = node->next;
    if (!lvl->head) lvl->tail = NULL;
    
    free(node);
    pq->total_count--;
    
    return 0;
}

// Отключить первый элемент с приоритетом НЕ НИЖЕ чем min_p (тў есть индекс <= min_p)
int pq_dequeue_min_priority(pq_t *pq, int min_p, msg_t *out) {
    if (!pq || !out) return -1;
    
    // Нормализуем min_p в диапазон [0, PQ_MAX_PRIORITY-1]
    if (min_p >= PQ_MAX_PRIORITY) 
        min_p = PQ_MAX_PRIORITY - 1;
    if (min_p < 0) 
        min_p = 0;
    
    // Ищем с высоким приоритетом (0)к min_p
    for (int p = 0; p <= min_p; p++) {
        if (pq->levels[p].head)
            return pq_dequeue_by_priority(pq, p, out);
    }
    
    return -1;  // нет элемента с требуемым приоритетом
}

// Вывести состояние очереди с приоритетами
void pq_print(const pq_t *pq) {
    if (!pq) return;
    
    printf("\n=== Очередь с приоритетами (ALL: %d) ===\n", pq->total_count);
    
    // Просматриваем все уровни приоритетов
    for (int p = 0; p < PQ_MAX_PRIORITY; p++) {
        const pq_level_t *lvl = &pq->levels[p];
        if (!lvl->head) continue; // этот уровень приоритета пуст
        
        printf("Уровень %3d: ", p);
        pq_node_t *node = lvl->head;
        while (node) {
            printf("%d ", node->msg.data);
            node = node->next;
        }
        printf("\n");
    }
}
