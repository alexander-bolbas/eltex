#include "contact.h"
#include <string.h>
#include <stdio.h>

void contact_init(contact_t *contact) {
    if (contact) {
        memset(contact, 0, sizeof(contact_t));
    }
}

void contact_print(const contact_t *contact) {
    if (!contact) return;
    printf("ID: %d\n", contact->id);
    printf("Last name: %s\n", contact->name.last_name);
    printf("First name: %s\n", contact->name.first_name);
    printf("Middle name: %s\n", contact->name.middle_name);
    printf("Work place: %s\n", contact->work.place);
    printf("Position: %s\n", contact->work.position);
    printf("Phone: %s\n", contact->phone);
    printf("Email: %s\n", contact->email);
    printf("Social: %s\n", contact->social);
    printf("Messenger: %s\n", contact->messenger);
}