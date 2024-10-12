#ifndef SPI_H_
#define SPI_H_
void SPI_vMasterInit(void);
void SPI_vSlaveInit(void);
unsigned char SPI_MasterTransmitchar(unsigned char data);
unsigned char SPI_SlaveReceivechar(unsigned char data);
void SPI_MasterTransmitString(unsigned char* ptr);
#endif