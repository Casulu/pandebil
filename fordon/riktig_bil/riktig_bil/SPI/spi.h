#ifndef SPI_H_
#define SPI_H_

#define MOSI 3
#define MISO 4
#define SCK 5

void spi_master_init(void);
void spi_master_transmit(char data);

#endif /* SPI_H_ */