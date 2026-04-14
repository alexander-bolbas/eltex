#include "dlist.h"
#include "ui.h"

int main(void) {
    dlist_t dlist;
    dlist_init(&dlist);
    
    ui_run_menu(&dlist);
    
    dlist_free(&dlist);
    return 0;
}
