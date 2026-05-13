#include<avr/io.h>
#include<util/delay.h>


void uart_init(void){
    UBRR0H = (103 >> 8); 
    UBRR0L = (103);
    UCSR0B |= (1<<TXEN0);
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
int main(void){
    uart_init();
    while(1){
        uart_print("My Creator is Martins-Builds\r\n");
        _delay_ms(1000);
    }
    return 0;
}