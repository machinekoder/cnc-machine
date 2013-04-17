#include "spi.h"
int8 initializeSPI(uint32 freq,float duty, uint8 ch)
{
    SPI_ENABLE_POWER();
    SPI_SET_CORE_CLK();
    

    SPI_SET_SSEL1;             
    SPI_SET_SCK1;
    SPI_SET_MISO1;
    SPI_SET_MOSI1;
    
    SPI_ENABLE_IRQ;
    
    
    return 0;
}

