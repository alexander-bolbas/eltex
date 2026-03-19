#include <stdio.h>
#include <string.h>

/* КОНСТАНТЫ */

/** Максимальное число контактов в книге. */
#define MAX_CONTACTS  100

/** Максимальная длина поля с именем/фамилией/отчеством. */
#define MAX_NAME       64

/** Максимальная длина скалярного текстового поля (работа, должность). */
#define MAX_FIELD     128

/** Максимальное число значений в одном многозначном поле (телефон, email …). */
#define MAX_MULTI       5

/** Максимальная длина одного значения в многозначном поле. */
#define MAX_ENTRY     128

/* ТИПЫ */

/**
 * @brief Один контакт телефонной книги.
 *
 * Поля last_name и first_name обязательны: функция fill_contact_interactive()
 * не вернёт управление, пока они не заполнены. Все остальные поля
 * необязательны — пустая строка означает «не заполнено».
 *
 * Многозначные поля реализованы как двумерные char-массивы плюс
 * счётчик реально заполненных строк.
 */
typedef struct contact_t {
    /* --- обязательные поля --- */
    char last_name [MAX_NAME];   /**< Фамилия. */
    char first_name[MAX_NAME];   /**< Имя. */

    /* --- необязательные скалярные поля --- */
    char middle_name[MAX_NAME];  /**< Отчество. */
    char workplace  [MAX_FIELD]; /**< Место работы. */
    char position   [MAX_FIELD]; /**< Должность. */

    /* --- многозначные поля --- */
    char phones    [MAX_MULTI][MAX_ENTRY]; /**< Номера телефонов. */
    char emails    [MAX_MULTI][MAX_ENTRY]; /**< Адреса электронной почты. */
    char socials   [MAX_MULTI][MAX_ENTRY]; /**< Ссылки на страницы в соцсетях. */
    char messengers[MAX_MULTI][MAX_ENTRY]; /**< Профили в мессенджерах. */

    /* --- счётчики заполненных элементов многозначных полей --- */
    int phones_cnt;      /**< Число заполненных телефонных номеров. */
    int emails_cnt;      /**< Число заполненных адресов email. */
    int socials_cnt;     /**< Число заполненных ссылок на соцсети. */
    int messengers_cnt;  /**< Число заполненных профилей мессенджеров. */
} contact_t;

/**
 * @brief Телефонная книга: фиксированный массив контактов и их счётчик.
 *
 * Все контакты хранятся подряд: при удалении элемент с индексом idx
 * заполняется сдвигом оставшихся влево, size уменьшается на единицу.
 */
typedef struct phone_book_t {
    contact_t entries[MAX_CONTACTS]; /**< Массив контактов. */
    int       size;                  /**< Число реально хранящихся контактов. */
} phone_book_t;

/* СПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ВВОДА */

/**
 * @brief Прочитать строку из stdin в буфер.
 *
 * Выводит приглашение @p prompt, читает строку через fgets,
 * обрезает завершающий '\n'. При пустом вводе buf[0] == '\0'.
 *
 * @param prompt   Текст приглашения (выводится без добавления '\n').
 * @param buf      Буфер для результата.
 * @param buf_size Размер буфера в байтах.
 */
static void read_line(const char *prompt, char *buf, int buf_size)
{
    printf("%s", prompt);
    fflush(stdout);

    if (fgets(buf, buf_size, stdin) == NULL) {
        /* EOF или ошибка чтения — возвращаем пустую строку */
        buf[0] = '\0';
        return;
    }

    /* убираем '\n', который fgets оставляет в конце */
    int len = (int)strlen(buf);
    if (len > 0 && buf[len - 1] == '\n')
        buf[len - 1] = '\0';
}

/**
 * @brief Прочитать целое число из stdin.
 *
 * Использует read_line + sscanf. При некорректном вводе возвращает -1.
 *
 * @param prompt Текст приглашения.
 * @return       Введённое целое число или -1 при ошибке.
 */
static int read_int(const char *prompt)
{
    char buf[32];
    read_line(prompt, buf, sizeof(buf));

    int value = -1;
    /* sscanf возвращает число успешно заполненных элементов */
    sscanf(buf, "%d", &value);
    return value;
}

