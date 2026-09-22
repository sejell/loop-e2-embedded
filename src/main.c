#include <stdio.h>
#include <zephyr/kernel.h>

#include "rgb.h"

int main(void)
{
    printf("HEX RGB test starting!\n");

    if (rgb_init() != 0) {
        printf("RGB initialization failed!\n");
        return 0;
    }

    // Mostly RED
    printf("Testing 0xF51020 - should be RED\n");
    rgb_set_hex(0xF51020);
    k_msleep(2000);

    // Mostly GREEN
    printf("Testing 0x20E840 - should be GREEN\n");
    rgb_set_hex(0x20E840);
    k_msleep(2000);

    // Mostly BLUE
    printf("Testing 0x2020E8 - should be BLUE\n");
    rgb_set_hex(0x2020E8);
    k_msleep(2000);

    rgb_off();

    printf("HEX RGB test complete!\n");

    return 0;
}