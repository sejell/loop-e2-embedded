#include "rgb.h"

#include <zephyr/drivers/gpio.h>

#define RGB_NODE DT_PATH(rgb_leds)

static const struct gpio_dt_spec red_led =
    GPIO_DT_SPEC_GET(RGB_NODE, red_gpios);

static const struct gpio_dt_spec green_led =
    GPIO_DT_SPEC_GET(RGB_NODE, green_gpios);

static const struct gpio_dt_spec blue_led =
    GPIO_DT_SPEC_GET(RGB_NODE, blue_gpios);


int rgb_init(void)
{
    if (!gpio_is_ready_dt(&red_led) ||
        !gpio_is_ready_dt(&green_led) ||
        !gpio_is_ready_dt(&blue_led)) {
        return -1;
        }

    if (gpio_pin_configure_dt(&red_led, GPIO_OUTPUT_INACTIVE) != 0) {
        return -1;
    }

    if (gpio_pin_configure_dt(&green_led, GPIO_OUTPUT_INACTIVE) != 0) {
        return -1;
    }

    if (gpio_pin_configure_dt(&blue_led, GPIO_OUTPUT_INACTIVE) != 0) {
        return -1;
    }

    return 0;
}


void rgb_off(void)
{
    gpio_pin_set_dt(&red_led, 0);
    gpio_pin_set_dt(&green_led, 0);
    gpio_pin_set_dt(&blue_led, 0);
}


void rgb_red(void)
{
    rgb_off();
    gpio_pin_set_dt(&red_led, 1);
}


void rgb_green(void)
{
    rgb_off();
    gpio_pin_set_dt(&green_led, 1);
}


void rgb_blue(void)
{
    rgb_off();
    gpio_pin_set_dt(&blue_led, 1);
}