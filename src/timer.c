#include <avr/io.h>

void timer_init() {
    TCCR0A = (1 << WGM01); // CTC mode
    TCCR0B = (1 << CS02) | (1 << CS00); // prescaler on 1024

    OCR0A = 251; // // Reaching a value of 251 will give an interrupt every 16ms 
    TIMSK0 = (1 << OCIE0A); //enable interrupt
    TIMSK0 = (1 << OCIE0A);
}

void timer_stop(){
    TCCR0B = 0;
}
