#ifndef TYPES_H
#define TYPES_H

#define MAX_ARGS   10
#define MAX_CMDS   32

// тип функции-операции: возвращает результат double
typedef double (*calc_op_fn_t)(int argc, double argv[]);

// одна команда = имя + стратегия
typedef struct {
    const char    *name;
    const char    *description;
    int            min_args;
    int            max_args;
    calc_op_fn_t   fn;
} calc_cmd_t;

#endif // TYPES_H