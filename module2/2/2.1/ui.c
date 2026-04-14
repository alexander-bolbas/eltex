#include <stdio.h>
#include <string.h>
#include "ui.h"
#include "contact.h"

// Reads a line, removes \n, returns 1 if not empty
static int read_line(const char *prompt, char *buf, int size) {
    printf("%s", prompt);
    if (!fgets(buf, size, stdin)) return 0;
    buf[strcspn(buf, "\n")] = '\0';
    return buf[0] != '\0';
}

// Input with default: Enter key = keep old value
static void read_line_default(const char *label, char *buf, int size) {
    char tmp[256];
    printf("%s [%s]: ", label, buf);
    if (fgets(tmp, sizeof(tmp), stdin)) {
        tmp[strcspn(tmp, "\n")] = '\0';
        if (tmp[0] != '\0')
            strncpy(buf, tmp, size - 1);
    }
}

void ui_input_contact(contact_t *contact) {
    if (!contact) return;
    
    contact_init(contact);
    
    // Required fields
    while (!read_line("Last name: ", contact->name.last_name, MAX_NAME_LEN))
        printf("Last name is required!\n");
    
    while (!read_line("First name: ", contact->name.first_name, MAX_NAME_LEN))
        printf("First name is required!\n");
    
    // Optional fields
    read_line("Middle name: ", contact->name.middle_name, MAX_NAME_LEN);
    read_line("Work place: ", contact->work.place, MAX_FIELD_LEN);
    read_line("Position: ", contact->work.position, MAX_FIELD_LEN);
    read_line("Phone: ", contact->phone, MAX_PHONE_LEN);
    read_line("Email: ", contact->email, MAX_EMAIL_LEN);
    read_line("Social: ", contact->social, MAX_FIELD_LEN);
    read_line("Messenger: ", contact->messenger, MAX_MESSENGER_LEN);
}

static void ui_edit_contact(contact_t *contact) {
    if (!contact) return;
    
    read_line_default("Last name", contact->name.last_name, MAX_NAME_LEN);
    read_line_default("First name", contact->name.first_name, MAX_NAME_LEN);
    read_line_default("Middle name", contact->name.middle_name, MAX_NAME_LEN);
    read_line_default("Work place", contact->work.place, MAX_FIELD_LEN);
    read_line_default("Position", contact->work.position, MAX_FIELD_LEN);
    read_line_default("Phone", contact->phone, MAX_PHONE_LEN);
    read_line_default("Email", contact->email, MAX_EMAIL_LEN);
    read_line_default("Social", contact->social, MAX_FIELD_LEN);
    read_line_default("Messenger", contact->messenger, MAX_MESSENGER_LEN);
}

void ui_run_menu(phone_book_t *phone_book) {
    if (!phone_book) return;
    
    while (1) {
        printf("\n=== PHONE BOOK ===\n");
        printf("1. Add contact\n");
        printf("2. Show all contacts\n");
        printf("3. Find contact by ID\n");
        printf("4. Find contact by name\n");
        printf("5. Edit contact\n");
        printf("6. Delete contact\n");
        printf("0. Exit\n");
        printf("> ");
        
        int choice;
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Input error.\n");
            continue;
        }
        while (getchar() != '\n');
        
        switch (choice) {
            case 1: {
                contact_t new_contact;
                ui_input_contact(&new_contact);
                if (pb_add(phone_book, &new_contact)) {
                    printf("Contact added with ID: %d\n", new_contact.id);
                } else {
                    printf("Error adding contact.\n");
                }
                break;
            }
            
            case 2:
                pb_print_all(phone_book);
                break;
            
            case 3: {
                printf("Enter contact ID: ");
                int id;
                if (scanf("%d", &id) == 1) {
                    while (getchar() != '\n');
                    const contact_t *c = pb_find_by_id(phone_book, id);
                    if (c) {
                        contact_print(c);
                    } else {
                        printf("Contact with ID %d not found.\n", id);
                    }
                } else {
                    while (getchar() != '\n');
                    printf("Input error.\n");
                }
                break;
            }
            
            case 4: {
                char first_name[MAX_NAME_LEN] = {0};
                char last_name[MAX_NAME_LEN] = {0};
                
                read_line("Enter first name: ", first_name, MAX_NAME_LEN);
                read_line("Enter last name: ", last_name, MAX_NAME_LEN);
                
                const contact_t *c = pb_find_by_name(phone_book, first_name, last_name);
                if (c) {
                    contact_print(c);
                } else {
                    printf("Contact not found.\n");
                }
                break;
            }
            
            case 5: {
                printf("Enter contact ID to edit: ");
                int id;
                if (scanf("%d", &id) == 1) {
                    while (getchar() != '\n');
                    const contact_t *existing = pb_find_by_id(phone_book, id);
                    if (existing) {
                        contact_t updated = *existing;
                        ui_edit_contact(&updated);
                        if (pb_edit(phone_book, id, &updated)) {
                            printf("Contact updated.\n");
                        }
                    } else {
                        printf("Contact with ID %d not found.\n", id);
                    }
                } else {
                    while (getchar() != '\n');
                    printf("Input error.\n");
                }
                break;
            }
            
            case 6: {
                printf("Enter contact ID to delete: ");
                int id;
                if (scanf("%d", &id) == 1) {
                    while (getchar() != '\n');
                    if (pb_delete(phone_book, id)) {
                        printf("Contact deleted.\n");
                    }
                } else {
                    while (getchar() != '\n');
                    printf("Input error.\n");
                }
                break;
            }
            
            case 0:
                printf("Goodbye!\n");
                return;
            
            default:
                printf("Invalid choice.\n");
        }
    }
}