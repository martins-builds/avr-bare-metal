# DHT11 Temperature & Humidity Sensor

Bare-metal DHT11 driver for ATmega328P. Reads temperature and humidity
using a custom single-wire protocol implementation. No Arduino libraries.

## How it works
1. Connect the DHT11 to the arduino board
2. Send the code to the board
3. Open screen display to see current temperature and humidity


## Concepts demonstrated
- Single-wire protocol (custom timing-based communication)
- Pin direction switching (output for start signal, input for reading)
- Pulse width measurement for bit decoding
- Checksum validation
- UART output

## Setup
- Arduino Uno (ATmega328P)
- Signal pin on DHT11 to PIN 4
- Data pin on DHT11 on 5V output
- Negative (right) on DHT11 to GND
- avr-gcc + avrdude on macOS

## Flash
make flash

## Monitor
screen /dev/tty.usbserial-1420 9600