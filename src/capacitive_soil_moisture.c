#include <avr/io.h>

void capacitive_soil_moisture_init(void){
    // Configure PORT C bit 0 to an input
	DDRC = (1 << DDC0);
	// Configure ADC to be left justified, use AVCC as reference, and select ADC1 as ADC input
	ADMUX = 0b01100001;
	// Set the prescaler(min value 128)
	ADCSRA = 150;
}

int16_t get_value_from_sensor(void){
    ADCSRA = ADCSRA | (1 << ADSC);
    return ADCH;
}

uint16_t transform_to_percent(void){
    uint16_t max = 140;
    uint16_t min = 65;
    return 100 - (((get_value_from_sensor()-min)*100)/(max-min));
}