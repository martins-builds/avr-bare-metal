#include<avr/io.h>
#include<util/delay.h>
#include<avr/interrupt.h>
#include<stdint.h>

#define LCD_ADDR 0x27
#define LCD_BL   0x08  // backlight bit

//dth11 functions
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

//uart functions
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
void uart_print(const char *str) {
    while (*str) {
        uart_send(*str++);
    }
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

int main(void){
    uart_init();
    twi_init();
    lcd_init();

    DDRB |= (1 << PB5);
    PORTB &= ~(1 << PB5);
    DDRD &= ~(1 << PD2);
    PORTD |= (1 << PD2);
    _delay_ms(1000); //for dth11 and lcd to settle

    uint8_t hum = 0, temp = 0, idle_printed = 0;
    while (1)
    {
        if (!idle_printed) {
            while (!(PIND & (1 << PD2)));  // make sure button is released before printing idle
            _delay_ms(20);
            lcd_send_cmd(0x01);
            _delay_ms(2);
            lcd_print("PRESS BUTTON FOR");
            lcd_set_cursor(1, 0);
            lcd_print("TEMP AND HUMID");
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

                PORTB |= (1 << PB5);
                if (hum == 0 && temp == 0) {
                    lcd_print("SENSOR ERROR");
                    uart_print("Checksum failed\r\n");
                    // skip the rest
                } else {
                    lcd_print("TEMP IS: ");
                    lcd_print_number(temp);
                    lcd_print("C");
                    lcd_set_cursor(1, 0);
                    lcd_print("HUMIDITY IS: ");
                    lcd_print_number(hum);
                    lcd_print("%");
                }

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