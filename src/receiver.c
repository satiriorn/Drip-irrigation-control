#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "STDIO_UART.h"

#include "servo.h"

#include "timer.h"

//	Include nRF24L01+ library
#include "nrf24l01.h"
#include "nrf24l01-mnemonics.h"
#include "spi.h"
#include "capacitive_soil_moisture.h"
void print_config(void);

void init(void);

//	Used in IRQ ISR
volatile bool message_received = false;
volatile bool status = false;
volatile bool state_sleep = false;

bool valve_status = false;

uint8_t time_sleep_min = 3;
uint8_t time_work_min = 1;
volatile uint16_t timer_counter = 0;


#define VALVE_OPEN 1259
#define VALVE_CLOSE 2299
#define VALVE_SEMIOPEN 1779
#define TIMER_HOUR_STOP 1	

int main(void)
{	
	init();
	char tx_message[32], rx_message[32];				// Define string array
    while (1) 
    {
		if (message_received)
		{
			message_received = false;
			strcpy(rx_message, nrf24_read_message());			
			if(strcmp(rx_message, "ON VALVE")==0)
			{
				printf("State on\r\n");
				set_servo_angle(VALVE_OPEN);
				valve_status = true;
				strcpy(tx_message,"State valve - open\r\n");
			}
			else if (strcmp(rx_message, "OFF VALVE")==0){
				printf("State off\r\n");
				set_servo_angle(VALVE_CLOSE);
				valve_status = false;
				strcpy(tx_message,"State valve - close\r\n");
			}
			else if(strcmp(rx_message, "TIMER ON")==0){
				printf("Start timer\r\n");
				set_servo_angle(VALVE_OPEN);
				timer0_init();
				valve_status = true;
				strcpy(tx_message,"Timer on state valve - open\r\n");
			}
			else if(strcmp(rx_message, "TIMER OFF")==0){
				printf("Timer end\r\n");
				//valve_status = false;
				set_servo_angle(VALVE_CLOSE);
				timer0_stop();
				valve_status = false;
				milliseconds=seconds=hours=minutes = 0;
				strcpy(tx_message,"Timer off state valve - close\r\n");
			}
			else{
				strcpy(tx_message,"The command is incorrect\r\n");
			}
			/*
			status = nrf24_send_message(tx_message);
			if (status == true) printf("Message sent successfully\r\n");
			*/
		}
		else if(seconds_second==20 && state_sleep==true){
				milliseconds_second=seconds_second=milliseconds_second=0;
				state_sleep=false;
				sleep_disable();
				sei();
				nrf24_state(POWERUP);
				servo_init();
				printf("SLEEP OFF\n");
			}
		else if(state_sleep==false && seconds_second==20){
				milliseconds_second=seconds_second=milliseconds_second=0;
				printf("SLEEP ON\n");
				state_sleep=true;
				sei();
				servo_power_off();
				nrf24_state(POWERDOWN);
				sleep_mode();
				sleep_enable();
				sleep_cpu();
			}
		if(transform_to_percent()<50 && !valve_status){
			printf("Valve open. %i percentage of humidity\n", transform_to_percent());
			set_servo_angle(VALVE_OPEN);
			valve_status = true;
		}
		else {
			set_servo_angle(VALVE_CLOSE);
		}
    }
}

//	Interrupt on IRQ pin
ISR(INT0_vect) 
{
	message_received = true;
}

ISR(TIMER0_COMPA_vect) {
    milliseconds+=16;
	if (milliseconds >= 1000) {
		milliseconds = 0;
		seconds++;
	}
	if(seconds==59){
		minutes++;
		seconds = 0;
		printf("%i:%i:%i\r\n", hours, minutes, seconds);
	}
	if(minutes>59){
		hours++;
		minutes = 0;
	}
	if(TIMER_HOUR_STOP==minutes){
		milliseconds=seconds=hours=minutes = 0;
		set_servo_angle(VALVE_OPEN);
	}
	/*
	if(30==seconds){
		set_servo_angle(VALVE_CLOSE);
	}
	*/
}

ISR(TIMER2_COMPA_vect) {
	milliseconds_second+=32;
    if (milliseconds_second >= 1000) {
		milliseconds_second = 0;
		seconds_second++;
		printf("%i:%i\r\n", minutes_second, seconds_second);
	}
	if(seconds_second==59){
		minutes_second++;
		seconds_second = 0;
	}	
}

void init(void) {	
	CLKPR = 0x80;// Enable change in clock frequency
  	CLKPR = 0x01; // Set clock division factor to 2, resulting in 8 MHz
	uart_init();//	Initialize UART
    nrf24_init();//	Initialize nRF24L01+ and print configuration info
	print_config();
	servo_init(); //Initialize control servo
	capacitive_soil_moisture_init();
	nrf24_start_listening(); //	Start listening to incoming messages
	set_servo_angle(VALVE_CLOSE);
	set_sleep_mode(SLEEP_MODE_PWR_SAVE);
	sei();
	timer2_init();
}

void print_config(void)
{
	uint8_t data;
	printf("Startup successful\n\n nRF24L01+ configured as:\n");
	printf("-------------------------------------------\n");
	nrf24_read(CONFIG,&data,1);
	printf("CONFIG		0x%x\n",data);
	nrf24_read(EN_AA,&data,1);
	printf("EN_AA			0x%x\n",data);
	nrf24_read(EN_RXADDR,&data,1);
	printf("EN_RXADDR		0x%x\n",data);
	nrf24_read(SETUP_RETR,&data,1);
	printf("SETUP_RETR		0x%x\n",data);
	nrf24_read(RF_CH,&data,1);
	printf("RF_CH			0x%x\n",data);
	nrf24_read(RF_SETUP,&data,1);
	printf("RF_SETUP		0x%x\n",data);
	nrf24_read(STATUS,&data,1);
	printf("STATUS		0x%x\n",data);
	nrf24_read(FEATURE,&data,1);
	printf("FEATURE		0x%x\n",data);
	printf("-------------------------------------------\n\n");
}
