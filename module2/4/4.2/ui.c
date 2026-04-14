#include <stdio.h>
#include <stdlib.h>
#include "ui.h"

void ui_run_menu(void) {
    pq_t pq;
    pq_init(&pq);
    
    while (1) {
        printf("\n=== ОЧЕРЕДЬ С ПРИОРИТЕТОМ ===\n");
        printf("1. Добавить сообщение\n");
        printf("2. Извлечь первое (наивысший приоритет)\n");
        printf("3. Извлечь с конкретным приоритетом\n");
        printf("4. Извлечь с приоритетом не ниже чем...\n");
        printf("5. Показать все\n");
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
                // Добавить сообщение
                msg_t msg;
                printf("Введите данные сообщения: ");
                if (scanf("%d", &msg.data) != 1) {
                    while (getchar() != '\n');
                    printf("Ошибка ввода.\n");
                    break;
                }
                printf("Введите приоритет (0-255): ");
                if (scanf("%d", &msg.priority) != 1) {
                    while (getchar() != '\n');
                    printf("Ошибка ввода.\n");
                    break;
                }
                while (getchar() != '\n');
                
                if (msg.priority < 0 || msg.priority >= PQ_MAX_PRIORITY) {
                    printf("Ошибка: приоритет должен быть 0-255.\n");
                    break;
                }
                
                if (pq_enqueue(&pq, msg) == 0) {
                    printf("Сообщение добавлено: данные=%d, приоритет=%d\n", 
                           msg.data, msg.priority);
                } else {
                    printf("Ошибка при добавлении сообщения.\n");
                }
                break;
            }
            
            case 2: {
                // Извлечь первое (наивысший приоритет)
                msg_t msg;
                if (pq_dequeue_first(&pq, &msg) == 0) {
                    printf("Извлечено: данные=%d, приоритет=%d\n", 
                           msg.data, msg.priority);
                } else {
                    printf("Очередь пуста.\n");
                }
                break;
            }
            
            case 3: {
                // Извлечь с конкретным приоритетом
                int priority;
                printf("Введите приоритет (0-255): ");
                if (scanf("%d", &priority) != 1) {
                    while (getchar() != '\n');
                    printf("Ошибка ввода.\n");
                    break;
                }
                while (getchar() != '\n');
                
                if (priority < 0 || priority >= PQ_MAX_PRIORITY) {
                    printf("Ошибка: приоритет должен быть 0-255.\n");
                    break;
                }
                
                msg_t msg;
                if (pq_dequeue_by_priority(&pq, priority, &msg) == 0) {
                    printf("Извлечено: данные=%d, приоритет=%d\n", 
                           msg.data, msg.priority);
                } else {
                    printf("Нет сообщений с приоритетом %d.\n", priority);
                }
                break;
            }
            
            case 4: {
                // Извлечь с приоритетом не ниже чем min_p
                int min_p;
                printf("Введите минимальный приоритет (0-255): ");
                if (scanf("%d", &min_p) != 1) {
                    while (getchar() != '\n');
                    printf("Ошибка ввода.\n");
                    break;
                }
                while (getchar() != '\n');
                
                if (min_p < 0) min_p = 0;
                if (min_p >= PQ_MAX_PRIORITY) min_p = PQ_MAX_PRIORITY - 1;
                
                msg_t msg;
                if (pq_dequeue_min_priority(&pq, min_p, &msg) == 0) {
                    printf("Извлечено: данные=%d, приоритет=%d\n", 
                           msg.data, msg.priority);
                } else {
                    printf("Нет сообщений с приоритетом <= %d.\n", min_p);
                }
                break;
            }
            
            case 5:
                pq_print(&pq);
                break;
            
            case 0:
                printf("До свидания!\n");
                pq_free(&pq);
                return;
            
            default:
                printf("Неверный выбор.\n");
        }
    }
}
