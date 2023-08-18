#include <stdio.h>

void timer0_init(void);
void timer0_stop(void);
void timer2_init(void);

volatile uint16_t milliseconds = 0;
volatile uint8_t seconds = 0;
volatile uint8_t hours = 0;
volatile uint8_t minutes = 0;

volatile uint16_t milliseconds_second = 0;
volatile uint8_t seconds_second = 0;
volatile uint8_t minutes_second = 0;
