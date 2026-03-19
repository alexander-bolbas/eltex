#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/* ТИПЫ */

/**
 * @brief Узел универсального строкового списка.
 *
 * Используется для хранения переменного числа значений внутри контакта:
 * телефонных номеров, адресов email, ссылок на соцсети, мессенджеров.
 * Узлы связаны в порядке добавления.
 */
typedef struct str_node {
    char*            value; /**< Строка на куче; никогда не NULL и не пустая. */
    struct str_node* next;  /**< Следующий узел или NULL, если узел последний. */
} str_node_t;

/**
 * @brief Вся информация об одном контакте.
 *
 * @note last_name и first_name обязательны и после fill_contact_interactive()
 *       никогда не бывают NULL. Все остальные char*-поля могут быть NULL —
 *       это означает, что поле не заполнено. Четыре str_node_t*-поля равны
 *       NULL, когда соответствующий под-список пуст.
 */
typedef struct contact {
    char* last_name;   /**< Фамилия — обязательно. */
    char* first_name;  /**< Имя     — обязательно. */
    char* middle_name; /**< Отчество — необязательно. */
    char* workplace;   /**< Место работы — необязательно. */
    char* position;    /**< Должность — необязательно. */

    str_node_t* phones;      /**< Список телефонных номеров. */
    str_node_t* emails;      /**< Список адресов email. */
    str_node_t* socials;     /**< Список ссылок на профили в соцсетях. */
    str_node_t* messengers;  /**< Список хендлов/ссылок мессенджеров. */
} contact_t;

/**
 * @brief Узел главного списка контактов.
 */
typedef struct contact_node {
    contact_t*           contact; /**< Указатель на контакт; список владеет памятью. */
    struct contact_node* next;    /**< Следующий узел или NULL. */
} contact_node_t;

/**
 * @brief Телефонная книга — односвязный список контактов.
 */
typedef struct contact_list {
    contact_node_t* head; /**< Первый узел или NULL, если список пуст. */
    int             size; /**< Текущее количество контактов. */
} contact_list_t;

/* ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ДЛЯ СТРОКОВОГО СПИСКА */

/**
 * @brief Создать новый узел строкового списка с копией строки @p value.
 *
 * Выделяет память под узел и под строку отдельно.
 * Вызывающая сторона обязана в итоге освободить узел через
 * str_list_free() или str_list_remove_at().
 * При нехватке памяти завершает программу через perror + exit.
 *
 * @param value  Непустая строка для хранения.
 * @return       Указатель на новый узел.
 */
static str_node_t* str_node_create(const char* value) 
{
    str_node_t* node = malloc(sizeof(str_node_t));
    if (!node) { perror("malloc"); exit(EXIT_FAILURE); }
    node->value = malloc(strlen(value) + 1);
    if (!node->value) { perror("malloc"); exit(EXIT_FAILURE); }
    strcpy(node->value, value);
    node->next = NULL;
    return node;
}

/**
 * @brief Освободить весь строковый список начиная с узла @p head.
 *
 * Проходит список от начала до конца, освобождая сначала строку каждого
 * узла, затем сам узел. Безопасен при передаче NULL (ничего не делает).
 *
 * @param head  Первый узел списка (может быть NULL).
 */
static void str_list_free(str_node_t* head) 
{
    while (head) {
        str_node_t* next = head->next; /* сохраняем перед освобождением */
        free(head->value);
        free(head);
        head = next;
    }
}

/**
 * @brief Добавить новое строковое значение в конец списка.
 *
 * Если список пуст, новый узел становится головой.
 * Иначе функция проходит до последнего узла и присоединяет новый.
 * Добавление O(n); допустимо, так как под-списки короткие.
 *
 * @param head_ptr  Адрес указателя на голову списка (меняется если список был пуст).
 * @param value     Строка для копирования и добавления.
 */
static void str_list_append(str_node_t** head_ptr, const char* value) 
{
    str_node_t* new_node = str_node_create(value);

    if (!*head_ptr) {
        /* список был пуст — новый узел становится головой */
        *head_ptr = new_node;
        return;
    }

    /* идём до последнего узла */
    str_node_t* cursor = *head_ptr;
    while (cursor->next) {
        cursor = cursor->next;
    }
    cursor->next = new_node;
}

