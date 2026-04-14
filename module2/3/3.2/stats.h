#ifndef STATS_H
#define STATS_H

typedef struct {
    int total;
    int same_subnet;
    int other_subnet;
} route_stats_t;

void stats_init(route_stats_t *s);
void stats_record(route_stats_t *s, int is_same);
void stats_print(const route_stats_t *s);

#endif // STATS_H
