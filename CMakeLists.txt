cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(e2_buzzer)

target_sources(app PRIVATE
        src/main.c
        drivers/buzzers/buzzer.c
        drivers/rgb/rgb.c
        drivers/servo/servo.c
        drivers/pwm/pwm.c
)

target_include_directories(app PRIVATE
        drivers/buzzers
        drivers/rgb
        drivers/servo
        drivers/pwm
)
