#include "../backtrace.h"
#include <uart.h>

int
backtrace_3(int a) {
  int b = a * 7 - 5;

  if(b > 3) {
    b -= 2;
  }

  print_backtrace();

  return b;
}

int
backtrace_2(int a) {
  int b = a * 100;
  if(b > 20) {
    b -= 20;
  }
  return backtrace_3(b);
}

void
backtrace_1(int a) {
  int b = a * 3 + 1;
  if(b > a + 100) {
    b -= 100;
  }

  backtrace_2(b);
}

void
test_backtrace(void) {
  backtrace_1(100);
}

void
main(void) {
  test_backtrace();

  uart_putchar(EOT);
}
