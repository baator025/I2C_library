void i2c_init(void);
uint8_t i2c_call(uint8_t address);
uint8_t i2c_scan(void);
uint8_t i2c_write(uint8_t address, uint8_t data);
uint8_t i2c_send_chunk(uint8_t address, uint8_t size, uint8_t* data);