#include<avr/io.h>

void i2c_init(void);
void i2c_send_start(void);
void i2c_send_stop(void);
void i2c_send_address(uint8_t address, char direction);
void i2c_send_byte(uint8_t data);
uint8_t i2c_call(uint8_t address);
uint8_t i2c_scan(void);
uint8_t i2c_write(uint8_t address, uint8_t data);
uint8_t i2c_send_chunk(uint8_t address, uint16_t size, uint8_t* data);