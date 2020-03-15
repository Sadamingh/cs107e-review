#include "printf_internal.h"
#include "printf.h"

#define MAX(x, y) ((x) <= (y) ? y: x)

int number_to_base(
  char *buf,
  size_t bufsize,
  unsigned int val,
  int base,
  int min_width,
  int negative
) {
  int result = 0;
  char output[MAX(sizeof(int), min_width)];
  int cursor = 0;

  if(val == 0) {
    output[cursor++] = '0';
  }
  while(val != 0) {
    int v = val % base;

    if(v < 10) {
      output[cursor++] = '0' + v;
    } else {
      output[cursor++] = 'a' + v - 10;
    }

    val = val / base;
  }

  int limit = min_width - cursor;
  if(negative) {
    limit--;
  }
  for(int i = 0; i < limit; i++) {
    output[cursor++] = '0';
  }

  if(negative) {
    output[cursor++] = '-';
  }

  result = cursor;

  for(int i = 0; i + 1 < bufsize && cursor > 0; i++) {
    *buf++ = output[--cursor];
  }

  if(bufsize > 0) {
    *buf = 0;
  }

  return result;
}

int unsigned_to_base(
  char *buf,
  size_t bufsize,
  unsigned int val,
  int base,
  int min_width
) {
  return number_to_base(
    buf, bufsize,
    val, base, min_width,
    0
  );
}

int signed_to_base(
  char *buf,
  size_t bufsize,
  int val,
  int base,
  int min_width
) {
  return number_to_base(
    buf, bufsize,
    val < 0 ? -val : val, base, min_width,
    val < 0 ? 1 : 0
  );
}
