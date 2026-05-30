#include "lcd.h"

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
void lcd_set_cursor(uint8_t row, uint8_t col){
    uint8_t addr = (row == 0) ? 0x80 + col : 0xC0 + col;
    lcd_send_cmd(addr);
}