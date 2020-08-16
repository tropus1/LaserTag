#include "RFID.h"
#include <xc.h>

void RFIDInit(SPI* spi)
{
    spi->Datos[0] = 0x04; //ComIEnReg
    spi->Datos[1] = 0x60;
    
    spi->Datos[2] = 0x06; //DivIEnReg
    spi->Datos[3] = 0x80;
    
    spi->Datos[4] = 0x22; //ModeReg
    spi->Datos[5] = 0x83;
    
    spi->Datos[6] = 0x24; //TxModeReg
    spi->Datos[7] = 0xB0;
    
    spi->Datos[8] = 0x26; //RxModeReg
    spi->Datos[9] = 0xB0;
    
    spi->Datos[10] = 0x4C; //RFCfgReg
    spi->Datos[11] = 0x00;
    
    spi->Datos[12] = 0x28; //TxControlReg
    spi->Datos[13] = 0x03;
    
    spi->Datos[14] = 0x02; //CommandReg
    spi->Datos[15] = 0x08; //Recieve
    
    spi->cantDatos = 15;
    
    SSP1BUF = spi->Datos[0];
}

void RFIDRecive(SPI *spi)
{
    spi->Datos[0] = 0x02; //CommandReg
    spi->Datos[1] = 0x08; //Recieve
    
    spi->cantDatos = 1;
    SSP1BUF = spi->Datos[0];
}

void RFIDTransmit(SPI *spi, char *data, char length) //Transmite los bytes indicados
{
    spi->Datos[0] = 0x02;
    spi->Datos[1] = 0x01; //Mem
    
    for(char i; i < length; i++)
    {
        spi->Datos[i + 2] = data[i];
    }
    
    spi->Datos[length + 2] = 0x02;
    spi->Datos[length + 3] = 0x04; //Transmit
    
    spi->cantDatos = length + 3;
    SSP1BUF = spi->Datos[0];
}