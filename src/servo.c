#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <util/delay.h>
#include <avr/io.h>

#define SERVO_PIN PD1 

void servo_init() {
	DDRB |= (1 << SERVO_PIN); //Set pin PD1(D9)

    TCCR1A |= (1 << COM1A1) | (1 << WGM11);
    TCCR1B |= (1 << WGM12) | (1 << WGM13) | (1 << CS11);
    ICR1 = 39999; //20ms PWM period
}

void set_servo_angle(uint8_t angle) {
    OCR1A = angle; //1000 + ((angle * 1000) / 180);
}
