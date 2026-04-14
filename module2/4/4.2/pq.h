#ifndef PQ_H
#define PQ_H

#include "types.h"

// Инициализирует пустую очередь
void pq_init(pq_t *pq);

// Освобождает все элементы очереди
void pq_free(pq_t *pq);

// Добавляет элемент в очередь
int pq_enqueue(pq_t *pq, msg_t msg);

// Извлекает элемент с наивысшим приоритетом (наименьший индекс)
int pq_dequeue_first(pq_t *pq, msg_t *out);

// Извлекает элемент с конкретным приоритетом
int pq_dequeue_by_priority(pq_t *pq, int priority, msg_t *out);

// Извлекает первый элемент с приоритетом не ниже заданного
int pq_dequeue_min_priority(pq_t *pq, int min_p, msg_t *out);

// Выводит содержимое очереди
void pq_print(const pq_t *pq);

#endif // PQ_H
