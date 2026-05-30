# Reaction Timer

Measures how fast you can press a button after an LED turns on.
Built in bare-metal C — no Arduino libraries.

## How It Works
Press the button to start. The LED turns on after a random delay.
Press the button again as fast as you can — your reaction time prints
over UART in milliseconds.

## Peripherals Used
- **Timer0** — CTC mode, counts milliseconds via ISR
- **ISR** — TIMER0_COMPA interrupt increments a millisecond counter
- **GPIO** — button input with debounce, LED output
- **UART** — prints reaction time result at 9600 baud

## Wiring
| Component | Arduino Pin |
|-----------|-------------|
| Button    | PD2 (Pin 2) |
| LED       | PB5 (Pin 13)|

## Built With
- avr-gcc (bare metal — no Arduino framework)
- ATmega328P datasheet
- Direct register manipulation only