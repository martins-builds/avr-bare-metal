MCU = atmega328p
F_CPU = 16000000UL
PORT = /dev/tty.usbserial-1420
BAUD = 115200

CC = avr-gcc
CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -Wall

main: src/main.c
	$(CC) $(CFLAGS) -o main.elf src/main.c
	avr-objcopy -O ihex main.elf main.hex

flash: main
	avrdude -c arduino -p m328p -P $(PORT) -b $(BAUD) -U flash:w:main.hex

clean:
	rm -f main.elf main.hex