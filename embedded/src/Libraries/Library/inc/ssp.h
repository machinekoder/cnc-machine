/**
 * @file ssp.h
 * @author Johannes Wimmer & Alexander Rössler
 * @brief SSP Library
 * @date 16-10-2012
 */
/** @defgroup SSP SSP (Synchronous Serial Port)
 * @ingroup Peripheral
 * @{
 */
#pragma once

#include <sspDriver.h>
#include <types.h>
#include <gpio.h>
#include <pincon.h>

typedef enum
{
    Ssp0 = 0,
    Ssp1 = 1
} Ssp;

/** Maintains the data size of one transmission byte */
typedef enum
{
  Ssp_DataSize_4Bit =  0b0011,
  Ssp_DataSize_5Bit =  0b0100,
  Ssp_DataSize_6Bit =  0b0101,
  Ssp_DataSize_7Bit =  0b0110,
  Ssp_DataSize_8Bit =  0b0111,
  Ssp_DataSize_9Bit =  0b1000,
  Ssp_DataSize_10Bit = 0b1001,
  Ssp_DataSize_11Bit = 0b1010,
  Ssp_DataSize_12Bit = 0b1011,
  Ssp_DataSize_13Bit = 0b1100,
  Ssp_DataSize_14Bit = 0b1101,
  Ssp_DataSize_15Bit = 0b1110,
  Ssp_DataSize_16Bit = 0b1111,
}Ssp_DataSize;
  
/** The frame format which should be used for the SSP bus */
typedef enum
{
  Ssp_FrameFormat_Spi       = 0b00,
  Ssp_FrameFormat_Ti        = 0b01,
  Ssp_FrameFormat_Microwire = 0b10,
}Ssp_FrameFormat;

/** Connect to the bus a slave or master */
typedef enum
{
    Ssp_Mode_Master = 0,
    Ssp_Mode_Slave = 1
} Ssp_Mode;
    
/** With loopback enabled input is taken from the output pin */
typedef enum
{
    Ssp_Loopback_Enabled = 1,
    Ssp_Loopback_Disabled = 0
} Ssp_Loopback;

/** Only necessary for SPI frame format.
 * Low Clock Out Polarity means that the clock is kept low between frames
 * High Clock Out Polarity means that the clock is kept high between frames */
typedef enum
{
    Ssp_ClockOutPolarity_Low = 0,
    Ssp_ClockOutPolarity_High = 1
} Ssp_ClockOutPolarity;

/** Only necessary for SPI frame format.
 *  First Clock Out Phase means that the ssp controller captures the signal on the first transition of the clock
 *  Second Clock Out Phase means that the ssp controller captures the signal on the second transition of the clock */
typedef enum 
{
    Ssp_ClockOutPhase_First = 0,
    Ssp_ClockOutPhase_Second = 1
} Ssp_ClockOutPhase;

/** Defines wheter a slave should output something on the bus or not */
typedef enum
{
    Ssp_SlaveOutput_Enabled = 0,
    Ssp_SlaveOutput_Disabled = 1
} Ssp_SlaveOutput;

/** Initializes SSP hardware device
 *  @param ssp              The SSP device to initialize.
 *  @param baudrate         The baudrate the SSP device should be initialized with.
 *  @param dataSize         The size of one chunk that is transfered.
 *  @param frameFormat      The frame format the hardware should be initialized with.
 *  @param masterSlave      Slave or master mode.
 *  @param loopbackMode     Enable or disables the loopback mode.
 *  @param slaveOutput      Defines wheter a slave should output bytes or not.
 *  @param clockOutPolarity Defines the clock out polarity if device is in SPI mode.
 *  @param clockOutPhase    Defines the clock out phase if device is in SPI mode.
 */
extern void Ssp_initialize(Ssp ssp,
                           uint32 baudrate,
                           Ssp_DataSize dataSize,
                           Ssp_FrameFormat frameFormat, 
                           Ssp_Mode masterSlave, 
                           Ssp_Loopback loopbackMode,
                           Ssp_SlaveOutput slaveOutput,
                           Ssp_ClockOutPolarity clockOutPolarity,
                           Ssp_ClockOutPhase clockOutPhase
                          );
/** Initializes a SEL pin for the SSP device.
 *  @param ssp  The SSP device to initialize.
 *  @param id   Id of the SEL pin.
 *  @param port The port of the pin.
 *  @param pin  The pin number.
 */
extern void Ssp_initializeSel(Ssp ssp, uint8 id, uint8 port, uint8 pin);
/** Puts a character to the SSP1 bus and discards the received data.
 *  @param ssp      The SSP device to write to.
 *  @param selId    Id of the SEL pin.
 *  @param data     The data to write.
 */
extern void Ssp_write(Ssp ssp, uint8 selId, uint16 data);
/** Gets a character from the SSP1 bus and writes 0 to the bus.
 *  @param ssp      The SSP device to read from.
 *  @param selId    Id of the SEL pin.
 *  @param data     A pointer to the data to read.
 */
extern void Ssp_read(Ssp ssp, uint8 selId, uint16* data);
/** Reads and writes data to the SSP1 bus
 *  @param ssp          The SSP device to read write to.
 *  @param selId        Id of the SEL pin.
 *  @param writeData    The data to write.
 *  @param readData     A pointer to the data to read.
 */
extern void Ssp_readWrite(Ssp ssp, uint8 selId, uint16 writeData, uint16* readData);

/**
 * @}
 */
