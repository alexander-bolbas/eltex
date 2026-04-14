#ifndef TYPES_H
#define TYPES_H

#define PQ_MAX_PRIORITY 256

typedef struct {
    int data;
    int priority;
} msg_t;

typedef struct pq_node pq_node_t;
struct pq_node {
    msg_t      msg;
    pq_node_t *next;
};

typedef struct {
    pq_node_t *head;
    pq_node_t *tail;
} pq_level_t;

typedef struct {
    pq_level_t levels[PQ_MAX_PRIORITY];
    int total_count;
} pq_t;

#endif // TYPES_H
