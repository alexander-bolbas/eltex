#include <stdio.h>
#include <string.h>
#include "ui.h"

// Читает строку, убирает \n, возвращает 1 если не пустая
static int read_line(const char *prompt, char *buf, int size) {
    printf("%s", prompt);
    if (!fgets(buf, size, stdin)) return 0;
    buf[strcspn(buf, "\n")] = '\0';
    return buf[0] != '\0';
}

// Ввод с дефолтом: Enter = оставить старое значение
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
    
    memset(contact, 0, sizeof(contact_t));
    
    // Обязательные поля
    while (!read_line("Фамилия: ", contact->name.last_name, MAX_NAME_LEN))
        printf("Фамилия обязательна!\n");
    
    while (!read_line("Имя: ", contact->name.first_name, MAX_NAME_LEN))
        printf("Имя обязательно!\n");
    
    // Необязательные поля
    read_line("Отчество: ", contact->name.middle_name, MAX_NAME_LEN);
    read_line("Место работы: ", contact->work.place, MAX_FIELD_LEN);
    read_line("Должность: ", contact->work.position, MAX_FIELD_LEN);
    read_line("Телефон: ", contact->phone, MAX_PHONE_LEN);
    read_line("Email: ", contact->email, MAX_EMAIL_LEN);
    read_line("Соцсеть: ", contact->social, MAX_FIELD_LEN);
    read_line("Мессенджер: ", contact->messenger, MAX_MESSENGER_LEN);
}

static void ui_edit_contact(contact_t *contact) {
    if (!contact) return;
    
    read_line_default("Фамилия", contact->name.last_name, MAX_NAME_LEN);
    read_line_default("Имя", contact->name.first_name, MAX_NAME_LEN);
    read_line_default("Отчество", contact->name.middle_name, MAX_NAME_LEN);
    read_line_default("Место работы", contact->work.place, MAX_FIELD_LEN);
    read_line_default("Должность", contact->work.position, MAX_FIELD_LEN);
    read_line_default("Телефон", contact->phone, MAX_PHONE_LEN);
    read_line_default("Email", contact->email, MAX_EMAIL_LEN);
    read_line_default("Соцсеть", contact->social, MAX_FIELD_LEN);
    read_line_default("Мессенджер", contact->messenger, MAX_MESSENGER_LEN);
}

void ui_run_menu(dlist_t *dlist) {
    if (!dlist) return;
    
    while (1) {
        printf("\n=== PHONE BOOK (Dynamic Library) ===\n");
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
            printf("Ошибка ввода.\n");
            continue;
        }
        while (getchar() != '\n');
        
        switch (choice) {
            case 1: {
                contact_t new_contact;
                ui_input_contact(&new_contact);
                contact_t *added = dlist_insert_sorted(dlist, &new_contact);
                if (added) {
                    printf("Контакт добавлен с ID: %d\n", added->id);
                } else {
                    printf("Ошибка при добавлении контакта.\n");
                }
                break;
            }
            
            case 2:
                dlist_print_all(dlist);
                break;
            
            case 3: {
                printf("Введите ID контакта: ");
                int id;
                if (scanf("%d", &id) == 1) {
                    while (getchar() != '\n');
                    contact_t *c = dlist_find_by_id(dlist, id);
                    if (c) {
                        printf("\n");
                        contact_print(c);
                    } else {
                        printf("Контакт с ID %d не найден.\n", id);
                    }
                } else {
                    while (getchar() != '\n');
                    printf("Ошибка ввода.\n");
                }
                break;
            }
            
            case 4: {
                char first_name[MAX_NAME_LEN] = {0};
                char last_name[MAX_NAME_LEN] = {0};
                
                read_line("Введите имя: ", first_name, MAX_NAME_LEN);
                read_line("Введите фамилию: ", last_name, MAX_NAME_LEN);
                
                contact_t *c = dlist_find_by_name(dlist, first_name, last_name);
                if (c) {
                    printf("\n");
                    contact_print(c);
                } else {
                    printf("Контакт не найден.\n");
                }
                break;
            }
            
            case 5: {
                printf("Введите ID контакта для редактирования: ");
                int id;
                if (scanf("%d", &id) == 1) {
                    while (getchar() != '\n');
                    contact_t *existing = dlist_find_by_id(dlist, id);
                    if (existing) {
                        contact_t updated = *existing;
                        ui_edit_contact(&updated);
                        
                        // Проверяем, изменилось ли имя/фамилия
                        int name_changed = (strcmp(existing->name.first_name, updated.name.first_name) != 0 ||
                                           strcmp(existing->name.last_name, updated.name.last_name) != 0);
                        
                        *existing = updated;
                        
                        if (name_changed) {
                            // После изменения имени переставляем в списке
                            dlist_reinsert(dlist, &existing);
                            printf("Контакт обновлен и переставлен в списке.\n");
                        } else {
                            printf("Контакт обновлен.\n");
                        }
                    } else {
                        printf("Контакт с ID %d не найден.\n", id);
                    }
                } else {
                    while (getchar() != '\n');
                    printf("Ошибка ввода.\n");
                }
                break;
            }
            
            case 6: {
                printf("Введите ID контакта для удаления: ");
                int id;
                if (scanf("%d", &id) == 1) {
                    while (getchar() != '\n');
                    contact_t *c = dlist_find_by_id(dlist, id);
                    if (c) {
                        dlist_remove(dlist, c);
                        printf("Контакт удален.\n");
                    } else {
                        printf("Контакт с ID %d не найден.\n", id);
                    }
                } else {
                    while (getchar() != '\n');
                    printf("Ошибка ввода.\n");
                }
                break;
            }
            
            case 0:
                printf("До свидания!\n");
                return;
            
            default:
                printf("Неверный выбор.\n");
        }
    }
}
