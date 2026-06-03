#include<avr/io.h>
#include<util/delay.h>
#include<stdint.h>

#define RC522_CMD_REG     0x01
#define RC522_FIFO_DATA   0x09
#define RC522_FIFO_LEVEL  0x0A
#define RC522_BIT_FRAMING 0x0D
#define RC522_COM_IRQ     0x04

void spi_init(void){
    PRR &= ~(1 << PRSPI);
    DDRB |= (1 << PB2);
    DDR_SPI = (1 << DD_MOSI) | (1 << DD_SCK);
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

uint8_t spi_transfer(uint8_t data){
    SPDR = data;
    while(!(SPSR & (1 << SPIF)));
    return SPDR;
}

void rc522_write(uint8_t reg, uint8_t data){
    PORTB &= ~(1 << PB2);
    spi_transfer((reg << 1) & 0x7E);         // send address byte
    spi_transfer(data); 
    PORTB |= (1 << PB2);
}
uint8_t rc522_read(uint8_t reg){
    PORTB &= ~(1 << PB2);
    spi_transfer(((reg << 1) & 0x7E) | 0x80);         // send address byte
    spi_transfer(0x00); 
    PORTB |= (1 << PB2);
    return data;
}

void rc522_init(void) {
    spi_init();
    rc522_write(RC522_CMD_REG, 0x0F);
    while(rc522_read(RC522_CMD_REG) & (1 << 4));   // wait for PowerDown bit to clear
}
void rc522_request(void){
    rc522_write(RC522_COM_IRQ, 0x00);
    rc522_write(RC522_FIFO_LEVEL, 0x80);
    rc522_write(RC522_BIT_FRAMING, 0x07);
    rc522_write(RC522_FIFO_DATA, 0x26);
    rc522_write(RC522_CMD_REG, 0x0C);
    rc522_write(RC522_BIT_FRAMING, 0x80);

    uint16_t timeout = 2000;
    while(!(rc522_read(RC522_COM_IRQ) & ((1 << 5) | (1 << 4)))) {
        if(--timeout == 0) return;  // no card found
    }
}
void rc522_get_uid(uint8_t *uid){
    uint8_t num = rc522_read(RC522_FIFO_LEVEL);
    for (int i = 0; i < num; i++)
    {
        uid[i] = rc522_read(RC522_FIFO_DATA);
    }
}

int main(void){

    return 0;
}