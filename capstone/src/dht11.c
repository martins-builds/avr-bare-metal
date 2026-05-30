#include "dht11.h"

void dht11_start(void){
    DDRD |= (1 << PD4);
    PORTD &= ~(1 << PD4);
    _delay_ms(18);
    PORTD |= (1 << PD4);
    _delay_us(30);
    DDRD &= ~(1 << PD4);
    PORTD |= (1 << PD4);
}
void dht11_response(void){
    while(PIND & (1 << PD4));
    while(!(PIND & (1 << PD4)));
    while(PIND & (1 << PD4));
}
uint8_t dht11_read_bit(void){
    while(!(PIND & (1 << PD4)));  // wait for HIGH
    _delay_us(40);
    uint8_t bit = (PIND & (1 << PD4)) ? 1 : 0;
    while(PIND & (1 << PD4));     // wait for HIGH to finish
    return bit;
}
uint8_t dht11_read_byte(){
    uint8_t data = 0;
    for (int i = 7; i >= 0; i--)
    {
        data |= (dht11_read_bit() << i);
    }
    return data;
}
void dht11_read_data(uint8_t *hum, uint8_t *temp) {
    uint8_t hum_int  = dht11_read_byte();
    uint8_t hum_dec  = dht11_read_byte();
    uint8_t temp_int = dht11_read_byte();
    uint8_t temp_dec = dht11_read_byte();
    uint8_t checksum = dht11_read_byte();

    if (checksum == (hum_int + hum_dec + temp_int + temp_dec)) {
        *hum  = hum_int;
        *temp = temp_int;
    }
}