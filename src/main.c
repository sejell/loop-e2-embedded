#include <stdio.h>
#include <zephyr/kernel.h>

#include "buzzer.h"

int main(void)
{
    printf("E2 Buzzer Project Started!\n");

    if (buzzer_init() != 0) {
        printf("Buzzer initialization failed!\n");
        return 0;
    }

    printf("Buzzer initialized successfully.\n");

    while (1) {
        printf("Buzzer ON for 2 seconds\n");

        buzzer_play(2000);

        printf("Buzzer OFF for 2 seconds\n");

        k_msleep(2000);
    }

    return 0;
}
