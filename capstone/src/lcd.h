#ifndef LCD_H
#define LCD_H

#include<avr/io.h>
#include <util/delay.h>

#define LCD_ADDR 0x27
#define LCD_BL   0x08  // backlight bit

void twi_init(void);
void twi_start(void);
void twi_write(uint8_t data);
void twi_stop(void);
void lcd_send_byte(uint8_t data);
void lcd_pulse_enable(uint8_t data);
void lcd_send_nibble(uint8_t nibble, uint8_t rs);
void lcd_send_cmd(uint8_t cmd);
void lcd_send_char(uint8_t ch);
void lcd_init(void);
void lcd_print(const char *str);
void lcd_print_number(uint16_t num);    
void lcd_set_cursor(uint8_t row, uint8_t col);

#endif