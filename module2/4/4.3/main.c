#include "avl.h"
#include "ui.h"

int main(void) {
    avl_tree_t tree;
    avl_init(&tree);
    
    ui_run_menu(&tree);
    
    avl_free(&tree);
    return 0;
}
