# AVR Bare-Metal Capstone — Multi-Sensor Monitor

The most complete project in this repo. Combines every peripheral covered
across all previous projects into one working system — written entirely in
bare-metal C with no Arduino libraries.

## What It Does
Press a button to trigger a full sensor read. The system reads temperature
and humidity from a DHT11, reads voltage from a potentiometer via ADC, and
displays everything on a 16x2 I2C LCD. An LED blinks continuously via a
hardware timer — blink speed increases automatically when temperature exceeds
30°C.

## Hardware
- Arduino Uno (ATmega328P)
- DHT11 temperature & humidity sensor (3-pin module)
- 16x2 LCD with I2C backpack (PCF8574, address 0x27)
- Potentiometer (ADC voltage reading)
- Push button
- LED + 220Ω resistor

## Wiring
| Component     | Arduino Pin |
|---------------|-------------|
| DHT11 Data    | PD4 (Pin 4) |
| Button        | PD2 (Pin 2) |
| LED           | PB5 (Pin 13)|
| Potentiometer | A0          |
| LCD SDA       | A4          |
| LCD SCL       | A5          |

## Peripherals Used
- **GPIO** — button input with software debounce, LED output
- **Timer0** — CTC mode interrupt-driven LED blink, no delay()
- **ADC** — potentiometer voltage reading via ADMUX/ADCSRA registers
- **DHT11** — 1-wire protocol, checksum validation
- **TWI/I2C** — custom driver for 16x2 LCD (no Wire.h)
- **UART** — serial debug output at 9600 baud

## Project Structure
capstone/
├── src/
│   ├── main.c       — application logic, timer ISR
│   ├── dht11.c/h    — DHT11 sensor driver
│   ├── lcd.c/h      — TWI/I2C LCD driver
│   ├── adc.c/h      — ADC driver
│   └── uart.c/h     — UART driver
└── Makefile

## Built With
- avr-gcc (bare metal — no Arduino framework)
- PlatformIO
- Direct register manipulation only