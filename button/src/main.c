#include <avr/io.h>
#include <util/delay.h>
#include<stdint.h>

int main(void){
    uint8_t ledState = 0;
    DDRB |= (1 << PB5);
    DDRD &= ~(1 << PD2);
    PORTD |= (1 << PD2);
    while (1)
    {
        if (!(PIND & (1 << PD2)))
        {
            _delay_ms(20);
            if (ledState == 0)
            {
                PORTB |= (1 << PB5);
                ledState = 1;
            }
            else{
                PORTB &= ~(1 << PB5);
                ledState = 0;
            }
            while (!(PIND & (1 << PD2)));
                _delay_ms(20);
        }
    }
    return 0;
}