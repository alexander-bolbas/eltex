#ifndef PERMS_H
#define PERMS_H

#include "types.h"

// Парсит строку "755" или "rwxr-xr--"
int perms_parse_string(const char *str, perms_t *out);

// Конвертирует в символьное представление, буфер должен быть >= 10 байт
void perms_to_symbolic(perms_t mode, char buf[10]);

// Конвертирует в восьмеричное число
int perms_to_octal(perms_t mode);

// Конвертирует в двоичное представление, буфер должен быть >= 10 байт
void perms_to_binary(perms_t mode, char buf[10]);

// Применяет модификатор вида "u+x,g-w,o=r"
int perms_apply_modifier(perms_t current, const char *modifier, perms_t *out);

#endif // PERMS_H
