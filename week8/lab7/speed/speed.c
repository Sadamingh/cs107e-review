/* File speed.c
 * -----------
 * Simple main program with timing code to measure
 * performance of different implementations of redraw.
 */

#include "gl.h"
#include "fb.h"
#include "printf.h"
#include "timer.h"
#include "uart.h"

// -------------------------------------------------------------------
// Time Trial Helpers
// -------------------------------------------------------------------

unsigned
_time_trial(void (*fn)(color_t c))
{
  static int nrefresh = 0;
  color_t cycle[3] = { GL_RED, GL_WHITE, GL_BLUE };
  color_t c = cycle[nrefresh++ % 3];

  gl_clear(GL_BLACK);
  unsigned start = timer_get_ticks();
  fn(c);
  unsigned elapsed = timer_get_ticks() - start;
  return elapsed;
}

#define TIME_TRIAL(fn)                                                         \
  do {                                                                         \
    printf("Running " #fn "... ");                                             \
    printf("took %d ticks\n", _time_trial(fn));                                \
  } while(0)

// -------------------------------------------------------------------
// Baseline redraw0, correct implementation but pokey
// -------------------------------------------------------------------

void
redraw0(color_t c)
{
  for(int y = 0; y < gl_get_height(); y++) {
    for(int x = 0; x < gl_get_width(); x++) {
      gl_draw_pixel(x, y, c);
    }
  }
}

// -------------------------------------------------------------------
// Wicked fast redraw functions of your own go here:
// -------------------------------------------------------------------

void
redraw1(color_t c)
{
  int height = gl_get_height();
  int width = gl_get_width();

  for(int y = 0; y < height; y++) {
    for(int x = 0; x < width; x++) {
      gl_draw_pixel(x, y, c);
    }
  }
}

void
redraw2(color_t c)
{
  int height = gl_get_height();
  int width = gl_get_width();

  int pitch = fb_get_pitch() / 4;
  unsigned int(*arr)[pitch] = fb_get_draw_buffer();

  for(int y = 0; y < height; y++) {
    for(int x = 0; x < width; x++) {
      arr[y][x] = c;
    }
  }
}

void
redraw3(color_t c)
{
  int height = gl_get_height();
  int width = gl_get_width();

  int pitch = fb_get_pitch() / 4;
  unsigned int(*arr)[pitch] = fb_get_draw_buffer();

  for(int x = 0; x < width; x++) {
    for(int y = 0; y < height; y++) {
      arr[y][x] = c;
    }
  }
}

void
redraw4(color_t c)
{
  int total = gl_get_height() * gl_get_width();

  unsigned int *arr = fb_get_draw_buffer();

  for(int i = 0; i < total; i++) {
    arr[i] = c;
  }
}

void
redraw5(color_t c)
{
  int total = gl_get_height() * gl_get_width();

  unsigned int *p = fb_get_draw_buffer();

  for(int i = 0; i < total; i++) {
    *p++ = c;
  }
}

void
redraw6(color_t c)
{
  int total = gl_get_height() * gl_get_width();

  unsigned int *p = fb_get_draw_buffer();

  for(int i = total; i != 0; i--) {
    *p++ = c;
  }
}

void
main(void)
{
  timer_init();
  uart_init();
  gl_init(640, 480, GL_SINGLEBUFFER);

  printf("\nStarting time trials now.\n");

  // basic case
  // 1224996
  // TIME_TRIAL(redraw0);

  // move unnecessary function calls outside of loop
  // 1025915
  // TIME_TRIAL(redraw1);

  // fill buffer directly instead of calling `gl_draw_pixel`
  // 188602
  // TIME_TRIAL(redraw2);

  // same as above but loop column by row
  // 186712
  // TIME_TRIAL(redraw3);

  // loop over the pixels as a 1-d array
  // 166351
  // TIME_TRIAL(redraw4);

  // use a pointer instead of calculating target address
  // 148687
  // TIME_TRIAL(redraw5);

  // -Og
  // 40875
  // TIME_TRIAL(redraw5);

  // -Ofast
  // 38932
  // TIME_TRIAL(redraw5);

  // -O2
  // 38928
  // TIME_TRIAL(redraw5);

  // -O2
  // use a decrementing loop
  // 25091
  TIME_TRIAL(redraw6);

  printf("\nAll done with time trials.\n");
  uart_putchar(EOT);
}
