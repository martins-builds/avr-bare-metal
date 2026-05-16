#include<avr/io.h>
#include<avr/interrupt.h>
#include <util/delay.h>

volatile uint8_t state = 0;
volatile uint16_t random_delay = 0;
volatile uint16_t react_time = 0;
volatile uint16_t counter = 0;

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

void timer_init(void){
    OCR0A = 155;
    TCCR0A = (1 << WGM01);
    TCCR0B = (1 << CS02) | (1 << CS00);
    TIMSK0 = (1 << OCIE0A);
}
ISR(TIMER0_COMPA_vect){
    if (state == 0)
    {
        
    }
    else if (state == 1)
    {
        counter++;
        if (counter >= random_delay)
        {
            PORTB |= (1 << PB5);
            react_time = 0;
            counter = 0;
            state = 2;
        }
        
    }
    else if (state == 2)
    {
        react_time++;
    }
    
}


int main(void){

    uart_init();
    timer_init();
    sei();
    DDRB |= (1 << PB5);
    PORTB &= ~(1 << PB5);  // explicitly off at startup
    _delay_ms(1000);  // wait for serial connection to settle
    DDRD &= ~(1 << PD2);
    PORTD |= (1 << PD2);

    while(1){
        if (!(PIND & (1 << PD2))) {
            if (state == 0) {
                random_delay = 50 + (TCNT0 * 2);
                counter = 0;
                state = 1;
                _delay_ms(200);
            }   else if (state == 2 && react_time > 0) {
                    if (react_time * 10 < 100)
                    {
                        uart_print("Too fast, Cheating\r\n");
                    }
                    else{
                        uart_print_number(react_time * 10);  // *10 converts ticks to ms
                        uart_print("ms");
                        uart_send('\r');
                        uart_send('\n');
                    }
                    react_time = 0;
                    PORTB &= ~(1 << PB5);
                    state = 0;
                    _delay_ms(200);
                }
        }       
    }
    return 0;
}