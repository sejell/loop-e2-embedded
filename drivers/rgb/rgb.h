#ifndef RGB_H
#define RGB_H

#include <stdint.h>

int rgb_init(void);

void rgb_red(void);
void rgb_green(void);
void rgb_blue(void);
void rgb_off(void);

void rgb_set_hex(uint32_t hex_value);

#endif