/**
 * @brief Удалить узел с 1-индексом @p index из строкового списка.
 *
 * Для головы (index == 1) обновляется сам указатель на голову.
 * Для остальных позиций находится предшественник и перешивается его next.
 * Удалённый узел и его строка освобождаются.
 *
 * Логика цикла: перемещаемся ровно (index - 2) раз, чтобы остановиться
 * на узле-предшественнике (index == 2 → цикл не выполняется ни разу,
 * predecessor остаётся головой — это корректно).
 *
 * @param head_ptr  Адрес указателя на голову списка.
 * @param index     1-индекс узла для удаления.
 * @return          1 при успехе, 0 если @p index за пределами списка.
 */
static int str_list_remove_at(str_node_t** head_ptr, int index) 
{
    if (!*head_ptr || index < 1) return 0;

    if (index == 1) {
        /* удаляем голову, сдвигаем указатель */
        str_node_t* to_delete = *head_ptr;
        *head_ptr = to_delete->next;
        free(to_delete->value);
        free(to_delete);
        return 1;
    }

    /* ищем узел, стоящий непосредственно перед целевым.
       цикл выполняется (index-2) раз: при index==2 условие i<1 ложно сразу,
       predecessor остаётся на голове — это именно предшественник элемента №2. */
    str_node_t* predecessor = *head_ptr;
    for (int i = 1; i < index - 1 && predecessor->next; i++) {
        predecessor = predecessor->next;
    }

    if (!predecessor->next) return 0; /* index за пределами списка */

    str_node_t* to_delete = predecessor->next;
    predecessor->next = to_delete->next; /* вырезаем узел из цепочки */
    free(to_delete->value);
    free(to_delete);
    return 1;
}

/* ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ДЛЯ КОНТАКТА */

/**
 * @brief Вернуть копию строки @p src, выделенную на куче.
 *
 * Простая обёртка над malloc + strcpy.
 * При нехватке памяти завершает программу через perror + exit.
 * Вызывающая сторона владеет результатом и обязана его освободить.
 *
 * @param src  Ненулевая строка для копирования.
 * @return     Указатель на новую копию.
 */
static char* string_duplicate(const char* src) 
{
    char* copy = malloc(strlen(src) + 1);
    if (!copy) { perror("malloc"); exit(EXIT_FAILURE); }
    strcpy(copy, src);
    return copy;
}

/**
 * @brief Перезаписать char*-поле контакта, освободив предыдущее значение.
 *
 * Если @p new_value равно NULL или пустой строке — поле обнуляется (NULL).
 * Иначе старое значение освобождается и сохраняется копия @p new_value.
 * Такой подход делает «редактирование на месте» безопасным:
 * нет двойного освобождения, нет утечек.
 *
 * @param field_ptr  Адрес char*-поля для обновления.
 * @param new_value  Новая строка (NULL или "" очищает поле).
 */
static void field_set(char** field_ptr, const char* new_value) 
{
    free(*field_ptr); /* free(NULL) — легальная операция, ничего не делает */
    *field_ptr = (new_value && new_value[0]) ? string_duplicate(new_value) : NULL;
}

/**
 * @brief Выделить и обнулить новый contact_t.
 *
 * calloc обнуляет все байты, поэтому каждое поле начинается как NULL,
 * а каждый под-список начинается пустым.
 * При нехватке памяти завершает программу через perror + exit.
 *
 * @return Указатель на новый контакт.
 */
static contact_t* contact_create(void) 
{
    contact_t* c = calloc(1, sizeof(contact_t));
    if (!c) { perror("calloc"); exit(EXIT_FAILURE); }
    return c;
}

/**
 * @brief Освободить контакт и всю принадлежащую ему память.
 *
 * Последовательно освобождает пять char*-полей (NULL-безопасно),
 * затем четыре строковых под-списка через str_list_free(),
 * и наконец сам contact_t.
 *
 * @param contact  Контакт для уничтожения (NULL-безопасно).
 */
static void contact_destroy(contact_t* contact) 
{
    if (!contact) return;

    /* скалярные строковые поля */
    free(contact->last_name);
    free(contact->first_name);
    free(contact->middle_name);
    free(contact->workplace);
    free(contact->position);

    /* под-списки */
    str_list_free(contact->phones);
    str_list_free(contact->emails);
    str_list_free(contact->socials);
    str_list_free(contact->messengers);

    free(contact);
}

/* ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ДЛЯ СПИСКА КОНТАКТОВ */

/**
 * @brief Выделить пустую телефонную книгу.
 *
 * При нехватке памяти завершает программу через perror + exit.
 *
 * @return Указатель на новый список с head = NULL и size = 0.
 */
static contact_list_t* contact_list_create(void) 
{
    contact_list_t* list = calloc(1, sizeof(contact_list_t));
    if (!list) { perror("calloc"); exit(EXIT_FAILURE); }
    return list;
}

