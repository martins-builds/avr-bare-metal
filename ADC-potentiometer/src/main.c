#include<avr/io.h>
#include<util/delay.h>
#include<avr/interrupt.h>
#include<stdint.h>

void adc_init(void){
    ADMUX = (1 << REFS0);
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}
uint16_t adc_read(void){
    ADCSRA |= (1 << ADSC);
    while(ADCSRA & (1 << ADSC));
    return ADC;
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
void uart_print(const char *str) {
    while (*str) {
        uart_send(*str++);
    }
}
int main(void){
    uart_init();
    adc_init();
    while (1)
    {
        uint16_t voltage_mv = (adc_read() * 5000UL) / 1023;
        uint16_t whole = voltage_mv / 1000;
        uint16_t decimal = voltage_mv % 1000;
        uart_print_number(whole);
        uart_send('.');
        if(decimal < 100)
            uart_send('0');
        if(decimal < 10)
            uart_send('0');
        uart_print_number(decimal);
        uart_print("V\r\n");
        _delay_ms(200);
    }
    return 0;
}