#include <avr/io.h>

void timer0_init(void) {
    TCCR0A = (1 << WGM01); // CTC mode
    TCCR0B = (1 << CS02) | (1 << CS00); // prescaler on 1024

    OCR0A = 251; // // Reaching a value of 251 will give an interrupt every 16ms 
    TIMSK0 = (1 << OCIE0A); //enable interrupt
}

void timer2_init(void) {
    TCCR2A = (1 << WGM21); // CTC mode
    TCCR2B = (1 << CS22) | (1 << CS21) | (1 << CS20); // prescaler on 1024

    OCR2A = 251; // Reaching a value of 251 will give an interrupt every 16ms
    TIMSK2 = (1 << OCIE2A); // Enable interrupt
}

void timer0_stop(void){
    TCCR0B = 0;
}
