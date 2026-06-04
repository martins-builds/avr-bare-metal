# RC522 RFID Reader

Reads RFID card UIDs over SPI using a bare-metal C driver.
No Arduino libraries — direct register manipulation only.

## How It Works
On startup the RC522 is initialised over SPI. The main loop
continuously sends a REQA command to wake up cards in range,
reads the UID bytes from the FIFO buffer, and prints them
over UART in hex format.

## Hardware
- Arduino Uno (ATmega328P)
- RC522 RFID module
- RFID card or key fob

## Wiring
| RC522 Pin | Arduino Pin |
|-----------|-------------|
| VCC       | 3.3V (not 5V) |
| GND       | GND |
| RST       | Pin 9 (PB1) |
| SDA/CS    | Pin 10 (PB2) |
| MOSI      | Pin 11 (PB3) |
| MISO      | Pin 12 (PB4) |
| SCK       | Pin 13 (PB5) |

## Peripherals Used
- **SPI** — SPCR, SPDR, SPSR registers, master mode
- **UART** — prints card UID at 9600 baud in hex format
- **GPIO** — RST and SS pin control

## Built With
- avr-gcc (bare metal — no Arduino framework)
- ATmega328P datasheet + RC522 datasheet
- Direct register manipulation only

## Monitor
screen /dev/tty.usbserial-1420 9600