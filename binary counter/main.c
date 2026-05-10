#include <avr/io.h>
#include <util/delay.h>
#include<stdint.h>

#define time 1500

int main(void){
    DDRB |= (1 << PB2) | (1 << PB3) | (1 << PB4) | (1 << PB5);
    while (1)
    {
        for(uint8_t counter = 0; counter < 16; counter++)
        {
            PORTB &= ~(0b00111100);
            PORTB |= (counter << 2);
            _delay_ms(time);
        }
    }
    return 0;
}