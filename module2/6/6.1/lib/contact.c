#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "contact.h"

contact_t* contact_create(const contact_t *data) {
    if (!data) return NULL;
    
    contact_t *c = malloc(sizeof(contact_t));
    if (!c) return NULL;
    
    *c = *data;
    c->prev = c->next = NULL;
    
    return c;
}

void contact_free(contact_t *c) {
    if (c) free(c);
}

int contact_compare(const contact_t *a, const contact_t *b) {
    if (!a || !b) return 0;
    
    int cmp = strcmp(a->name.last_name, b->name.last_name);
    if (cmp != 0) return cmp;
    
    return strcmp(a->name.first_name, b->name.first_name);
}

void contact_print(const contact_t *contact) {
    if (!contact) return;
    printf("ID: %d\n", contact->id);
    printf("Фамилия: %s\n", contact->name.last_name);
    printf("Имя: %s\n", contact->name.first_name);
    printf("Отчество: %s\n", contact->name.middle_name);
    printf("Место работы: %s\n", contact->work.place);
    printf("Должность: %s\n", contact->work.position);
    printf("Телефон: %s\n", contact->phone);
    printf("Email: %s\n", contact->email);
    printf("Соцсеть: %s\n", contact->social);
    printf("Мессенджер: %s\n", contact->messenger);
}