/**
 * @brief Добавить контакт в конец телефонной книги.
 *
 * Выделяет новый contact_node_t, сохраняет в нём указатель на контакт
 * и присоединяет узел после текущего последнего. Если список пуст,
 * новый узел становится головой. Увеличивает list->size.
 * При нехватке памяти завершает программу через perror + exit.
 *
 * @param list     Телефонная книга для добавления.
 * @param contact  Контакт на куче; право владения передаётся списку.
 */
static void contact_list_append(contact_list_t* list, contact_t* contact) 
{
    contact_node_t* new_node = malloc(sizeof(contact_node_t));
    if (!new_node) { perror("malloc"); exit(EXIT_FAILURE); }
    new_node->contact = contact;
    new_node->next    = NULL;

    if (!list->head) {
        list->head = new_node;
    } else {
        /* идём до последнего узла */
        contact_node_t* cursor = list->head;
        while (cursor->next) cursor = cursor->next;
        cursor->next = new_node;
    }
    list->size++;
}

/**
 * @brief Вернуть контакт по 1-индексу или NULL, если индекс за пределами.
 *
 * Проходит список от головы, считая узлы. Сложность O(n).
 *
 * @param list   Телефонная книга.
 * @param index  1-индекс (1 … list->size).
 * @return       Указатель на контакт (список остаётся владельцем) или NULL.
 */
static contact_t* contact_list_get(contact_list_t* list, int index) 
{
    if (index < 1 || index > list->size) return NULL;

    contact_node_t* cursor = list->head;
    for (int i = 1; i < index; i++) cursor = cursor->next;
    return cursor->contact;
}

/**
 * @brief Удалить и уничтожить контакт по 1-индексу.
 *
 * Перешивает предшественника мимо удаляемого узла, освобождает узел
 * и его контакт, уменьшает list->size. При index == 1 обновляется
 * сам указатель на голову.
 *
 * @param list   Телефонная книга.
 * @param index  1-индекс позиции для удаления.
 * @return       1 при успехе, 0 если @p index за пределами списка.
 */
static int contact_list_remove_at(contact_list_t* list, int index) 
{
    if (index < 1 || index > list->size) return 0;

    contact_node_t* predecessor = NULL;
    contact_node_t* target      = list->head;

    /* движемся до нужного узла */
    for (int i = 1; i < index; i++) {
        predecessor = target;
        target      = target->next;
    }

    /* перешиваем ссылки */
    if (predecessor) predecessor->next = target->next;
    else             list->head        = target->next;

    contact_destroy(target->contact);
    free(target);
    list->size--;
    return 1;
}

/**
 * @brief Освободить всю телефонную книгу вместе со всеми контактами.
 *
 * Проходит список от головы до конца, уничтожая каждый контакт и
 * освобождая каждый узел. Затем освобождает сам contact_list_t.
 *
 * @param list  Телефонная книга для уничтожения.
 */
static void contact_list_destroy(contact_list_t* list) 
{
    contact_node_t* cursor = list->head;
    while (cursor) {
        contact_node_t* next = cursor->next;
        contact_destroy(cursor->contact);
        free(cursor);
        cursor = next;
    }
    free(list);
}

/* ВВОД/ВЫВОД */

/**
 * @brief Напечатать @p prompt и прочитать одну строку из stdin в @p buf.
 *
 * Использует fgets, поэтому буфер не может переполниться.
 * Обрезает завершающий перенос строки. При EOF buf[0] = '\0'.
 * Если введённая строка длиннее (buf_sz - 1) символов, очищает
 * хвост из stdin, чтобы следующий вызов не читал остатки.
 *
 * @param prompt  Строка-приглашение (выводится без автоматического '\n').
 * @param buf     Буфер назначения.
 * @param buf_sz  Размер @p buf в байтах (передаётся напрямую в fgets).
 */
static void read_line(const char* prompt, char* buf, int buf_sz) 
{
    printf("%s", prompt);
    fflush(stdout);

    if (!fgets(buf, buf_sz, stdin)) {
        buf[0] = '\0';
        return;
    }

    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') {
        /* нормальный случай: строка уместилась, убираем '\n' */
        buf[len - 1] = '\0';
    } else {
        /* строка длиннее буфера — очищаем хвост в stdin,
           иначе следующий fgets мгновенно прочитает остаток */
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF);
    }
}

