#ifndef CONTACT_H
#define CONTACT_H

#include "types.h"

// Выделяет и инициализирует новый узел
contact_t* contact_create(const contact_t *data);

// Освобождает узел
void contact_free(contact_t *c);

// Сравнивает контакты: <0 если a < b, 0 если равны, >0 если a > b
// Сортировка по фамилии, потом по имени
int contact_compare(const contact_t *a, const contact_t *b);

// Выводит контакт
void contact_print(const contact_t *contact);

#endif // CONTACT_H
