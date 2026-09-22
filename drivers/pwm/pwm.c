#include "pwm.h"
#include <zephyr/drivers/pwm.h>

#define PWM_NODE DT_PATH(pwm_led)

static const struct pwm_dt_spec led_pwm = PWM_DT_SPEC_GET(PWM_NODE);

int pwm_init(void) {
  if (!pwm_is_ready_dt(&led_pwm)) {
    return -ENODEV;
  }

  return 0;
}

int pwm_set_brightness(uint8_t brightness) {
  uint32_t pulse = (led_pwm.period * brightness) / 255U;
  return pwm_set_pulse_dt(&led_pwm, pulse);
}
