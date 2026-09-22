#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/kernel.h>

#include "buzzer.h"
#include "pwm.h"
#include "rgb.h"
#include "servo.h"
#include "zephyr/sys/printk.h"

#define UART_NODE DT_CHOSEN(zephyr_console)
static const struct device *uart = DEVICE_DT_GET(UART_NODE);

#define CMD_BUFFER_SIZE 64
static char cmd_buffer[CMD_BUFFER_SIZE];
static size_t cmd_pos = 0;

/* Servo sweep state: 0 -> 90 -> 0, one degree every 20 ms (as in temp.cpp) */
#define SERVO_UPDATE_MS 20
#define SERVO_MIN_ANGLE 0
#define SERVO_MAX_ANGLE 90

static int servo_angle = SERVO_MIN_ANGLE;
static int servo_direction = 1;
static int64_t last_servo_update;

/* Non-blocking buzzer state: turn off once the requested duration expires */
static bool buzzer_active;
static int64_t buzzer_start;
static int buzzer_duration_ms;

/* Parse a decimal integer that must consume the whole string */
static bool parse_number(const char *s, long *value) {
  char *end;

  if (*s == '\0') {
    return false;
  }

  *value = strtol(s, &end, 10);

  return *end == '\0';
}

/* Strip leading/trailing whitespace in place and return the result */
static char *trim(char *s) {
  size_t len;

  while (*s == ' ' || *s == '\t') {
    s++;
  }

  len = strlen(s);
  while (len > 0 && (s[len - 1] == ' ' || s[len - 1] == '\t' ||
                     s[len - 1] == '\r' || s[len - 1] == '\n')) {
    len--;
  }
  s[len] = '\0';

  return s;
}

/* #RRGGBB -> light the LED closest to that color */
static void handle_rgb(const char *cmd) {
  char *end;
  long color = strtol(cmd + 1, &end, 16);

  if (end == cmd + 1 || *end != '\0' || color < 0 || color > 0xFFFFFF) {
    printk("Invalid HEX color: %s\n", cmd);
    return;
  }

  rgb_set_hex((uint32_t)color);

  printk("HEX received: %s\n", cmd);
}

/* Y<0-255> -> set yellow LED brightness via PWM */
static void handle_yellow(const char *cmd) {
  long value;

  if (!parse_number(cmd + 1, &value) || value < 0 || value > 255) {
    printk("Invalid brightness (expected Y0-Y255): %s\n", cmd);
    return;
  }

  pwm_set_brightness((uint8_t)value);

  printk("Yellow LED brightness: %ld\n", value);
}

/* B<ms> -> buzz for the given duration without blocking the loop */
static void handle_buzzer(const char *cmd) {
  long duration;

  if (!parse_number(cmd + 1, &duration) || duration <= 0) {
    printk("Invalid duration (expected B>0): %s\n", cmd);
    return;
  }

  printk("Buzzing for %ld ms\n", duration);

  buzzer_on();
  buzzer_active = true;
  buzzer_start = k_uptime_get();
  buzzer_duration_ms = (int)duration;
}

static void process_command(const char *cmd) {
  printk("Command received: %s\n", cmd);

  if (cmd[0] == '#') {
    handle_rgb(cmd);
  } else if (cmd[0] == 'Y') {
    handle_yellow(cmd);
  } else if (cmd[0] == 'B') {
    handle_buzzer(cmd);
  } else {
    printk("Unknown command\n");
  }
}

/* Stop the buzzer once the requested duration has elapsed */
static void update_buzzer(void) {
  if (buzzer_active &&
      (k_uptime_get() - buzzer_start) >= buzzer_duration_ms) {
    buzzer_off();
    buzzer_active = false;
  }
}

/* Sweep the servo between 0 and 90 degrees, one step every 20 ms */
static void update_servo(void) {
  int64_t now = k_uptime_get();

  if (now - last_servo_update < SERVO_UPDATE_MS) {
    return;
  }
  last_servo_update = now;

  servo_angle += servo_direction;

  /* <= because two conditions can trigger on the same step */
  if (servo_angle >= SERVO_MAX_ANGLE) {
    servo_angle = SERVO_MAX_ANGLE;
    servo_direction = -1;
  }

  if (servo_angle <= SERVO_MIN_ANGLE) {
    servo_angle = SERVO_MIN_ANGLE;
    servo_direction = 1;
  }

  servo_set_angle(servo_angle);
}

/* Drain everything currently buffered in the UART, one command per line */
static void uart_receive(void) {
  uint8_t c;

  while (uart_poll_in(uart, &c) == 0) {
    if (c == '\r' || c == '\n') {
      if (cmd_pos > 0) {
        cmd_buffer[cmd_pos] = '\0';
        cmd_pos = 0;

        const char *cmd = trim(cmd_buffer);

        if (cmd[0] != '\0') {
          process_command(cmd);
        }
      }
    } else if (cmd_pos < CMD_BUFFER_SIZE - 1) {
      cmd_buffer[cmd_pos++] = (char)c;
    }
  }
}

int main(void) {
  if (!device_is_ready(uart)) {
    return 0;
  }

  if (buzzer_init() != 0) {
    printk("Buzzer init failed\n");
    return 0;
  }

  if (rgb_init() != 0) {
    printk("RGB init failed\n");
    return 0;
  }

  if (pwm_init() != 0) {
    printk("PWM init failed\n");
    return 0;
  }

  if (servo_init() != 0) {
    printk("Servo init failed\n");
    return 0;
  }

  /* Park the servo at 0 degrees, matching myServo.write(0) in setup() */
  servo_set_angle(SERVO_MIN_ANGLE);
  last_servo_update = k_uptime_get();

  printk("ESP32 command interface ready\n");

  while (1) {
    uart_receive();
    update_servo();
    update_buzzer();
    k_msleep(1);
  }

  return 0;
}
