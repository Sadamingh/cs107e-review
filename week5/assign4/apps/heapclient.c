#include <printf.h>
#include <uart.h>

void main(void) {
  printf("This is our heap client");
  uart_putchar(EOT);
}
