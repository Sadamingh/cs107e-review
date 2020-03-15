#include "timer.h"
#include "strings.h"
#define assert(x) if(!(x)) abort()

#define GPIO_FSEL3  ((unsigned int *)0x2020000c)
#define GPIO_SET1   ((unsigned int *)0x20200020)
#define GPIO_CLR1   ((unsigned int *)0x2020002c)

// Red power LED (on Pi board) is GPIO 35.
#define ABORT_OUTPUT (1 << (3*5))
#define ABORT_BIT    (1 << (35-32))

void abort(void) {
  *GPIO_FSEL3 = ABORT_OUTPUT;
  while (1) {
    *GPIO_SET1 = ABORT_BIT;
    timer_delay_ms(100);
    *GPIO_CLR1 = ABORT_BIT;
    timer_delay_ms(100);
  }
}

void test_strings() {
  // memset
  {
    char buf[10];
    void *result = memset(buf, 'a', 5);
    assert(result == buf);
    for(int i = 0; i < 5; i++) {
      assert(buf[i] == 'a');
    }
  }

  // memcpy
  {
    char dst[10];
    char src[10] = "abcdefg";

    {
      void *result = memcpy(dst, src, 1);
      assert(result == dst);
      assert(dst[0] = 'a');
    }

    {
      void *result = memcpy(dst + 1, src + 1, 3);
      assert(result == dst + 1);
      assert(dst[1] = 'b');
      assert(dst[2] = 'c');
      assert(dst[3] = 'd');
    }
  }

  // strlen
  {
    assert(strlen("") == 0);
    assert(strlen("abc") == 3);
  }

  // strcmp
  {
    assert(strcmp("", "") == 0);
    assert(strcmp("a", "a") == 0);
    assert(strcmp("abcd", "abcd") == 0);

    assert(strcmp("a", "b") < 0);
    assert(strcmp("hello a", "hello b") < 0);
    assert(strcmp("b", "a") > 0);
    assert(strcmp("hello b", "hello a") > 0);

    char buf1[10] = {};
    char buf2[10] = {};

    buf1[0] = 10;
    buf2[0] = 20;
    assert(strcmp(buf1, buf2) < 0);

    buf1[0] = 30;
    buf2[0] = 20;
    assert(strcmp(buf1, buf2) > 0);

    buf1[0] = -1;
    buf2[0] = 20;
    assert(strcmp(buf1, buf2) > 0);

    buf1[0] = 1;
    buf2[0] = -2;
    assert(strcmp(buf1, buf2) < 0);

    buf1[0] = -1;
    buf2[0] = -2;
    assert(strcmp(buf1, buf2) > 0);
  }

  // strtonum
  {
    const char *endptr;
    unsigned int value;

    char *src = "123";
    value = strtonum(src, &endptr);
    assert(value == 123 && endptr == src + 3);

    src = "0x60";
    value = strtonum(src, &endptr);
    assert(value == 96 && endptr == src + 4);

    src = "0xabc";
    value = strtonum(src, &endptr);
    assert(value == 2748 && endptr == src + 5);

    src = "abcd";
    value = strtonum(src, &endptr);
    assert(value == 0 && endptr == src + 0);

    src = " 123";
    value = strtonum(src, &endptr);
    assert(value == 0 && endptr == src + 0);

    src = "0X60";
    value = strtonum(src, &endptr);
    assert(value == 0 && endptr == src + 1);

    src = "0xAB";
    value = strtonum(src, &endptr);
    assert(value == 0 && endptr == src + 2);

    src = "+100";
    value = strtonum(src, &endptr);
    assert(value == 0 && endptr == src);

    src = "100";
    value = strtonum(src, NULL);
    assert(value == 100);
  }

  // strlcat
  {
    size_t result;

    {
      char dst[10] = {};
      dst[0] = 'x';
      result = strlcat(dst, "hello", 10);
      assert(result == 6);
      assert(strcmp(dst, "xhello") == 0);
    }

    {
      char dst[10] = {};
      dst[0] = 'a';
      dst[1] = 'b';
      result = strlcat(dst, "xy", 10);
      assert(result == 4);
      assert(strcmp(dst, "abxy") == 0);
    }

    {
      char dst[10] = {};
      dst[0] = 'x';
      char *src = "too much string";
      result = strlcat(dst, src, 10);
      assert(result == strlen(src) + 1);
      assert(strcmp(dst, "xtoo much") == 0);
    }

    {
      char dst[10] = {};
      dst[0] = 'x';
      char *src = "a string";
      result = strlcat(dst, src, 0);
      assert(result == strlen(src) + 1);
      assert(strcmp(dst, "x") == 0);
    }
  }
}

void main(void) {
  test_strings();
}
