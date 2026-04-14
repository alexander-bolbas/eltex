#include <stdio.h>
#include <string.h>
#include "ui.h"
#include "contact.h"

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

static void print_contact_callback(contact_t *contact, void *userdata) {
    (void)userdata;
    if (contact) {
        contact_print(contact);
        printf("\n");
    }
}

void ui_run_menu(avl_tree_t *tree) {
    if (!tree) return;
    
    while (1) {
        printf("\n=== ТЕЛЕФОННАЯ КНИГА (AVL-дерево) ===\n");
        printf("1. Добавить контакт\n");
        printf("2. Показать все контакты (inorder)\n");
        printf("3. Найти контакт по ID\n");
        printf("4. Редактировать контакт\n");
        printf("5. Удалить контакт\n");
        printf("6. Ребалансировать дерево\n");
        printf("0. Выход\n");
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
                int id = avl_insert(tree, &new_contact);
                if (id > 0) {
                    printf("Контакт добавлен с ID: %d\n", id);
                } else {
                    printf("Ошибка при добавлении контакта.\n");
                }
                break;
            }
            
            case 2:
                if (tree->count == 0) {
                    printf("Телефонная книга пуста.\n");
                } else {
                    printf("=== Все контакты (сортировка по ID) ===\n\n");
                    avl_inorder(tree->root, print_contact_callback, NULL);
                }
                break;
            
            case 3: {
                printf("Введите ID контакта: ");
                int id;
                if (scanf("%d", &id) == 1) {
                    while (getchar() != '\n');
                    contact_t *c = avl_find_by_id(tree, id);
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
                printf("Введите ID контакта для редактирования: ");
                int id;
                if (scanf("%d", &id) == 1) {
                    while (getchar() != '\n');
                    contact_t *c = avl_find_by_id(tree, id);
                    if (c) {
                        ui_edit_contact(c);
                        printf("Контакт обновлен.\n");
                    } else {
                        printf("Контакт с ID %d не найден.\n", id);
                    }
                } else {
                    while (getchar() != '\n');
                    printf("Ошибка ввода.\n");
                }
                break;
            }
            
            case 5: {
                printf("Введите ID контакта для удаления: ");
                int id;
                if (scanf("%d", &id) == 1) {
                    while (getchar() != '\n');
                    if (avl_delete(tree, id) == 0) {
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
            
            case 6: {
                printf("Ребалансировка дерева...\n");
                avl_rebuild(tree);
                printf("Дерево ребалансировано. Элементов: %d\n", tree->count);
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
