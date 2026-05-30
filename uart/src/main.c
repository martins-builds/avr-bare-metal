#include<avr/io.h>
#include<util/delay.h>


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
unsigned char uart_receive(void){
    while(!(UCSR0A & (1 << RXC0)));
    return UDR0;
}
void uart_print(const char *str) {
    while (*str) {
        uart_send(*str++);
    }
}
int main(void){
    uart_init();
    unsigned char input;
    while(1){
        input = uart_receive();
        if (input == 'A')
        {
            uart_print("Hello From Arduino\r\n");
            _delay_ms(1000);
        }
        else if (input == 'B')
        {
            uart_print("My Creator is Martins-Builds\r\n");
            _delay_ms(1000);
        }
        else
            uart_send(input);
    }
    return 0;
}