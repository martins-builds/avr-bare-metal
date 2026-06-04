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
    DDRB |= (1 << PB3) | (1 << PB5);
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
    uint8_t data = spi_transfer(0x00); 
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

void uart_init(void){
    UBRR0H = (103 >> 8); 
    UBRR0L = (103);
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}
void uart_send(unsigned char data){
    while(!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
}
void uart_print(const char *str) {
    while (*str) {
        uart_send(*str++);
    }
}
void uart_print_number(uint16_t num){
    char buf[6];
    uint8_t i = 0;
    
    if (num == 0){
        uart_send('0');
        return;
    }
    while (num > 0){
        buf[i++] = '0' + (num % 10);
        num /= 10;
    }
    // digits are reversed, print backwards
    while (i > 0){
        uart_send(buf[--i]);
    }
}

int main(void){
    uart_init();
    rc522_init();
    uint8_t uid[5];
    _delay_ms(2000);

    while (1)
    {
        rc522_request();
        rc522_get_uid(uid);
        for (int i = 0; i < 4; i++)
        {
            uart_print_number(uid[i]);
            uart_send(' ')
        }
        uart_print("\r\n");
        _delay_ms(500);        
    }
    return 0;
}