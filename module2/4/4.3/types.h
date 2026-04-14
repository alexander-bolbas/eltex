#ifndef TYPES_H
#define TYPES_H

#include <stddef.h>

#define MAX_NAME_LEN 64
#define MAX_FIELD_LEN 128
#define MAX_PHONE_LEN 32
#define MAX_EMAIL_LEN 64
#define MAX_MESSENGER_LEN 64
#define AVL_REBUILD_EVERY 20

typedef struct {
    char last_name[MAX_NAME_LEN];
    char first_name[MAX_NAME_LEN];
    char middle_name[MAX_NAME_LEN];
} person_name_t;

typedef struct {
    char place[MAX_FIELD_LEN];
    char position[MAX_FIELD_LEN];
} work_info_t;

// Прямое объявление для самоссылки
typedef struct contact contact_t;

struct contact {
    int id;
    person_name_t name;
    work_info_t work;
    char phone[MAX_PHONE_LEN];
    char email[MAX_EMAIL_LEN];
    char social[MAX_FIELD_LEN];
    char messenger[MAX_MESSENGER_LEN];
    
    contact_t *left;
    contact_t *right;
    int height;
};

typedef struct {
    contact_t *root;
    int count;
    int id_counter;
    int op_count;
} avl_tree_t;

#endif // TYPES_H
