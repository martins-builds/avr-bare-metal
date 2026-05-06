#include <avr/io.h>
#include <util/delay.h>

#define DOT_ON    100
#define DOT_OFF   100
#define DASH_ON   300
#define DASH_OFF  100
#define LETTER_GAP 400
#define NAME_GAP  1000

void dot(void) {
    PORTB |= (1 << PB5);
    _delay_ms(DOT_ON);
    PORTB &= ~(1 << PB5);
    _delay_ms(DOT_OFF);
}

void dash(void) {
    PORTB |= (1 << PB5);
    _delay_ms(DASH_ON);
    PORTB &= ~(1 << PB5);
    _delay_ms(DASH_OFF);
}

int main(void) {
    DDRB |= (1 << PB5);

    while (1) {
        dash(); dash();
        _delay_ms(LETTER_GAP);
        dot(); dash();
        _delay_ms(LETTER_GAP);
        dot(); dash(); dot();
        _delay_ms(LETTER_GAP);
        dash();
        _delay_ms(LETTER_GAP);
        dot(); dot();
        _delay_ms(LETTER_GAP);
        dash(); dot();
        _delay_ms(LETTER_GAP);
        dot(); dot(); dot();
        _delay_ms(NAME_GAP);
    }
        
    return 0;
}
