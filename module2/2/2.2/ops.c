#include <math.h>
#include "ops.h"

int op_add(double a, double b, double *out) {
    *out = a + b;
    return 0;
}

int op_sub(double a, double b, double *out) {
    *out = a - b;
    return 0;
}

int op_mul(double a, double b, double *out) {
    *out = a * b;
    return 0;
}

int op_div(double a, double b, double *out) {
    if (b == 0.0) return -1;
    *out = a / b;
    return 0;
}

int op_pow(double a, double b, double *out) {
    *out = pow(a, b);
    return 0;
}

// Извлечение квадратного корня
int op_sqrt(double a, double b, double *out) {
    (void)b; // Параметр b не используется (требуется для единообразия сигнатуры)
    if (a < 0.0) return -1; // Ошибка: квадратный корень из отрицательного числа
    *out = sqrt(a);
    return 0;
}