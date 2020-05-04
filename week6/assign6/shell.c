#include <shell.h>

static formatted_fn_t shell_printf;

void
shell_init(formatted_fn_t print_fn) {
  shell_printf = print_fn;
}

void
shell_run(void) {
}
