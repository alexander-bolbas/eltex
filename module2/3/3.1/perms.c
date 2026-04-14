#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/stat.h>
#include "perms.h"

// Таблица битов для прав доступа: r w x r w x r w x (пользователь группа остальные)
static const mode_t k_bit_table[9] = {
    S_IRUSR, S_IWUSR, S_IXUSR,  // Права пользователя: чтение, запись, исполнение
    S_IRGRP, S_IWGRP, S_IXGRP,  // Права группы
    S_IROTH, S_IWOTH, S_IXOTH,  // Права остальных
};

// Символы для представления прав: r=чтение, w=запись, x=исполнение
static const char k_char_table[9] = "rwxrwxrwx";

void perms_to_symbolic(perms_t mode, char buf[10]) {
    if (!buf) return;
    for (int i = 0; i < 9; i++)
        buf[i] = (mode & k_bit_table[i]) ? k_char_table[i] : '-';
    buf[9] = '\0';
}

void perms_to_binary(perms_t mode, char buf[10]) {
    if (!buf) return;
    for (int i = 0; i < 9; i++)
        buf[i] = (mode & k_bit_table[i]) ? '1' : '0';
    buf[9] = '\0';
}

int perms_to_octal(perms_t mode) {
    return (((mode >> 6) & 7) * 100) + (((mode >> 3) & 7) * 10) + (mode & 7);
}

// Парсит строку с правами в восьмеричном или символьном формате
int perms_parse_string(const char *str, perms_t *out) {
    if (!str || !out) return -1;
    
    size_t len = strlen(str);
    
    // Числовой формат: "755" (восьмеричное представление)
    if (len == 3 && isdigit(str[0]) && isdigit(str[1]) && isdigit(str[2])) {
        unsigned int val;
        if (sscanf(str, "%o", &val) != 1) return -1;
        *out = (mode_t)(val & 0777); // Маска для сохранения только битов прав (0-777 в восьмеричной)
        return 0;
    }
    
    // Символьный формат: "rwxr-xr--" (символьное представление)
    if (len == 9) {
        mode_t m = 0;
        for (int i = 0; i < 9; i++) {
            if (str[i] == k_char_table[i])
                m |= k_bit_table[i]; // Устанавливаем бит если символ совпадает
            else if (str[i] != '-')
                return -1;  // Ошибка: неверный символ (не совпадает ни с правом ни с '-')
        }
        *out = m;
        return 0;
    }
    
    return -1;
}

// Применяет один модификатор токена вида "ug+rx" к текущим правам
// Поддерживает операторы: + (добавить), - (удалить), = (установить)
static int apply_one_token(perms_t *mode, const char *token) {
    if (!token || !mode) return -1;
    
    // Ищем оператор (+, -, =)
    const char *op_ptr = strpbrk(token, "+-=");
    if (!op_ptr) return -1;
    
    char op = *op_ptr;
    
    // Кто получит права: u(пользователь), g(группа), o(остальные), a(все)
    char who[5] = "a";
    size_t who_len = op_ptr - token;
    
    if (who_len > 0 && who_len < sizeof(who)) {
        strncpy(who, token, who_len);
        who[who_len] = '\0';
    }
    
    // Какие права: r(чтение), w(запись), x(исполнение)
    const char *perms_str = op_ptr + 1;
    if (!perms_str[0]) return -1;  // Ошибка: нет прав после оператора
    
    // Для оператора '=' нужно сначала сбросить все права этой категории
    if (op == '=') {
        for (const char *w = who; *w; w++) {
            if (*w == 'u' || *w == 'a') {
                // Очищаем права пользователя
                *mode &= ~(S_IRUSR | S_IWUSR | S_IXUSR);
            }
            if (*w == 'g' || *w == 'a') {
                // Очищаем права группы
                *mode &= ~(S_IRGRP | S_IWGRP | S_IXGRP);
            }
            if (*w == 'o' || *w == 'a') {
                // Очищаем права остальных
                *mode &= ~(S_IROTH | S_IWOTH | S_IXOTH);
            }
        }
        op = '+';  // После сброса работаем как '+' (добавляем новые права)
    }
    
    // Применяем каждую перм к каждому who
    for (const char *w = who; *w; w++) {
        for (const char *p = perms_str; *p; p++) {
            mode_t bit = 0;
            
            if (*w == 'u' || *w == 'a') {
                if (*p == 'r') bit |= S_IRUSR;
                if (*p == 'w') bit |= S_IWUSR;
                if (*p == 'x') bit |= S_IXUSR;
            }
            if (*w == 'g' || *w == 'a') {
                if (*p == 'r') bit |= S_IRGRP;
                if (*p == 'w') bit |= S_IWGRP;
                if (*p == 'x') bit |= S_IXGRP;
            }
            if (*w == 'o' || *w == 'a') {
                if (*p == 'r') bit |= S_IROTH;
                if (*p == 'w') bit |= S_IWOTH;
                if (*p == 'x') bit |= S_IXOTH;
            }
            
            if (op == '+') 
                *mode |= bit;
            else if (op == '-') 
                *mode &= ~bit;
        }
    }
    
    return 0;
}

int perms_apply_modifier(perms_t current, const char *modifier, perms_t *out) {
    if (!modifier || !out) return -1;
    
    perms_t m = current;
    char buf[256];
    strncpy(buf, modifier, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';
    
    char *token = strtok(buf, ",");
    while (token) {
        if (apply_one_token(&m, token) != 0) return -1;
        token = strtok(NULL, ",");
    }
    
    *out = m;
    return 0;
}