/* РЕДАКТИРОВАНИЕ МНОГОЗНАЧНОГО ПОЛЯ */

/**
 * @brief Интерактивное редактирование одного многозначного поля контакта.
 *
 * Отображает текущее содержимое поля и мини-меню:
 *   a) add    — добавить новое значение (если не превышен MAX_MULTI),
 *   r) remove — удалить значение по 1-индексу (оставшиеся сдвигаются),
 *   d) done   — завершить редактирование поля.
 *
 * При сдвиге после удаления элемент arr[*cnt - 1] обнуляется, чтобы
 * не оставлять «мусорных» копий строк в конце массива.
 *
 * @param label Название поля (выводится как заголовок мини-меню).
 * @param arr   Двумерный массив строк: arr[MAX_MULTI][MAX_ENTRY].
 * @param cnt   Указатель на счётчик заполненных элементов.
 */
static void edit_multifield_interactive(const char *label,
                                        char arr[][MAX_ENTRY], int *cnt)
{
    char cmd[8];
    char value_buf[MAX_ENTRY];

    for (;;) {
        /* --- отображаем заголовок и текущее содержимое --- */
        printf("\n  -- %s --\n", label);
        if (*cnt == 0) {
            puts("  (empty)");
        } else {
            for (int i = 0; i < *cnt; i++)
                printf("  %d. %s\n", i + 1, arr[i]);
        }

        puts("  a) add   r) remove   d) done");
        read_line("  > ", cmd, sizeof(cmd));

        if (cmd[0] == 'a') {
            /* --- добавление: проверяем лимит, затем записываем в следующую ячейку --- */
            if (*cnt >= MAX_MULTI) {
                printf("  Limit reached (%d entries max).\n", MAX_MULTI);
                continue;
            }
            read_line("  value: ", value_buf, sizeof(value_buf));
            if (value_buf[0] == '\0') {
                puts("  Empty value ignored.");
            } else {
                strncpy(arr[*cnt], value_buf, MAX_ENTRY - 1);
                arr[*cnt][MAX_ENTRY - 1] = '\0'; /* гарантируем завершающий нуль */
                (*cnt)++;
            }

        } else if (cmd[0] == 'r') {
            if (*cnt == 0) { puts("  Nothing to remove."); continue; }

            int idx = read_int("  index: ");
            if (idx < 1 || idx > *cnt) {
                puts("  Bad index.");
            } else {
                /* сдвигаем элементы начиная с idx влево на одну позицию */
                for (int i = idx - 1; i < *cnt - 1; i++)
                    strncpy(arr[i], arr[i + 1], MAX_ENTRY);
                /* очищаем освободившуюся последнюю ячейку */
                arr[*cnt - 1][0] = '\0';
                (*cnt)--;
            }

        } else if (cmd[0] == 'd') {
            break; /* выходим из мини-меню */
        }
    }
}

/* ЗАПОЛНЕНИЕ КОНТАКТА */

/**
 * @brief Интерактивно заполнить или перезаписать все поля контакта.
 *
 * Поля опрашиваются в порядке:
 *   1. Last name  (обязательно — повторный запрос при пустом вводе)
 *   2. First name (обязательно — повторный запрос при пустом вводе)
 *   3. Middle name, Workplace, Position (Enter = пропустить / оставить пустым)
 *   4. Мини-меню для каждого из четырёх многозначных полей.
 *
 * При редактировании существующего контакта скалярные поля
 * перезаписываются полностью; многозначные поля редактируются
 * инкрементно через мини-меню.
 *
 * @param contact Указатель на контакт для заполнения.
 */
