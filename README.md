# Controlling drip irrigation using nrf24l01+ and arduino uno/nano

The idea of the project is remote control using a radio module nrf24l01+.
A library was used for initialization and interaction with the radio module:
https://github.com/thehelvijs/nRF24L01-avr-bareminimum/tree/master
The valve is controlled using a servo motor. The servomotor is controlled via the pwm bus by Timer1(16-bit).
Timer0 is also used to turn irrigation on and off for a specified time. Timer2 is used to count down time in sleep mode.
In a sleepless state on the Arduino nano board, the consumption varies between 20-30mah. In SLEEP_MODE_PWR_SAVE 10-20mah.
The receiver is powered by two 18650 batteries connected in series. The working time depends on the intensity of the sleep mode. 
For more efficient operation from the battery, the frequency was reduced to 8 MHz instead of 16 MHz
Scheme of connecting elements for receiver:

<img src="https://github.com/satiriorn/Drip-irrigation-control/blob/Satiriorn/schemes/scheme_of_receiver.jpg" alt="Scheme of connecting elements for receiver"/>

The transmitter has minimum functionality(yet)
Scheme of connecting elements for transmitter:
<img src="https://github.com/satiriorn/Drip-irrigation-control/blob/Satiriorn/schemes/scheme_of_transmitter.jpg" alt="Scheme of connecting elements for transmitter"/>

For the case, a 3D model was used at the link:
https://www.thingiverse.com/thing:5347976
For arduino nano model scaling can be default but 18650 need bigger size or need to use smaller size battery. 
For arduino uno you must increase size of model by 20%.

Scheme of connecting elements for transmitter:
<img src="https://github.com/satiriorn/Drip-irrigation-control/blob/Satiriorn/schemes/result.jpg" alt="Result"/>