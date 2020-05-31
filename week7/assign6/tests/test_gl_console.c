#include <uart.h>
#include <timer.h>
#include <fb.h>
#include <gl.h>
#include <console.h>

#define WIDTH 1920
#define HEIGHT 1080
#define Pi 3.1415926
#define Minimum(a, b) ((a) < (b) ? (a) : (b))

void gl_draw_line_with_width(int x1,
  int y1,
  int x2,
  int y2,
  float line_width,
  color_t c);

void gl_draw_triangle_with_width(int x1,
  int y1,
  int x2,
  int y2,
  int x3,
  int y3,
  float line_width,
  color_t c);

void
test_fb_singlebuffer()
{
  fb_init(WIDTH, HEIGHT, 4, FB_SINGLEBUFFER);
  int pitch = fb_get_pitch() / 4;
  int width = fb_get_width();
  int height = fb_get_height();
  unsigned int *buf = (unsigned int *)fb_get_draw_buffer();

  for(int y = 0; y < height; y++) {
    for(int x = 0; x < width; x++) {
      buf[y * pitch + x] = 0xff00ff00; // green
    }
  }

  for(int y = 0; y < height; y++) {
    for(int x = 0; x < width; x++) {
      buf[y * pitch + x] = 0xff0000ff; // blue
    }
  }
}

void
test_fb_doublebuffer()
{
  fb_init(WIDTH, HEIGHT, 4, FB_DOUBLEBUFFER);

  int pitch = fb_get_pitch() / 4;
  int width = fb_get_width();
  int height = fb_get_height();

  unsigned int *buf = (unsigned int *)fb_get_draw_buffer();
  for(int y = 0; y < height; y++) {
    for(int x = 0; x < width; x++) {
      buf[y * pitch + x] = 0xff00ff00; // green
    }
  }
  fb_swap_buffer();

  buf = (unsigned int *)fb_get_draw_buffer();
  for(int y = 0; y < height; y++) {
    for(int x = 0; x < width; x++) {
      buf[y * pitch + x] = 0xff0000ff; // blue
    }
  }
  fb_swap_buffer();
}

void
test_gl_simple(void)
{
  gl_init(WIDTH, HEIGHT, GL_SINGLEBUFFER);

  gl_clear(GL_WHITE);

  timer_delay(1);
  gl_draw_rect(0, 0, 100, 100, GL_BLUE);

  timer_delay(1);
  gl_draw_rect(0, 100, 100, 100, GL_RED);

  timer_delay(1);
  gl_draw_rect(200, 200, 100, 100, GL_YELLOW);
}

void
test_gl_doublebuffer(void)
{
  gl_init(WIDTH / 2, HEIGHT / 2, GL_DOUBLEBUFFER);

  {
    gl_clear(GL_WHITE);
    gl_swap_buffer();
  }

  {
    timer_delay(1);
    gl_clear(GL_WHITE);
    gl_draw_rect(0, 0, 100, 100, GL_BLUE);
    gl_swap_buffer();
  }

  {
    timer_delay(1);
    gl_clear(GL_WHITE);
    gl_draw_rect(0, 100, 100, 100, GL_RED);
    gl_swap_buffer();
  }

  {
    timer_delay(1);
    gl_clear(GL_WHITE);
    gl_draw_rect(200, 200, 100, 100, GL_YELLOW);
    gl_swap_buffer();
  }
}

#define MANDELBROT_LOOP 20

// a + bi
int
inside_mandelbrot_set(float init_a, float init_b)
{
  float a = init_a;
  float b = init_b;
  float tmp;

  for(int i = 0; i < MANDELBROT_LOOP; i++) {
    if((a * a + b * b) >= 4) {
      return 0;
    }

    tmp = a;
    a = a * a - b * b + init_a;
    b = 2 * tmp * b + init_b;
  }

  return 1;
}

void
test_gl_draw_mandelbrot(void)
{
  int width = WIDTH;
  int height = HEIGHT;

  gl_init(width, height, GL_SINGLEBUFFER);

  gl_clear(GL_WHITE);

  // X-axis: -2 ~ 2, real
  // Y-axis: -1 ~ 1, imaginary
  // Screen center is the origin point
  for(int y = 0; y < height; y++) {
    for(int x = 0; x < width; x++) {
      float real = (float)(x - width / 2) / (float)(width / 2) * 2;
      float imaginary = (float)(height / 2 - y) / (float)(height / 2);

      if(inside_mandelbrot_set(real, imaginary)) {
        gl_draw_pixel(x, y, GL_BLACK);
      }
    }
  }
}

