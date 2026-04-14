#ifndef CMD_REGISTRY_H
#define CMD_REGISTRY_H

#include "types.h"

typedef struct {
    calc_cmd_t cmds[MAX_CMDS];
    int count;
} cmd_registry_t;

void registry_init(cmd_registry_t *r);
int registry_register(cmd_registry_t *r, calc_cmd_t cmd);
const calc_cmd_t *registry_find(const cmd_registry_t *r, const char *name);
void registry_print_menu(const cmd_registry_t *r);

#endif // CMD_REGISTRY_H