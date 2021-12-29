#include <avr/io.h>

void i2c_init(void){
	TWCR |= (1<<TWINT);
	TWCR |= (1<<TWEN);								//enable TWI pins
	//TWI speed
	TWSR |= (1<<TWPS0);								//prescaler value
	TWSR &= ~(1<<TWPS1);
	TWBR = 12;										//bit rate register value
}

uint8_t i2c_call(uint8_t address){
	TWCR |= (1<<TWINT);
	TWCR |= (1<<TWSTA);										//send start bit
	while (! (TWCR & (1<<TWINT)));
	TWDR = 2*address;										//transmit SLA+R/W, for write SLA+W=2*SLA, for read SLA+R=2*SLA+1
	TWCR = (1<<TWINT) | (1<<TWEN);
	while (! (TWCR & (1<<TWINT)));
	if((TWSR&0xF8) == 0x18){								//check ACK bit, 0x18 for transmiter, 0x40 for receiver
		TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
		while (! (TWCR & (1<<TWSTO)));
		return 1;
		} else{
		TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
		while (! (TWCR & (1<<TWSTO)));
		return 0;
	}
}

uint8_t i2c_scan(void){
	uint8_t SLA;
	for(SLA = 1; SLA < 128; SLA++){
		if(i2c_call(SLA) == 1){
			return SLA;
			break;
			} else if(SLA == 127){
			return 255;
			break;
		}
	}
	return 254;
}

uint8_t i2c_write(uint8_t address, uint8_t data){
	TWCR |= (1<<TWINT);
	TWCR |= (1<<TWSTA);										//send start bit
	while (! (TWCR & (1<<TWINT)));
	TWDR = 2*address;										//transmit SLA+W
	TWCR = (1<<TWINT) | (1<<TWEN);
	while (! (TWCR & (1<<TWINT)));
	//if((TWSR&0xF8) == 0x18){								//check ACK bit, 0x18 for transmiter
	TWDR = data;										//send data
	TWCR = (1<<TWINT) | (1<<TWEN);
	while (! (TWCR & (1<<TWINT)));
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);			//send stop bit
	while (! (TWCR & (1<<TWSTO)));
	//}
	return 0;
}

uint8_t i2c_send_chunk(uint8_t address, uint8_t size, uint8_t* data){
	uint8_t i;
	TWCR |= (1<<TWINT);
	TWCR |= (1<<TWSTA);										//send start bit
	while (! (TWCR & (1<<TWINT)));
	TWDR = 2*address;										//transmit SLA+W
	TWCR = (1<<TWINT) | (1<<TWEN);
	while (! (TWCR & (1<<TWINT)));
	
	if((TWSR&0xF8) != 0x18)	return 'e';						//check ACK bit, 0x18 for transmiter
	
	for(i=0; i<size; i++){
		TWDR = *(data + i);										//send data
		TWCR = (1<<TWINT) | (1<<TWEN);
		while (! (TWCR & (1<<TWINT)));
	}
	
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);			//send stop bit
	while (! (TWCR & (1<<TWSTO)));
	
	return 0;
}