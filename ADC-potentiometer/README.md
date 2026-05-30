# ADC Potentiometer

uses a potentiometer and displays the changing voltage in volts on the screen

## How it works
1. Connect the potentiometer to the arduino board
2. Send the code to the board
3. Open screen dispalay to see current voltage
4. Turn the potentiometer to change voltage


## Concepts demonstrated
- ADC
- ADMUX
- ADCSRA
- Integer math for voltage conversion 
- UART output on screen

## Setup
- Arduino Uno (ATmega328P)
- Left Potentiometer pin to GND
- middle potentiometer pin on A0
- Right pin to 5V
- avr-gcc + avrdude on macOS

## Flash
make flash

## Monitor
screen /dev/tty.usbserial-1420 9600