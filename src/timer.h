#include <stdio.h>

void timer_init();
void timer_stop();

volatile uint16_t milliseconds = 0;
uint8_t seconds = 0;
uint8_t hours = 0;
uint8_t minutes = 0;