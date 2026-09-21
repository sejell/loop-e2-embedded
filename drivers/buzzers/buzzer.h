#ifndef BUZZER_H
#define BUZZER_H

int buzzer_init(void);
void buzzer_on(void);
void buzzer_off(void);
void buzzer_play(int duration_ms);

#endif