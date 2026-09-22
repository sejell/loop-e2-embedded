#include "servo.h"

#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>

#define SERVO_NODE DT_PATH(servo)

static const struct pwm_dt_spec servo =
    PWM_DT_SPEC_GET(SERVO_NODE);

int servo_init(void)
{
    if (!pwm_is_ready_dt(&servo)) {
        return -1;
    }

    return 0;
}

int servo_set_angle(int angle)
{
    if (angle < 0 || angle > 180) {
        return -1;
    }

    /*
     * Convert:
     * 0 degrees   -> 1 ms pulse
     * 90 degrees  -> 1.5 ms pulse
     * 180 degrees -> 2 ms pulse
     */
    uint32_t pulse_us = 1000 + ((uint32_t)angle * 1000 / 180);

    return pwm_set_pulse_dt(&servo, PWM_USEC(pulse_us));
}