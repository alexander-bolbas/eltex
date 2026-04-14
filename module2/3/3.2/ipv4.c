#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ipv4.h"

// Парсит строку в формате "A.B.C.D" в 32-битное число IPv4 адреса
int ipv4_parse(const char *str, ipv4_addr_t *out) {
    unsigned int a, b, c, d;
    int end_pos;
    
    // Читаем четыре числа разделенные точками
    if (sscanf(str, "%u.%u.%u.%u%n", &a, &b, &c, &d, &end_pos) != 4)
        return -1; // Ошибка парсинга
    
    // Проверяем что это конец строки
    if (str[end_pos] != '\0') 
        return -1; // Лишние символы в строке
    
    // Проверяем что каждый октет не превышает 255
    if (a > 255 || b > 255 || c > 255 || d > 255) 
        return -1; // Октет выходит за границы
    
    // Преобразуем в 32-битное представление: старший октет в MSB
    *out = ((ipv4_addr_t)a << 24) | ((ipv4_addr_t)b << 16) | 
           ((ipv4_addr_t)c << 8) | (ipv4_addr_t)d;
    return 0;
}

// Проверяет является ли маска подсети корректной (например 255.255.255.0)
int ipv4_is_valid_mask(ipv4_addr_t mask) {
    // Корректная маска: начинается с единиц, потом только нули (нет перемешивания)
    uint32_t inv = ~mask; // Инвертируем маску
    // Проверяем что (inv & (inv+1)) == 0 (классический алгоритм проверки)
    return (inv & (inv + 1)) == 0;
}

// Проверяет находятся ли два IP адреса в одной подсети
int ipv4_same_subnet(ipv4_addr_t ip, ipv4_addr_t gw, ipv4_addr_t mask) {
    // Сравниваем сетевые части адресов: если (ip & mask) == (gw & mask), то в одной подсети
    return (ip & mask) == (gw & mask);
}

// Генерирует случайный 32-битный IP адрес
ipv4_addr_t ipv4_random(void) {
    // Объединяем два 16-битных случайных числа в один 32-битный адрес
    return ((ipv4_addr_t)(rand() & 0xFFFF) << 16) | (rand() & 0xFFFF);
}

// Преобразует 32-битный IP адрес обратно в строку вида "A.B.C.D"
void ipv4_to_string(ipv4_addr_t ip, char buf[16]) {
    // Извлекаем каждый из четырех октетов и выводим в буфер
    snprintf(buf, 16, "%u.%u.%u.%u",
             (ip >> 24) & 0xFF, (ip >> 16) & 0xFF, // старшие октеты
             (ip >>  8) & 0xFF,  ip        & 0xFF); // младшие октеты
}
