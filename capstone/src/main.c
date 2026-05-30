#include "uart.h"
#include "adc.h"
#include "dht11.h"
#include "lcd.h"

volatile uint16_t counter = 0;
volatile uint16_t speed_threshold = 50;

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
    PORTB &= ~(1 << PB5);
    DDRD &= ~(1 << PD2);
    PORTD |= (1 << PD2);

    uint8_t hum = 0, temp = 0, idle_printed = 0;

    timer_init();
    sei();
    adc_init();
    uart_init();
    twi_init();
    lcd_init();
    _delay_ms(2000);
    while(1){
         if (!idle_printed) {
            while (!(PIND & (1 << PD2)));  // make sure button is released before printing idle
            _delay_ms(20);
            lcd_send_cmd(0x01);
            _delay_ms(2);
            lcd_print("PRESS BUTTON FOR");
            lcd_set_cursor(1, 0);
            lcd_print("INFO");
            idle_printed = 1;
        }

        if (!(PIND & (1 << PD2))) {
            _delay_ms(20);
            if (!(PIND & (1 << PD2)))
            {
                while (!(PIND & (1 << PD2)));
                _delay_ms(50);
               lcd_send_cmd(0x01);   // clear LCD first
                _delay_ms(5);         // let LCD finish
                _delay_ms(2000);
                dht11_start();        // THEN start DHT11
                dht11_response();
                hum = 0;
                temp = 0;
                dht11_read_data(&hum, &temp);

                uint16_t voltage_mv = (adc_read() * 5000UL) / 1023;
                uint16_t whole = voltage_mv / 1000;
                uint16_t decimal = voltage_mv % 1000;

                if (hum == 0 && temp == 0) {
                    lcd_print("SENSOR ERROR");
                    uart_print("Checksum failed\r\n");
                    // skip the rest
                } else {
                    if(temp > 30)
                        speed_threshold = 20;
                    else
                        speed_threshold = 50;

                    lcd_print("T: ");
                    lcd_print_number(temp);
                    lcd_print("C ");
                    lcd_print("H: ");
                    lcd_print_number(hum);
                    lcd_print("%");

                    lcd_set_cursor(1, 0);
                    
                    lcd_print("V: ");
                    lcd_print_number(whole);
                    lcd_print(".");
                    if(decimal < 100)
                        lcd_print("0");
                    if(decimal < 10)
                        lcd_print("0");
                    lcd_print_number(decimal);
                    lcd_print("V");
                    _delay_ms(200);
                }

                uart_print("Temp: ");
                uart_print_number(temp);
                uart_print("C | ");
                uart_print("Hum: ");
                uart_print_number(hum);
                uart_print("% | ");
                uart_print("V: ");
                uart_print_number(whole);
                uart_send('.');
                if(decimal < 100)
                    uart_send('0');
                if(decimal < 10)
                    uart_send('0');
                uart_print_number(decimal);
                uart_print("V\r\n");


                while (!(PIND & (1 << PD2))); // just wait — do nothing inside
                _delay_ms(20);               // debounce the press
                while (PIND & (1 << PD2));
                _delay_ms(50);
                lcd_send_cmd(0x01);          // clear ONCE after button pressed
                _delay_ms(2);
                PORTB &= ~(1 << PB5);       // LED off
                idle_printed = 0;           // ✓ reset so idle message reprints
            }
        }
    }
    return 0;
}