#include<avr/io.h>
#include<avr/interrupt.h>
#include <util/delay.h>

#define LCD_ADDR 0x27
#define LCD_BL   0x08  // backlight bit
volatile uint8_t state = 0;
volatile uint16_t random_delay = 0;
volatile uint16_t react_time = 0;
volatile uint16_t counter = 0;
volatile uint8_t led_on = 0;  // set in ISR when LED turns on

//lcd functions
void twi_init(void){
    TWBR = 72;
    TWSR = 0x00;
    TWCR = (1 << TWEN);
}
void twi_start(void){
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTA);
    while(!(TWCR & (1 << TWINT)));
}
void twi_write(uint8_t data){
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while(!(TWCR & (1 << TWINT)));
}
void twi_stop(void){
    TWCR = (1 << TWSTO) | (1 << TWINT) | (1 << TWEN);
}
void lcd_send_byte(uint8_t data){
    twi_start();
    twi_write(LCD_ADDR << 1);  // address + write bit
    twi_write(data | LCD_BL);  // send with backlight on
    twi_stop();
}
void lcd_pulse_enable(uint8_t data){
    lcd_send_byte(data | 0x04);   // EN high
    _delay_us(1);
    lcd_send_byte(data & ~0x04);  // EN low
    _delay_us(50);
}
void lcd_send_nibble(uint8_t nibble, uint8_t rs){
    uint8_t data = (nibble & 0xF0) | LCD_BL | rs;
    lcd_pulse_enable(data);
}
void lcd_send_cmd(uint8_t cmd){
    lcd_send_nibble(cmd & 0xF0, 0x00);        // high nibble, RS=0
    lcd_send_nibble((cmd << 4) & 0xF0, 0x00); // low nibble, RS=0
}
void lcd_send_char(uint8_t ch){
    lcd_send_nibble(ch & 0xF0, 0x01);        // high nibble, RS=1
    lcd_send_nibble((ch << 4) & 0xF0, 0x01); // low nibble, RS=1
}
void lcd_init(void){
    _delay_ms(50);              // wait for LCD power up

    // special initialization sequence - 4-bit mode
    lcd_send_nibble(0x30, 0);   // function set
    _delay_ms(5);
    lcd_send_nibble(0x30, 0);   // function set
    _delay_us(150);
    lcd_send_nibble(0x30, 0);   // function set
    _delay_us(150);
    lcd_send_nibble(0x20, 0);   // switch to 4-bit mode

    // now in 4-bit mode - configure
    lcd_send_cmd(0x28);  // 2 lines, 5x8 font
    lcd_send_cmd(0x0C);  // display on, cursor off
    lcd_send_cmd(0x06);  // entry mode - increment cursor
    lcd_send_cmd(0x01);  // clear display
    _delay_ms(2);        // clear needs extra time
}
void lcd_print(const char *str){
    while(*str){
        lcd_send_char(*str++);
    }
}
void lcd_set_cursor(uint8_t row, uint8_t col){
    uint8_t addr = (row == 0) ? 0x80 + col : 0xC0 + col;
    lcd_send_cmd(addr);
}
void lcd_print_number(uint16_t num){
    char buf[6];
    uint8_t i = 0;
    if (num == 0){
        lcd_send_char('0');
        return;
    }
    while (num > 0){
        buf[i++] = '0' + (num % 10);
        num /= 10;
    }
    while (i > 0){
        lcd_send_char(buf[--i]);
    }
}

//reaction functions

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
            led_on = 1;
        }
        
    }
    else if (state == 2)
    {
        react_time++;
    }
    
}
int main(void){
    twi_init();
    lcd_init();
    timer_init();
    sei();
    DDRB |= (1 << PB5);
    PORTB &= ~(1 << PB5);  // explicitly off at startup
    _delay_ms(1000);  // wait for serial connection to settle
    DDRD &= ~(1 << PD2);
    PORTD |= (1 << PD2);
    lcd_print("Press to start");
    while(1){
        if (led_on) {
            lcd_send_cmd(0x01);
            _delay_ms(2);
            lcd_print("PRESS NOW!");
            led_on = 0;
        }
        if (!(PIND & (1 << PD2))) {
            if (state == 0) {
                random_delay = 50 + (TCNT0 * 2);
                counter = 0;
                state = 1;
                lcd_send_cmd(0x01);  // clear
                _delay_ms(2);
                lcd_print("Get ready...");
                lcd_send_cmd(0x01);  // clear
                _delay_ms(200);
            }   else if (state == 2 && react_time > 0) {
                    if (react_time * 10 < 100)
                    {
                        lcd_send_cmd(0x01);  // clear
                        _delay_ms(2);
                        lcd_print("Too fast");
                        lcd_set_cursor(1, 0);
                        lcd_print("CHEATING");
                    }
                    else{
                        lcd_send_cmd(0x01); //clear
                        _delay_ms(2);
                        lcd_print("Your time:");
                        lcd_set_cursor(1, 0);
                        lcd_print_number(react_time * 10);
                        lcd_print("ms");
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