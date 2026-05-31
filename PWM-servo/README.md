# PWM Servo Motor

Bare-metal servo motor control using hardware PWM on Timer1.
Sweeps through 0°, 90°, 25°, and 180° positions. No Arduino libraries.

## How it works
- Timer1 configured in Fast PWM mode 14 with ICR1 as TOP
- Prescaler of 8 gives 2MHz timer clock
- ICR1 = 40000 sets 50Hz PWM frequency (20ms period)
- OCR1A controls pulse width — 1000 ticks = 0°, 4000 ticks = 180°
- Formula: OCR1A = 1000 + (angle * 3000 / 180)

## Concepts demonstrated
- Hardware PWM (Timer1 Fast PWM mode 14)
- ICR1 as period register
- OCR1A as pulse width register
- Servo calibration via pulse width tuning

## Setup
- Arduino Uno (ATmega328P)
- Brown wire → GND
- Red wire → 5V
- Yellow/Orange wire → Pin 9 (PB1, OC1A)
- avr-gcc + avrdude on macOS

## Flash
make flash