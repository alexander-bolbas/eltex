#include "phone_book.h"
#include "ui.h"

int main(void) {
    phone_book_t phone_book;
    pb_init(&phone_book);
    ui_run_menu(&phone_book);
    return 0;
}