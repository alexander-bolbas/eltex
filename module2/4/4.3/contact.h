#ifndef CONTACT_H
#define CONTACT_H

#include "types.h"

// Создает новый узел контакта
contact_t* contact_create(const contact_t *data);

// Освобождает узел
void contact_free(contact_t *c);

// Копирует поля одного контакта в другой
void contact_copy_data(contact_t *dst, const contact_t *src);

// Выводит контакт
void contact_print(const contact_t *contact);

#endif // CONTACT_H
