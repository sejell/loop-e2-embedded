#ifndef PWM_H
#define PWM_H

#include <stdint.h>

int pwm_init(void);
int pwm_set_brightness(uint8_t brightness);

#endif // !PWM_H
