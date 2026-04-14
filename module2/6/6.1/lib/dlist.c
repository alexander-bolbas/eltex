#include <stdio.h>
#include <string.h>
#include "dlist.h"

void dlist_init(dlist_t *dl) {
    if (!dl) return;
    dl->head = dl->tail = NULL;
    dl->count = 0;
    dl->id_counter = 1;
}

void dlist_free(dlist_t *dl) {
    if (!dl) return;
    
    contact_t *cur = dl->head;
    while (cur) {
        contact_t *next = cur->next;
        contact_free(cur);
        cur = next;
    }
    
    dl->head = dl->tail = NULL;
    dl->count = 0;
}

contact_t* dlist_insert_sorted(dlist_t *dl, const contact_t *data) {
    if (!dl || !data) return NULL;
    
    contact_t *node = contact_create(data);
    if (!node) return NULL;
    
    node->id = dl->id_counter++;
    
    // Список пуст
    if (!dl->head) {
        dl->head = dl->tail = node;
        dl->count++;
        return node;
    }
    
    // Ищем место вставки
    contact_t *cur = dl->head;
    while (cur && contact_compare(node, cur) > 0)
        cur = cur->next;
    
    if (!cur) {
        // Вставка в конец
        node->prev = dl->tail;
        dl->tail->next = node;
        dl->tail = node;
    } else if (cur == dl->head) {
        // Вставка в начало
        node->next = dl->head;
        dl->head->prev = node;
        dl->head = node;
    } else {
        // Вставка в середину
        node->prev = cur->prev;
        node->next = cur;
        cur->prev->next = node;
        cur->prev = node;
    }
    
    dl->count++;
    return node;
}

void dlist_remove(dlist_t *dl, contact_t *node) {
    if (!dl || !node) return;
    
    if (node->prev) 
        node->prev->next = node->next;
    else            
        dl->head = node->next;
    
    if (node->next) 
        node->next->prev = node->prev;
    else            
        dl->tail = node->prev;
    
    node->prev = node->next = NULL;
    dl->count--;
    contact_free(node);
}

contact_t* dlist_find_by_id(dlist_t *dl, int id) {
    if (!dl) return NULL;
    
    for (contact_t *cur = dl->head; cur; cur = cur->next) {
        if (cur->id == id)
            return cur;
    }
    
    return NULL;
}

contact_t* dlist_find_by_name(dlist_t *dl, const char *first_name, const char *last_name) {
    if (!dl || !first_name || !last_name) return NULL;
    
    for (contact_t *cur = dl->head; cur; cur = cur->next) {
        if (strcmp(cur->name.first_name, first_name) == 0 &&
            strcmp(cur->name.last_name, last_name) == 0) {
            return cur;
        }
    }
    
    return NULL;
}

void dlist_reinsert(dlist_t *dl, contact_t **node_ptr) {
    if (!dl || !node_ptr || !*node_ptr) return;
    
    contact_t *node = *node_ptr;
    contact_t data = *node;
    int saved_id = node->id;
    
    dlist_remove(dl, node);
    
    contact_t *new_node = dlist_insert_sorted(dl, &data);
    if (new_node) {
        new_node->id = saved_id;
        *node_ptr = new_node;
    }
    dl->id_counter--;  // не считать как новый
}

void dlist_print_all(const dlist_t *dl) {
    if (!dl) return;
    
    if (dl->count == 0) {
        printf("Телефонная книга пуста.\n");
        return;
    }
    
    int idx = 1;
    for (contact_t *cur = dl->head; cur; cur = cur->next) {
        printf("\n=== Контакт %d ===\n", idx++);
        contact_print(cur);
    }
}