/**
 * @brief Напечатать @p prompt и прочитать целое число из stdin.
 *
 * Использует strtol вместо atoi, чтобы отличить нечисловой ввод от нуля.
 * При нечисловом вводе (например, "abc") выводит предупреждение и
 * возвращает 0, не завершая программу неожиданным образом.
 *
 * @param prompt  Строка-приглашение.
 * @return        Разобранное целое или 0 при нечисловом вводе.
 */
static int read_int(const char* prompt) 
{
    char buf[32];
    read_line(prompt, buf, sizeof(buf));

    char* endptr;
    errno = 0;
    long value = strtol(buf, &endptr, 10);

    /* нечисловой ввод: endptr указывает на первый символ буфера */
    if (endptr == buf || (*endptr != '\0' && *endptr != '\n')) {
        puts("  Invalid input, treating as 0.");
        return 0;
    }
    return (int)value;
}

/**
 * @brief Интерактивное мини-меню для редактирования одного строкового под-списка.
 *
 * Отображает текущее содержимое под-списка с нумерацией 1…n.
 * Принимает однобуквенные команды:
 *   a — запросить новое значение и добавить в конец
 *   r — запросить индекс и удалить элемент
 *   d — завершить, вернуться к вызывающей стороне
 * Цикл продолжается до ввода 'd'.
 *
 * @param head_ptr  Адрес указателя на голову под-списка (меняется на месте).
 * @param label     Читаемое название, печатается как заголовок подменю.
 */
static void edit_str_list_interactive(str_node_t** head_ptr, const char* label) 
{
    char command[16];
    char value_buf[256];

    for (;;) {
        /* печатаем заголовок и текущее содержимое */
        printf("\n  -- %s --\n", label);
        str_node_t* cursor = *head_ptr;
        if (!cursor) puts("  (empty)");
        for (int i = 1; cursor; i++, cursor = cursor->next) {
            printf("  %d. %s\n", i, cursor->value);
        }

        puts("  a) add   r) remove   d) done");
        read_line("  > ", command, sizeof(command));

        if (command[0] == 'a') {
            read_line("  value: ", value_buf, sizeof(value_buf));
            if (value_buf[0]) {
                str_list_append(head_ptr, value_buf);
            } else {
                /* пустой ввод игнорируется — информируем пользователя */
                puts("  Empty value ignored.");
            }

        } else if (command[0] == 'r') {
            if (!*head_ptr) { puts("  Nothing to remove."); continue; }
            int idx = read_int("  index: ");
            if (!str_list_remove_at(head_ptr, idx)) puts("  Bad index.");

        } else if (command[0] == 'd') {
            break; /* выходим из мини-меню */
        }
    }
}

/**
 * @brief Заполнить или перезаписать все поля контакта в интерактивном режиме.
 *
 * Запрашивает у пользователя поля в следующем порядке:
 *   1. Фамилия  (обязательно — повторный запрос при пустом вводе)
 *   2. Имя      (обязательно — повторный запрос при пустом вводе)
 *   3. Отчество, место работы, должность (необязательно; Enter = пропустить)
 *   4. Четыре мини-меню для под-списков: телефоны, email, соцсети, мессенджеры
 *
 * При редактировании существующего контакта старые значения заменяются
 * новыми; под-списки редактируются инкрементно через мини-меню.
 * При EOF в обязательном поле выводит сообщение об ошибке и завершает программу.
 *
 * @param contact  Контакт для заполнения (может уже содержать данные).
 */
static void fill_contact_interactive(contact_t* contact) 
{
    char input_buf[256];

    /* обязательное поле — повторяем запрос пока не введут непустую строку.
       При EOF завершаем программу аварийно, чтобы не зациклиться. */
    for (;;) {
        read_line("  Last name*:  ", input_buf, sizeof(input_buf));
        if (input_buf[0]) break;
        if (feof(stdin)) {
            puts("\nError: unexpected EOF. Exiting.");
            exit(EXIT_FAILURE);
        }
        puts("  Field is required. Please try again.");
    }
    field_set(&contact->last_name, input_buf);

    for (;;) {
        read_line("  First name*: ", input_buf, sizeof(input_buf));
        if (input_buf[0]) break;
        if (feof(stdin)) {
            puts("\nError: unexpected EOF. Exiting.");
            exit(EXIT_FAILURE);
        }
        puts("  Field is required. Please try again.");
    }
    field_set(&contact->first_name, input_buf);

    /* необязательные скалярные поля */
    read_line("  Middle name  (Enter to skip): ", input_buf, sizeof(input_buf));
    field_set(&contact->middle_name, input_buf);

    read_line("  Workplace    (Enter to skip): ", input_buf, sizeof(input_buf));
    field_set(&contact->workplace, input_buf);

    read_line("  Position     (Enter to skip): ", input_buf, sizeof(input_buf));
    field_set(&contact->position, input_buf);

    /* под-списки — каждый в своём мини-меню */
    edit_str_list_interactive(&contact->phones,     "Phones");
    edit_str_list_interactive(&contact->emails,     "Emails");
    edit_str_list_interactive(&contact->socials,    "Social networks");
    edit_str_list_interactive(&contact->messengers, "Messengers");
}