static void fill_contact_interactive(contact_t *contact)
{
    char input_buf[MAX_ENTRY];

    /* --- обязательное поле: Last name --- */
    for (;;) {
        read_line("  Last name*:    ", input_buf, sizeof(input_buf));
        if (input_buf[0] != '\0') break;
        puts("  Field is required. Please try again.");
    }
    strncpy(contact->last_name, input_buf, MAX_NAME - 1);
    contact->last_name[MAX_NAME - 1] = '\0';

    /* --- обязательное поле: First name --- */
    for (;;) {
        read_line("  First name*:   ", input_buf, sizeof(input_buf));
        if (input_buf[0] != '\0') break;
        puts("  Field is required. Please try again.");
    }
    strncpy(contact->first_name, input_buf, MAX_NAME - 1);
    contact->first_name[MAX_NAME - 1] = '\0';

    /* --- необязательные скалярные поля --- */
    read_line("  Middle name    (Enter to skip): ", input_buf, sizeof(input_buf));
    strncpy(contact->middle_name, input_buf, MAX_NAME - 1);
    contact->middle_name[MAX_NAME - 1] = '\0';

    read_line("  Workplace      (Enter to skip): ", input_buf, sizeof(input_buf));
    strncpy(contact->workplace, input_buf, MAX_FIELD - 1);
    contact->workplace[MAX_FIELD - 1] = '\0';

    read_line("  Position       (Enter to skip): ", input_buf, sizeof(input_buf));
    strncpy(contact->position, input_buf, MAX_FIELD - 1);
    contact->position[MAX_FIELD - 1] = '\0';

    /* --- многозначные поля, каждое в своём мини-меню --- */
    edit_multifield_interactive("Phones",          contact->phones,     &contact->phones_cnt);
    edit_multifield_interactive("Emails",          contact->emails,     &contact->emails_cnt);
    edit_multifield_interactive("Social networks", contact->socials,    &contact->socials_cnt);
    edit_multifield_interactive("Messengers",      contact->messengers, &contact->messengers_cnt);
}

/* ВЫВОД КОНТАКТОВ */

/**
 * @brief Вывести содержимое многозначного поля в одну строку.
 *
 * Если поле пустое (cnt == 0), функция ничего не выводит.
 * Значения разделяются строкой " | ".
 *
 * @param label Метка поля; выводится выровненной в 18 символов.
 * @param arr   Двумерный массив строк поля (только чтение).
 * @param cnt   Число заполненных элементов массива.
 */
static void display_multifield(const char *label,
                                const char arr[][MAX_ENTRY], int cnt)
{
    if (cnt == 0) return;

    printf("    %-18s", label);
    for (int i = 0; i < cnt; i++) {
        printf("%s", arr[i]);
        if (i < cnt - 1) printf(" | "); /* разделитель между значениями */
    }
    putchar('\n');
}

/**
 * @brief Вывести один контакт с его порядковым номером в книге.
 *
 * Формат вывода:
 *   [N] Last First Middle
 *       Workplace:        …
 *       Position:         …
 *       Phones:           … | …
 *       …
 *
 * Незаполненные поля (пустые строки) молча пропускаются.
 *
 * @param list_index 1-индекс контакта в книге (выводится в скобках).
 * @param contact    Контакт для отображения (только чтение).
 */
static void display_contact(int list_index, const contact_t *contact)
{
    printf("\n  [%d] %s %s", list_index,
           contact->last_name, contact->first_name);
    /* отчество выводим только если заполнено */
    if (contact->middle_name[0] != '\0')
        printf(" %s", contact->middle_name);
    putchar('\n');

    if (contact->workplace[0] != '\0')
        printf("    %-18s%s\n", "Workplace:", contact->workplace);
    if (contact->position[0] != '\0')
        printf("    %-18s%s\n", "Position:",  contact->position);

    display_multifield("Phones:",      contact->phones,     contact->phones_cnt);
    display_multifield("Emails:",      contact->emails,     contact->emails_cnt);
    display_multifield("Social nets:", contact->socials,    contact->socials_cnt);
    display_multifield("Messengers:",  contact->messengers, contact->messengers_cnt);
}

/**
 * @brief Вывести все контакты телефонной книги.
 *
 * При пустой книге выводит сообщение "(no contacts)".
 * Контакты нумеруются с 1 — этот же индекс используется в командах
 * редактирования и удаления.
 *
 * @param phone_book Телефонная книга для отображения (только чтение).
 */
static void display_all_contacts(const phone_book_t *phone_book)
{
    if (phone_book->size == 0) {
        puts("  (no contacts)");
        return;
    }

    for (int i = 0; i < phone_book->size; i++)
        display_contact(i + 1, &phone_book->entries[i]);

    putchar('\n');
}

/* ОПЕРАЦИИ С КНИГОЙ */

