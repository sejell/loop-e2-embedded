#include <stdio.h>
#include <zephyr/kernel.h>

#include "rgb.h"

int main(void)
{
    printf("RGB test starting!\n");

    if (rgb_init() != 0) {
        printf("RGB initialization failed!\n");
        return 0;
    }

    printf("RED\n");
    rgb_red();
    k_msleep(2000);

    printf("GREEN\n");
    rgb_green();
    k_msleep(2000);

    printf("BLUE\n");
    rgb_blue();
    k_msleep(2000);

    rgb_off();

    printf("RGB test complete!\n");

    return 0;
}