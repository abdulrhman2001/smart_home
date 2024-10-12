#ifndef EEPROM_H_
#define EEPROM_H_
void EEPROM_WRITE(unsigned short address,unsigned char data);
unsigned char EEPROM_READ(unsigned short address);
#endif