#include <zephyr/kernel.h>

#include "pwm.h"

int main(void) {
  int ret = pwm_init();

  if (ret < 0) {
    return ret;
  }

  pwm_set_brightness(128);

  return 0;
}
