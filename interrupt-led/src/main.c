#include<avr/io.h>
#include<avr/interrupt.h>

volatile uint16_t counter = 0;
volatile uint16_t speed_threshold = 0;

void timer_init(void){
    OCR0A = 155;
    TCCR0A = (1 << WGM01);
    TCCR0B = (1 << CS02) | (1 << CS00);
    TIMSK0 = (1 << OCIE0A);
}
ISR(TIMER0_COMPA_vect){
    counter++;
    if (counter >= speed_threshold){
        PORTB ^= (1 << PB5);
        counter = 0;
    }
}

int main(void){
    DDRB |= (1 << PB5);
    DDRD &= ~(1 << PD2);
    PORTD |= (1 << PD2);
    timer_init();
    sei();
    while(1){
        if (!(PIND & (1 << PD2))){
            speed_threshold = 20;
        }
        else{
            speed_threshold = 50;
        }
    }
    return 0;
}