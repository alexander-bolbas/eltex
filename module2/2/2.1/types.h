#ifndef TYPES_H
#define TYPES_H

#define MAX_CONTACTS 64
#define MAX_NAME_LEN 64
#define MAX_FIELD_LEN 128
#define MAX_PHONE_LEN 32
#define MAX_EMAIL_LEN 64
#define MAX_MESSENGER_LEN 64

typedef struct {
    char last_name[MAX_NAME_LEN];
    char first_name[MAX_NAME_LEN];
    char middle_name[MAX_NAME_LEN];
} person_name_t;

typedef struct {
    char place[MAX_FIELD_LEN];
    char position[MAX_FIELD_LEN];
} work_info_t;

typedef struct {
    int id;
    person_name_t name;
    work_info_t work;
    char phone[MAX_PHONE_LEN];
    char email[MAX_EMAIL_LEN];
    char social[MAX_FIELD_LEN];
    char messenger[MAX_MESSENGER_LEN];
} contact_t;

typedef struct {
    contact_t contacts[MAX_CONTACTS];
    int count;
    int next_id;
} phone_book_t;

#endif // TYPES_H