void
test_gl_draw_font(void)
{
  int width = WIDTH / 4;
  int height = HEIGHT / 4;

  gl_init(width, height, GL_SINGLEBUFFER);

  gl_clear(GL_WHITE);

  // A full character
  gl_draw_char(0, 0, 'A', GL_GREEN);

  // A clipped character
  gl_draw_char(width - 10, -5, 'B', GL_RED);

  // A full string
  gl_draw_string(0, height / 2, "Awesome CS107E!", GL_YELLOW);

  // A clipped string
  gl_draw_string(width - 100,
    height / 2,
    "What I can not create, I do not understand.",
    GL_YELLOW);
}

void
test_console(void)
{
  console_init(20, 40);

  {
    console_printf("Hello World!");
  }

  {
    timer_delay(1);
    console_printf("\nOther line!");
  }

  {
    timer_delay(1);
    console_printf("\nLone long long long long long long long long long line!");
  }

  // Scroll
  {
    timer_delay(1);
    for(int i = 0; i < 16; i++) {
      console_printf("\nLine %d", i);
    }

    timer_delay(1);
    console_printf("\nScroll down");
  }

  // `\b`, `\r`
  {
    timer_delay(1);
    console_printf("\nMove cursor back and carriage return");
    for(int i = 0; i < 5; i++) {
      timer_delay_ms(100);
      console_printf("\b");
    }

    timer_delay(1);
    console_printf("\r");
  }

  // `\f`
  {
    timer_delay(1);
    console_printf("\nform feed to clear all contents");
    console_printf("\f");
  }
}

void
test_extension_drawing(void)
{
  int width = WIDTH / 4;
  int height = HEIGHT / 4;

  gl_init(width, height, GL_SINGLEBUFFER);

  gl_clear(GL_WHITE);

  // Basic line
  gl_draw_line(100, 50, 250, 40, GL_BLACK);

  // Clipped line
  gl_draw_line(width - 50, 50, width + 50, 30, GL_BLACK);

  // Basic triangle
  gl_draw_triangle(10, 10, 50, 80, 80, 60, GL_YELLOW);

  // Clipped triangle
  gl_draw_triangle(width - 50, 30, width + 50, 40, width + 20, 60, GL_YELLOW);
}

void
test_extension_draw_hexagram(void)
{
  int width = WIDTH / 4;
  int height = HEIGHT / 4;

  gl_init(width, height, GL_SINGLEBUFFER);

  gl_clear(GL_WHITE);

  int cx = width / 2;
  int cy = height / 2;

  float r = (Minimum(width, height) - 20.0f) / 2.0f;
  float t1 = r * 0.866025f;
  float t2 = r * 0.5f;

  // Clockwise, point1 ~ point6
  int x1 = cx, y1 = cy - r;
  int x2 = cx + t1, y2 = cy - t2;
  int x3 = cx + t1, y3 = cy + t2;
  int x4 = cx, y4 = cy + r;
  int x5 = cx - t1, y5 = cy + t2;
  int x6 = cx - t1, y6 = cy - t2;

  color_t color = 0xffeae458;

  // Hexagram
  gl_draw_line_with_width(x1, y1, x3, y3, 3.0f, color);
  gl_draw_line_with_width(x1, y1, x5, y5, 3.0f, color);
  gl_draw_line_with_width(x2, y2, x4, y4, 3.0f, color);
  gl_draw_line_with_width(x2, y2, x6, y6, 3.0f, color);
  gl_draw_line_with_width(x3, y3, x5, y5, 3.0f, color);
  gl_draw_line_with_width(x4, y4, x6, y6, 3.0f, color);

  // Inner hexagram
  r = r * 0.3;
  t1 = r * 0.866025f;
  t2 = r * 0.5f;

  x1 = cx, y1 = cy - r;
  x2 = cx + t1, y2 = cy - t2;
  x3 = cx + t1, y3 = cy + t2;
  x4 = cx, y4 = cy + r;
  x5 = cx - t1, y5 = cy + t2;
  x6 = cx - t1, y6 = cy - t2;

  gl_draw_triangle(x1, y1, x3, y3, x5, y5, color);
  gl_draw_triangle(x2, y2, x4, y4, x6, y6, color);
}

void
main(void)
{
  // test_fb_singlebuffer();

  // test_fb_doublebuffer();

  // test_gl_simple();

  // test_gl_doublebuffer();

  // test_gl_draw_mandelbrot();

  // test_gl_draw_font();

  // test_console();

  // test_extension_drawing();

  test_extension_draw_hexagram();

  uart_putchar(EOT);
}
