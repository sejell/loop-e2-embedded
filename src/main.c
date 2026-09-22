#include <zephyr/kernel.h>

#include "servo.h"

int main(void) {
  printf("Servo test starting!\n");

  if (servo_init() != 0) {
    printf("Servo initialization failed!\n");
    return 0;
  }

  printf("Moving to 0 degrees\n");
  servo_set_angle(0);
  k_msleep(2000);

  printf("Moving to 90 degrees\n");
  servo_set_angle(90);
  k_msleep(2000);

  printf("Moving to 180 degrees\n");
  servo_set_angle(180);
  k_msleep(2000);

  printf("Moving back to 90 degrees\n");
  servo_set_angle(90);

  printf("Servo test complete!\n");

  return 0;
}