/* ОТОБРАЖЕНИЕ */

/**
 * @brief Напечатать один строковый под-список на одной строке с меткой.
 *
 * Элементы разделяются " | ". При пустом списке ничего не печатается.
 *
 * @param label  Метка поля; печатается выровненной по левому краю в 16 символов.
 * @param head   Первый узел под-списка (NULL — ничего не делать).
 */
static void display_str_list(const char* label, str_node_t* head) 
{
    if (!head) return;

    printf("    %-16s", label);
    for (str_node_t* node = head; node; node = node->next) {
        printf("%s", node->value);
        if (node->next) printf(" | ");
    }
    putchar('\n');
}

/**
 * @brief Напечатать один контакт с его 1-индексом в списке.
 *
 * Формат вывода:
 *   [N] Фамилия Имя Отчество
 *       Workplace:      …
 *       Position:       …
 *       Phones:         … | …
 *       …
 *
 * Незаполненные поля (NULL) молча пропускаются.
 *
 * @param list_index  1-индекс в телефонной книге (выводится в скобках).
 * @param contact     Контакт для отображения.
 */
static void display_contact(int list_index, const contact_t* contact) 
{
    printf("\n  [%d] %s %s", list_index, contact->last_name, contact->first_name);
    if (contact->middle_name) printf(" %s", contact->middle_name);
    putchar('\n');

    if (contact->workplace) printf("    %-16s%s\n", "Workplace:", contact->workplace);
    if (contact->position)  printf("    %-16s%s\n", "Position:",  contact->position);

    display_str_list("Phones:",      contact->phones);
    display_str_list("Emails:",      contact->emails);
    display_str_list("Social nets:", contact->socials);
    display_str_list("Messengers:",  contact->messengers);
}

/**
 * @brief Напечатать все контакты телефонной книги.
 *
 * Проходит связный список от головы до конца, печатая каждый контакт
 * с его 1-индексом. При пустом списке выводит "(no contacts)".
 *
 * @param list  Телефонная книга для отображения.
 */
static void display_all_contacts(contact_list_t* list) 
{
    if (!list->size) {
        puts("  (no contacts)");
        return;
    }

    contact_node_t* cursor = list->head;
    for (int index = 1; cursor; index++, cursor = cursor->next) {
        display_contact(index, cursor->contact);
    }
    putchar('\n');
}

/* ТОЧКА ВХОДА */

/**
 * @brief Точка входа — главный цикл меню.
 *
 * Создаёт пустую телефонную книгу, затем в бесконечном цикле
 * показывает меню и обрабатывает выбор пользователя:
 *   1 — вывести все контакты
 *   2 — создать новый контакт и добавить в книгу
 *   3 — показать список, выбрать по индексу, отредактировать на месте
 *   4 — показать список, выбрать по индексу, удалить
 *   0 — уничтожить книгу и выйти
 *
 * @return 0 при нормальном завершении.
 */
int main(void) 
{
    contact_list_t* phone_book = contact_list_create();

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
            display_all_contacts(phone_book);
            break;

        case 2: {
            contact_t* new_contact = contact_create();
            puts("\n-- New contact (* required) --");
            fill_contact_interactive(new_contact);
            contact_list_append(phone_book, new_contact);
            puts("Contact added.");
            break;
        }

        case 3: {
            display_all_contacts(phone_book);
            if (!phone_book->size) break;

            int index = read_int("Index to edit: ");
            contact_t* target = contact_list_get(phone_book, index);
            if (!target) { puts("Bad index."); break; }

            puts("\n-- Edit contact (* required) --");
            fill_contact_interactive(target);
            puts("Contact updated.");
            break;
        }

        case 4: {
            display_all_contacts(phone_book);
            if (!phone_book->size) break;

            int index = read_int("Index to delete: ");
            if (contact_list_remove_at(phone_book, index)) puts("Contact deleted.");
            else                                            puts("Bad index.");
            break;
        }

        case 0:
            contact_list_destroy(phone_book);
            puts("Bye.");
            return 0;

        default:
            puts("Unknown option.");
        }
    }
}
