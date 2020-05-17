#include <gpio.h>
#include <keyboard.h>
#include <console.h>
#include <shell.h>

#define NUM_ROWS 20
#define NUM_COLS 40

void
main(void)
{
  gpio_init();
  console_init(NUM_ROWS, NUM_COLS);
  keyboard_init(KEYBOARD_CLOCK, KEYBOARD_DATA);

  shell_init(console_printf);
  shell_run();
}
