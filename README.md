# AVR Bare-Metal

Embedded systems projects written in bare-metal C on the ATmega328P (Arduino Uno).
No Arduino libraries. No abstractions. Direct register manipulation only.

## Projects

| Project | Description | Peripherals |
|---------|-------------|-------------|
| [blink](./blink/) | LED blink via register | GPIO |
| [button](./button/) | Button-controlled LED | GPIO, debounce |
| [binary-counter](./binary-counter/) | Binary counter on LEDs | GPIO, timer |
| [interrupt-led](./interrupt-led/) | LED toggle via interrupt | INT0, ISR |
| [uart](./uart/) | Serial communication | USART registers |
| [adc-potentiometer](./ADC-potentiometer/) | Voltage reading | ADC, ADMUX |
| [lcd](./lcd/) | I2C LCD driver | TWI, PCF8574 |
| [dht11](./dht11/) | Temperature & humidity sensor | DHT11 1-wire |
| [reaction-timer](./reaction-timer/) | Reaction speed timer | Timer, UART |
| [reaction-lcd](./reaction-lcd/) | Reaction timer on LCD | Timer, TWI |
| [dht11-button-control-lcd](./dth11-button-control-lcd/) | Button-triggered DHT11 on LCD | GPIO, DHT11, TWI |
| [capstone](./capstone/) | Multi-sensor monitor | GPIO, Timer, ADC, DHT11, TWI, UART |
| [pwm-servo](./servo/) | Servo motor control via hardware PWM | Timer1, PWM, OCR1A |

## Built With
- avr-gcc + avrdude — bare metal, no Arduino framework
- ATmega328P datasheet as primary reference

## Focus
Every project in this repo controls hardware directly through memory-mapped
registers. No digitalWrite(), no Wire.h, no Serial — just C and the datasheet.