#include<avr/io.h>
#include<util/delay.h>
#include<avr/interrupt.h>
#include<stdint.h>

void dht11_start(void){
    DDRD |= (1 << PD4);
    PORTD &= ~(1 << PD4);
    _delay_ms(18);
    PORTD |= (1 << PD4);
    _delay_us(30);
    DDRD &= ~(1 << PD4);
    PORTD |= (1 << PD4);
}
void dht11_response(void){
    while(PIND & (1 << PD4));
    while(!(PIND & (1 << PD4)));
    while(PIND & (1 << PD4));
}
uint8_t dht11_read_bit(void){
    while(!(PIND & (1 << PD4)));  // wait for HIGH
    _delay_us(40);
    uint8_t bit = (PIND & (1 << PD4)) ? 1 : 0;
    while(PIND & (1 << PD4));     // wait for HIGH to finish
    return bit;
}
uint8_t dht11_read_byte(){
    uint8_t data = 0;
    for (int i = 7; i >= 0; i--)
    {
        data |= (dht11_read_bit() << i);
    }
    return data;
}
void dht11_read_data(uint8_t *hum, uint8_t *temp) {
    uint8_t hum_int  = dht11_read_byte();
    uint8_t hum_dec  = dht11_read_byte();
    uint8_t temp_int = dht11_read_byte();
    uint8_t temp_dec = dht11_read_byte();
    uint8_t checksum = dht11_read_byte();

    if (checksum == (hum_int + hum_dec + temp_int + temp_dec)) {
        *hum  = hum_int;
        *temp = temp_int;
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
    _delay_ms(2000);
    while (1)
    {
        dht11_start();
        dht11_response();
        uint8_t hum, temp;
        dht11_read_data(&hum, &temp);
        uart_print("Temperature is: ");
        uart_print_number(temp);
        uart_print(" C\r\n");
        uart_print("Humidity is: ");
        uart_print_number(hum);
        uart_print("%\r\n");
        _delay_ms(2000);
    }
    return 0;
}