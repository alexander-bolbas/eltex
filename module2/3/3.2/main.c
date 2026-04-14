#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ipv4.h"
#include "stats.h"

// Главная функция
int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Использование: %s <шлюз> <маска> <N>\n", argv[0]);
        fprintf(stderr, "Пример: %s 192.168.1.1 255.255.255.0 10000\n", argv[0]);
        return 1;
    }
    
    ipv4_addr_t gateway, mask;
    
    if (ipv4_parse(argv[1], &gateway) != 0) {
        fprintf(stderr, "Error: invalid gateway IP: %s\n", argv[1]);
        return 1;
    }
    
    if (ipv4_parse(argv[2], &mask) != 0) {
        fprintf(stderr, "Error: invalid subnet mask: %s\n", argv[2]);
        return 1;
    }
    
    if (!ipv4_is_valid_mask(mask)) {
        fprintf(stderr, "Error: invalid subnet mask: %s\n", argv[2]);
        return 1;
    }
    
    int n = atoi(argv[3]);
    if (n <= 0) {
        fprintf(stderr, "Error: N must be > 0\n");
        return 1;
    }
    
    srand((unsigned)time(NULL));
    route_stats_t stats;
    stats_init(&stats);
    
    char gw_str[16], mask_str[16];
    ipv4_to_string(gateway, gw_str);
    ipv4_to_string(mask, mask_str);
    
    printf("Gateway: %s\n", gw_str);
    printf("Subnet mask: %s\n", mask_str);
    printf("Generating %d packets...\n", n);
    
    // Симуляция обработки пакетов
    for (int i = 0; i < n; i++) {
        // Генерируем случайный IP адрес назначения
        ipv4_addr_t dest = ipv4_random();
        // Проверяем, принадлежит ли адрес подсети шлюза
        int is_same = ipv4_same_subnet(dest, gateway, mask);
        // Записываем результат в статистику
        stats_record(&stats, is_same);
    }
    
    // Выводим статистику
    stats_print(&stats);
    return 0;
}
