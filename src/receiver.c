//	Set clock frequency
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>

//	Set up UART for printf();
#ifndef BAUD
#define BAUD 9600
#endif
#include "STDIO_UART.h"

#include "servo.h"

#include "timer.h"

//	Include nRF24L01+ library
#include "nrf24l01.h"
#include "nrf24l01-mnemonics.h"
#include "spi.h"
void print_config(void);

void init(void);

//	Used in IRQ ISR
volatile bool message_received = false;
volatile bool status = false;
//bool valve_status = false;
#define VALVE_OPEN 1499
#define VALVE_CLOSE 3499
#define VALVE_SEMIOPEN 2499
#define TIMER_HOUR_STOP 1

int main(void)
{	
	init();
	char tx_message[32];				// Define string array
	char rx_message[32];
    while (1) 
    {
		printf("%i:%i:%i\r\n", hours, minutes, seconds);
		if (message_received)
		{
			message_received = false;
			printf("Received message: %s\r\n",nrf24_read_message());
			strcpy(rx_message, nrf24_read_message());
			
			if(strcmp(rx_message, "ON VALVE")==0)
			{
				printf("State on\r\n");
				set_servo_angle(VALVE_OPEN);
				strcpy(tx_message,"State valve - open\r\n");
			}
			else if (strcmp(rx_message, "OFF VALVE")==0){
				printf("State off\r\n");
				set_servo_angle(VALVE_CLOSE);
				strcpy(tx_message,"State valve - close\r\n");
			}
			else if(strcmp(rx_message, "TIMER ON")==0){
				printf("Start timer\r\n");
				set_servo_angle(VALVE_OPEN);
				timer_init();
				strcpy(tx_message,"Timer on state valve - open\r\n");
			}
			else if(strcmp(rx_message, "TIMER OFF")==0){
				printf("Timer end\r\n");
				set_servo_angle(VALVE_CLOSE);
				timer_stop();
				milliseconds=seconds=hours=minutes = 0;
				strcpy(tx_message,"Timer off state valve - close\r\n");
			}
			else{
				strcpy(tx_message,"The command is incorrect\r\n");
			}
			
			_delay_ms(500);
			status = nrf24_send_message(tx_message);
			if (status == true) printf("Message sent successfully\r\n");
		}
		_delay_ms(1000);
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
	}
	if(minutes>59){
		hours++;
		minutes = 0;
	}
	if(TIMER_HOUR_STOP==minutes){
		milliseconds=seconds=hours=minutes = 0;
		set_servo_angle(VALVE_OPEN);
	}
	if(30==seconds){
		set_servo_angle(VALVE_CLOSE);
	}
}

void init() {
		//	Initialize UART
	uart_init();
	
	//	Initialize nRF24L01+ and print configuration info
    nrf24_init();
	print_config();
	//Initialize control servo
	servo_init();
	//	Start listening to incoming messages
	nrf24_start_listening();
	set_servo_angle(VALVE_CLOSE);
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
