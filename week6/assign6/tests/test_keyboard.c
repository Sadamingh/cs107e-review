#include <assert.h>
#include <keyboard.h>
#include <printf.h>
#include <uart.h>
#include <timer.h>

void
main(void) {
  uart_init();
  keyboard_init(KEYBOARD_CLOCK, KEYBOARD_DATA);
}
