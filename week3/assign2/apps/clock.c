#include "../basic.h"
#include "../gpio.h"
#include "../timer.h"

static volatile u32 *gp_pud = (u32 *)0x20200094;
static volatile u32 *gp_pud_clk = (u32 *)0x20200098;

// see BCM 2835 manual page 101
static void gpio_set_pullup(uint pin)
{
  uint bank = pin / 32;
  uint shift = pin % 32;

  *gp_pud = 2;

  for (volatile int i = 0; i < 150; i++) ;

  *(gp_pud_clk + bank) = 1 << shift;

  for (volatile int i = 0; i < 150; i++) ;

  *gp_pud = 0;
  *(gp_pud_clk + bank) = 0;
}

// Bit 0 (the least significant) will represent segment A, bit 1 segment B...
// Bit 7 DP
static u8 bit_patterns[] = {
  0x3f, // 0 0b00111111
  0x30, // 1 0b00110000
  0x5b, // 2 0b01011011
  0x4f, // 3 0b01001111
  0x66, // 4 0b01100110
  0x6d, // 5 0b01101101
  0x7d, // 6 0b01111101
  0x07, // 7 0b00000111
  0x7f, // 8 0b01111111
  0x6f, // 9 0b01101111
  0x77, // A 0b01110111
  0x7f, // B 0b01111111
  0x39, // C 0b00111001
  0x3f, // D 0b00111111
  0x79, // E 0b01111001
  0x71, // F 0b01110001
  0x40, // - 0b01000000
};

static void clear() {
  for(int i = 20; i <= 27; i++) {
    gpio_write(i, 0);
  }

  for(int i = 10; i <= 13; i++) {
    gpio_write(i, 0);
  }
}

static void display_bit_pattern(int digit_index, u8 pattern) {
  clear();

  gpio_write(10 + digit_index, 1);

  for(int i = 0; i < 8; i++) {
    if(pattern & (1 << i)) {
      gpio_write(20 + i , 1);
    }
  }
}

static u8 char_pattern(char c) {
  u8 pattern = 0;

  if(c >= '0' && c <= '9') {
    pattern = bit_patterns[c - '0'];
  } else if(c >= 'A' && c <= 'F') {
    pattern = bit_patterns[10 + c - 'A'];
  } else if(c == '-') {
    pattern = bit_patterns[16];
  }

  return pattern;
}

static u8 number_pattern(int num) {
  return bit_patterns[num];
}

static volatile u32 *gp_lev = (u32 *)0x20200034;
static int read_pin(uint pin) {
  u32 value  = *(gp_lev + (pin / 32));
  int offset = pin % 32;
  return (value >> offset) & 0x1;
}

static int button_status[54] = {};

// we need to debounce
static bool is_button_pressed(uint pin) {
  int *counter = button_status + pin;

  if(*counter == 0 && read_pin(pin) == 1) return false;

  // wait 10 cycles of main loop
  // that means 10 * 10000us = 100ms
  if(*counter == 10) {
    *counter = 0;

    return read_pin(pin) == 0;
  }

  *counter += 1;

  return false;
}

inline static
void save_total_seconds(int total_seconds, int dest[]) {
  int minutes = total_seconds / 60;
  int seconds = total_seconds % 60;
  dest[0] = minutes / 10;
  dest[1] = minutes % 10;
  dest[2] = seconds / 10;
  dest[3] = seconds % 10;
}

// max: inclusive
inline static
int inc_value_with_max(int value, int max) {
  value++;
  return value > max ? value % (max + 1) : value;
}

void main(void) {
  // set GPIO 20 ~ 27 to output
  // corresponding to A, B, ... F, DP
  for(int i = 20; i <= 27; i++) {
    gpio_set_output(i);
  }

  // set GPIO 10 ~ 13 to output
  // corresponding to Digigit 1 ~ 4
  for(int i = 10; i <= 13; i++) {
    gpio_set_output(i);
  }

  // set GPIO 2 ~ 3 to input
  int red_button_pin = 2;
  gpio_set_input(red_button_pin);
  gpio_set_pullup(red_button_pin);

  int blue_button_pin = 3;
  gpio_set_input(blue_button_pin);
  gpio_set_pullup(blue_button_pin);

  bool is_running = false;
  bool in_setting_mode = false;
  bool display_hyphen = true;
  int active_digit = 0;
  int total_seconds = 0;
  int count = 0;

  u8 patterns[4] = {};
  int setting_value[4] = {};

  // one main loop costs 10000us, 10ms
  while(1) {
    if(is_button_pressed(red_button_pin)) {
      display_hyphen = false;

      if(in_setting_mode) {
        int value = setting_value[active_digit];
        value = inc_value_with_max(value, (active_digit % 2) == 0 ? 5 : 9);
        setting_value[active_digit] = value;
      } else if(is_running) {
        is_running = false;
        save_total_seconds(total_seconds, setting_value);
      } else {
        is_running = true;
        total_seconds = (setting_value[0]*10 + setting_value[1])*60 + setting_value[2]*10 + setting_value[3];
      }
    };

    if(is_button_pressed(blue_button_pin)) {
      display_hyphen = false;

      if(in_setting_mode) {
        active_digit++;

        if(active_digit == 4) {
          in_setting_mode = false;
          active_digit = 0;
        }
      } else {
        active_digit = 0;
        in_setting_mode = true;
        is_running = false;
        save_total_seconds(total_seconds, setting_value);
      }
    }

    if(is_running) {
      int minutes = total_seconds / 60;
      int seconds = total_seconds % 60;
      patterns[0] = number_pattern(minutes / 10);
      patterns[1] = number_pattern(minutes % 10);
      patterns[2] = number_pattern(seconds / 10);
      patterns[3] = number_pattern(seconds % 10);
    } else {
      if(in_setting_mode) {
        for(int i = 0; i < 4; i++) {
          // blink effect
          if(i == active_digit && ((count / 50) % 2 == 1)) {
            patterns[i] = 0;
          } else {
            patterns[i] = number_pattern(setting_value[i]);
          }
        }
      } else {
        for(int i = 0; i < 4; i++) {
          if(display_hyphen) {
            patterns[i] = char_pattern('-');
          } else {
            patterns[i] = number_pattern(setting_value[i]);
          }
        }
      }
    }

    for(int i = 0; i < 4; i++) {
      display_bit_pattern(i, patterns[i]);
      timer_delay_us(2500);
    }

    count++;
    if(is_running && count % 100 == 0) {
      total_seconds++;
    }
  }
}