/**
 * @brief Добавить новый пустой контакт в конец книги.
 *
 * Обнуляет ячейку массива через memset, увеличивает счётчик size
 * и возвращает указатель на новый контакт для дальнейшего заполнения.
 * При переполнении возвращает NULL, не изменяя книгу.
 *
 * @param phone_book Телефонная книга.
 * @return           Указатель на новую запись или NULL при переполнении.
 */
static contact_t *phone_book_add(phone_book_t *phone_book)
{
    if (phone_book->size >= MAX_CONTACTS) return NULL;

    contact_t *new_contact = &phone_book->entries[phone_book->size];
    memset(new_contact, 0, sizeof(contact_t)); /* обнуляем все поля */
    phone_book->size++;
    return new_contact;
}

/**
 * @brief Получить указатель на контакт по его 1-индексу.
 *
 * @param phone_book Телефонная книга.
 * @param index      1-индекс контакта (от 1 до phone_book->size включительно).
 * @return           Указатель на контакт или NULL при выходе за границы.
 */
static contact_t *phone_book_get(phone_book_t *phone_book, int index)
{
    if (index < 1 || index > phone_book->size) return NULL;
    return &phone_book->entries[index - 1];
}

/**
 * @brief Удалить контакт по 1-индексу, сдвинув оставшиеся.
 *
 * Элементы после удалённого сдвигаются влево на одну позицию —
 * таким образом в массиве не остаётся «дыр». Освободившаяся
 * последняя ячейка обнуляется через memset. Счётчик size уменьшается.
 *
 * @param phone_book Телефонная книга.
 * @param index      1-индекс контакта для удаления.
 * @return           1 при успехе, 0 если index выходит за пределы.
 */
static int phone_book_delete(phone_book_t *phone_book, int index)
{
    if (index < 1 || index > phone_book->size) return 0;

    /* сдвигаем все контакты начиная с index влево на одну позицию */
    for (int i = index - 1; i < phone_book->size - 1; i++)
        phone_book->entries[i] = phone_book->entries[i + 1];

    /* обнуляем освободившуюся последнюю ячейку */
    memset(&phone_book->entries[phone_book->size - 1], 0, sizeof(contact_t));
    phone_book->size--;
    return 1;
}

/* ТОЧКА ВХОДА */

/**
 * @brief Точка входа — главный цикл меню.
 *
 * Инициализирует пустую книгу через memset, затем в бесконечном цикле
 * отображает главное меню и обрабатывает выбор:
 *   1 — вывести все контакты,
 *   2 — создать новый контакт,
 *   3 — редактировать существующий контакт,
 *   4 — удалить контакт,
 *   0 — выйти из программы.
 *
 * @return 0 при нормальном завершении.
 */
int main(void)
{
    phone_book_t phone_book;
    memset(&phone_book, 0, sizeof(phone_book_t));

    puts("=== Phone Book ===");

    for (;;) {
        puts("\n1. List contacts");
        puts("2. Add contact");
        puts("3. Edit contact");
        puts("4. Delete contact");
        puts("0. Exit");

        int choice = read_int("> ");

        switch (choice) {

        case 1:
            display_all_contacts(&phone_book);
            break;

        case 2: {
            contact_t *new_contact = phone_book_add(&phone_book);
            if (new_contact == NULL) {
                printf("  Phone book is full (max %d contacts).\n", MAX_CONTACTS);
                break;
            }
            puts("\n-- New contact (* required) --");
            fill_contact_interactive(new_contact);
            puts("Contact added.");
            break;
        }

        case 3: {
            display_all_contacts(&phone_book);
            if (phone_book.size == 0) break;

            int index = read_int("Index to edit: ");
            contact_t *target = phone_book_get(&phone_book, index);
            if (target == NULL) { puts("Bad index."); break; }

            puts("\n-- Edit contact (* required) --");
            fill_contact_interactive(target);
            puts("Contact updated.");
            break;
        }

        case 4: {
            display_all_contacts(&phone_book);
            if (phone_book.size == 0) break;

            int index = read_int("Index to delete: ");
            if (phone_book_delete(&phone_book, index)) puts("Contact deleted.");
            else                                        puts("Bad index.");
            break;
        }

        case 0:
            puts("Bye.");
            return 0;

        default:
            puts("Unknown option.");
        }
    }
}
