# DHT11 Temperature & Humidity Monitor

Button-triggered temperature and humidity monitor using a DHT11 sensor and 16x2 I2C LCD display. Built in bare-metal C — no Arduino libraries.

## Hardware
- Arduino Uno
- DHT11 temperature & humidity sensor (3-pin module)
- 16x2 LCD with I2C backpack (PCF8574, address 0x27)
- Push button
- LED + 220Ω resistor

## How It Works
LCD displays a prompt on startup. Press the button to trigger a DHT11 read — temperature and humidity display on the LCD and the LED turns on. Press again to return to idle.

## Wiring
| Component | Arduino Pin |
|-----------|-------------|
| DHT11 Data | PD4 (Pin 4) |
| Button | PD2 (Pin 2) |
| LED | PB5 (Pin 13) |
| LCD SDA | A4 |
| LCD SCL | A5 |

## Built With
- avr-gcc (bare metal — no Arduino framework)
- PlatformIO
- Direct register manipulation only