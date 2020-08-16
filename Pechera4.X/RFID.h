
typedef struct spi
{
    unsigned char Datos[50];
    unsigned char contDatos;
    unsigned char cantDatos;
}SPI;

typedef enum rfidcommand
{
    Idle = 0,
    Mem = 1,
    GenerateRandomID = 2,
    CalcCRC = 3,
    Transmit = 4,
    NoCmdChange = 7,
    Receive = 8,
    Transceive = 12,
    MFAuthent = 14,
    SoftReset = 15
}RFIDCommands;


void RFIDInit(SPI* spi);
void RFIDRecive(SPI *spi);
void RFIDTransmit(SPI *spi, char *data, char length);