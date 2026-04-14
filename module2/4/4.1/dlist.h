#ifndef DLIST_H
#define DLIST_H

#include "contact.h"

// Инициализация пустого списка
void dlist_init(dlist_t *dl);

// Освобождение всех узлов списка
void dlist_free(dlist_t *dl);

// Вставляет контакт в отсортированное место
contact_t* dlist_insert_sorted(dlist_t *dl, const contact_t *data);

// Удаляет узел из списка
void dlist_remove(dlist_t *dl, contact_t *node);

// Поиск контакта по ID
contact_t* dlist_find_by_id(dlist_t *dl, int id);

// Поиск контакта по имени
contact_t* dlist_find_by_name(dlist_t *dl, const char *first_name, const char *last_name);

// После изменения имени — переставляет узел в списке
void dlist_reinsert(dlist_t *dl, contact_t **node_ptr);

// Выводит все контакты
void dlist_print_all(const dlist_t *dl);

#endif // DLIST_H
