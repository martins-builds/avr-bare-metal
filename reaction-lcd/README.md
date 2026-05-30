# Reaction Timer using AVR LCD1602 I2C Driver

Bare-metal reaction timer using button to interact with the I2C driver for the LCD1602 display with PCF8574 backpack.
Prints text to a 16x2 LCD from an ATmega328P with no Arduino libraries.

## How it works
1. TWI hardware initialized at 100kHz
2. I2C START condition sent to PCF8574 at address 0x27
3. LCD initialized in 4-bit mode following HD44780 sequence
4. Characters sent as two nibbles with EN pulse to latch each
5. Timer fires ISR 100 times per second to count random delay
6. Button press captured in main loop, state machine controls game flow

## Concept demonstrated
- I2C
- LCD functions
- TWI functions
- Timer
- Interrupt

## Setup
- Arduino Uno (ATmega328P)
- SDA on Pin A4
- Ground to pin GND
- SCL on Pin A5
- 5V power to VCC
- avr-gcc + avrdude on macOS
- LED to pin 13
- Button to pin 2

## Flash
make flash