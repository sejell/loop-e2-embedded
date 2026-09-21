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

    for (int i = 0; i < 3; i++) {
        printf("Playing buzzer for 2 seconds\n");

        buzzer_play(2000);

        printf("Buzzer stopped\n");
        k_msleep(1000);
    }

    printf("Buzzer test complete!\n");

    return 0;
}