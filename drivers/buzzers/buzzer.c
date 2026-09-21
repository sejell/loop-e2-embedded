#include "buzzer.h"

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

#define BUZZER_NODE DT_PATH(buzzer)

static const struct gpio_dt_spec buzzer =
    GPIO_DT_SPEC_GET(BUZZER_NODE, gpios);

int buzzer_init(void)
{
    if (!gpio_is_ready_dt(&buzzer)) {
        return -1;
    }

    return gpio_pin_configure_dt(&buzzer, GPIO_OUTPUT_INACTIVE);
}

void buzzer_on(void)
{
    gpio_pin_set_dt(&buzzer, 1);
}

void buzzer_off(void)
{
    gpio_pin_set_dt(&buzzer, 0);
}

void buzzer_play(int duration_ms)
{
    buzzer_on();

    k_msleep(duration_ms);

    buzzer_off();
}