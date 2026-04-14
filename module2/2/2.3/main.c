#include "cmd_registry.h"
#include "ops.h"
#include "ui.h"

int main(void) {
    cmd_registry_t registry;
    registry_init(&registry);

    registry_register(&registry, (calc_cmd_t){"add", "Addition", 2, MAX_ARGS, op_add});
    registry_register(&registry, (calc_cmd_t){"sub", "Subtraction", 2, MAX_ARGS, op_sub});
    registry_register(&registry, (calc_cmd_t){"mul", "Multiplication", 2, MAX_ARGS, op_mul});
    registry_register(&registry, (calc_cmd_t){"div", "Division", 2, MAX_ARGS, op_div});
    registry_register(&registry, (calc_cmd_t){"pow", "Power", 2, 2, op_pow});
    registry_register(&registry, (calc_cmd_t){"sqrt", "Square Root", 1, 1, op_sqrt});

    ui_run_calculator(&registry);
    return 0;
}