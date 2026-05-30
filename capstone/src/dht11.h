#ifndef DHT11_H
#define DHT11_H

#include<avr/io.h>
#include<util/delay.h>
#include<stdint.h>

void dht11_start(void);
void dht11_response(void);
uint8_t dht11_read_bit(void);
uint8_t dht11_read_byte();
void dht11_read_data(uint8_t *hum, uint8_t *temp);

#endif