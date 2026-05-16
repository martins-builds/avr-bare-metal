# AVR Reaction Timer

Measures human reaction time in milliseconds using bare-metal C on an ATmega328P.
No Arduino libraries used.

## How it works
1. Press the button to start
2. Wait for the LED to turn on (random delay)
3. Press the button as fast as you can
4. Your reaction time appears in your terminal

## Cheat detection
Presses under 100ms are flagged as "Too fast, Cheating"

## Concepts demonstrated
- Hardware timer with ISR (Timer0 CTC mode)
- State machine (WAITING, ARMED, REACTING)
- UART transmit and receive
- Volatile shared variables between ISR and main
- Hardware randomness via TCNT0 capture

## Setup
- Arduino Uno (ATmega328P)
- LED on Pin 13
- Button on Pin 2
- avr-gcc + avrdude on macOS

## Flash
make flash

## Monitor
screen /dev/tty.usbserial-1420 9600