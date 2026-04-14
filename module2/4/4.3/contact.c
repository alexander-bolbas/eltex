#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "contact.h"

contact_t* contact_create(const contact_t *data) {
    if (!data) return NULL;
    
    contact_t *c = malloc(sizeof(contact_t));
    if (!c) return NULL;
    
    *c = *data;
    c->left = c->right = NULL;
    c->height = 1;
    
    return c;
}

void contact_free(contact_t *c) {
    if (c) free(c);
}

void contact_copy_data(contact_t *dst, const contact_t *src) {
    if (!dst || !src) return;
    
    strcpy(dst->name.first_name, src->name.first_name);
    strcpy(dst->name.last_name, src->name.last_name);
    strcpy(dst->name.middle_name, src->name.middle_name);
    
    strcpy(dst->work.place, src->work.place);
    strcpy(dst->work.position, src->work.position);
    
    strcpy(dst->phone, src->phone);
    strcpy(dst->email, src->email);
    strcpy(dst->social, src->social);
    strcpy(dst->messenger, src->messenger);
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
