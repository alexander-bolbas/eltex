#include <stdio.h>
#include <string.h>
#include "phone_book.h"
#include "contact.h"

void pb_init(phone_book_t *phone_book) {
    if (phone_book) {
        phone_book->count = 0;
        phone_book->next_id = 1;
    }
}

// Приватная вспомогательная функция для поиска индекса по ID
static int pb_find_index(const phone_book_t *pb, int id) {
    for (int i = 0; i < pb->count; i++)
        if (pb->contacts[i].id == id)
            return i;
    return -1;
}

int pb_add(phone_book_t *phone_book, const contact_t *contact) {
    if (!phone_book || !contact) return 0;
    if (phone_book->count >= MAX_CONTACTS) {
        printf("Телефонная книга заполнена.\n");
        return 0;
    }
    
    contact_t *slot = &phone_book->contacts[phone_book->count];
    *slot = *contact;
    slot->id = phone_book->next_id++;
    phone_book->count++;
    return 1;
}

int pb_edit(phone_book_t *phone_book, int id, const contact_t *contact) {
    if (!phone_book || !contact) return 0;
    
    int idx = pb_find_index(phone_book, id);
    if (idx < 0) {
        printf("Контакт с ID %d не найден.\n", id);
        return 0;
    }
    
    contact_t *target = &phone_book->contacts[idx];
    *target = *contact;
    target->id = id;
    return 1;
}

int pb_delete(phone_book_t *phone_book, int id) {
    if (!phone_book) return 0;
    
    int idx = pb_find_index(phone_book, id);
    if (idx < 0) {
        printf("Контакт с ID %d не найден.\n", id);
        return 0;
    }
    
    // Сдвиг элементов
    for (int i = idx; i < phone_book->count - 1; i++)
        phone_book->contacts[i] = phone_book->contacts[i + 1];
    
    phone_book->count--;
    return 1;
}

const contact_t* pb_find_by_id(const phone_book_t *phone_book, int id) {
    if (!phone_book) return NULL;
    
    int idx = pb_find_index(phone_book, id);
    return (idx >= 0) ? &phone_book->contacts[idx] : NULL;
}

const contact_t* pb_find_by_name(const phone_book_t *phone_book,
                                  const char *first_name,
                                  const char *last_name) {
    if (!phone_book || !first_name || !last_name) return NULL;
    
    for (int i = 0; i < phone_book->count; i++) {
        const contact_t *c = &phone_book->contacts[i];
        if (strcmp(c->name.first_name, first_name) == 0 &&
            strcmp(c->name.last_name, last_name) == 0) {
            return c;
        }
    }
    return NULL;
}

void pb_print_all(const phone_book_t *phone_book) {
    if (!phone_book) return;
    
    if (phone_book->count == 0) {
        printf("Phone book is empty.\n");
        return;
    }
    
    for (int i = 0; i < phone_book->count; i++) {
        const contact_t *c = &phone_book->contacts[i];
        printf("\n=== Contact %d ===\n", i + 1);
        contact_print(c);
    }
}