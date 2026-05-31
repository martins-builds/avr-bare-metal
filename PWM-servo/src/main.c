#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>

void servo_init(void){
    TCCR1A = (1 << WGM11) | (1 << COM1A1);
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11);
    ICR1 = 40000;
    OCR1A = 3000;
}
void servo_set_angle(uint8_t angle){
    OCR1A = 1100 + (angle * 3200UL / 180);
}
int main(void){

    DDRB |= (1 << PB1);
    servo_init();
    while (1)
    {
        servo_set_angle(0);
        _delay_ms(2000);
        servo_set_angle(90);
        _delay_ms(2000);
        servo_set_angle(25);
        _delay_ms(2000);
        servo_set_angle(180);
        _delay_ms(2000);
        servo_set_angle(0);
        _delay_ms(2000);
    }
    
    return 0;
}