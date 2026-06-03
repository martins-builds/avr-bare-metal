#include<avr/io.h>
#include<util/delay.h>

//uart code
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

//remote code
uint16_t measure_pulse_low(void){
    uint16_t count = 0;
    while(!(PINB & (1 << PB0))) count++;
    return count;
}

uint16_t measure_pulse_high(void){
    uint16_t count = 0;
    while(PINB & (1 << PB0)) count++;
    return count;
}

int main(void){
    uart_init();
    DDRB &= ~(1 << PB0);
PORTB |= (1 << PB0);

while(1){
    if(!(PINB & (1 << PB0))){
        uart_print("LOW\r\n");
        while(!(PINB & (1 << PB0)));
    }
}
}