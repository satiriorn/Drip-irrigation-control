#include <avr/io.h>

#define SERVO_PIN PB1

void servo_init(void) {
    DDRB    |= (1 << SERVO_PIN);
    TCCR1A |= (1 << COM1A1) | (1 << WGM11);
    TCCR1B |= (1 << WGM12) | (1 << WGM13) | (1 << CS11);  
    ICR1 = 15624;           
}

void set_servo_angle(uint16_t angle) {
    OCR1A = angle; //1000 + ((angle * 1000) / 180);
}
void servo_power_off(void){
    TCCR1A = 0;
    TCCR1A = 0;
    DDRB = 0xff;
}
