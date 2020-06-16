#include "gl.h"
#include "gpio.h"
#include "gpioextra.h"
#include "interrupts.h"
#include "printf.h"
#include "ringbuffer.h"
#include "uart.h"

static const int BUTTON = GPIO_PIN20;
static int gCount = 0;
static rb_t *rb;

static bool
button_pressed(unsigned int pc)
{
  if(gpio_check_and_clear_event(BUTTON)) {
    printf("![%d]", gCount);
    rb_enqueue(rb, gCount);
    gCount++;
    return true;
  }

  return false;
}

void
configure_button(void)
{
  gpio_set_input(BUTTON);
  gpio_set_pullup(BUTTON);
  gpio_enable_event_detection(BUTTON, GPIO_DETECT_FALLING_EDGE);
  interrupts_attach_handler(button_pressed, INTERRUPTS_GPIO3);
}

void
redraw(void)
{
  static int nrefresh = 0;
  color_t cycle[3] = { GL_RED, GL_WHITE, GL_BLUE };
  color_t c = cycle[nrefresh % 3];

  printf("Redraw iteration #%d\n", nrefresh++);
  for(int y = 0; y < gl_get_height(); y++) {
    for(int x = 0; x < gl_get_width(); x++) {
      gl_draw_pixel(x, y, c);
    }
  }
}

void
main(void)
{
  gpio_init();
  uart_init();
  gl_init(640, 480, GL_SINGLEBUFFER);
  interrupts_init();

  configure_button();

  interrupts_global_enable();

  rb = rb_new();

  int value = 0;

  while(1) {
    if(rb_dequeue(rb, &value)) {
      redraw();
    }
  }
}
