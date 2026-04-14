#ifndef PHONE_BOOK_H
#define PHONE_BOOK_H

#include "types.h"

void pb_init(phone_book_t *phone_book);
int pb_add(phone_book_t *phone_book, const contact_t *contact);
int pb_edit(phone_book_t *phone_book, int id, const contact_t *contact);
int pb_delete(phone_book_t *phone_book, int id);
const contact_t* pb_find_by_id(const phone_book_t *phone_book, int id);
const contact_t* pb_find_by_name(const phone_book_t *phone_book, 
                                   const char *first_name, 
                                   const char *last_name);
void pb_print_all(const phone_book_t *phone_book);

#endif // PHONE_BOOK_H