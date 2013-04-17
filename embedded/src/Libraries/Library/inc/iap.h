/**
 * @file iap.h
 * @author Alexander Rössler
 * @brief Library for controlling the IAP functions
 * @date 12-12-2012
 */
/** @defgroup IAP IAP (In Application Programming)
 * @ingroup Peripheral
 * @{
 */
#pragma once
#include <types.h>
#include <string.h>
#include <LPC17xx.h>

typedef enum {
    Iap_Status_CommandSuccess = 0,
    Iap_Status_InvalidCommand = 1,
    Iap_Status_SourceAddressError = 2,
    Iap_Status_DestinationAddressError = 3,
    Iap_Status_SourceAddressNotMapped = 4,
    Iap_Status_DestinationAddressNotMapped = 5,
    Iap_Status_CountError = 6,
    Iap_Status_InvalidSector = 7,
    Iap_Status_SectorNotBlank = 8,
    Iap_Status_SectorNotPreparedForWriteOperation = 9,
    Iap_Status_CompareError = 10,
    Iap_Status_Busy = 11
} Iap_Status;

/** Returns a pointer to the specific flash bank, this function is mostly for
 *  internal use.
 *  @param sector Specific sector in the flash memory.
 *  @param offset Offset from the beginning of the sector.
 *  @return Pointer to an address in the flash memory.
 */
void *Iap_getPointer(uint32 sector, uint32 offset);
/** Reads data from the flash memory.
 *  @param sector Specific sector in the flash memory.
 *  @param offset Offset from the beginning of the sector.
 *  @param buffer Pointer to the address where the data has to be stored.
 *  @param size   Size of the data to read.
 *  @return  Returns -1 on error or one of the IapStatusCodes if successful.
 */
int32 Iap_read(uint32 sector, uint32 offset, void *buffer, uint32 size);
/** Writes data to the flash memory.
 *  @param sector Specific sector in the flash memory.
 *  @param offset Offset from the beginning of the sector.
 *  @param buffer Pointer to the address where the data is stored.
 *  @param size   Size of the data to write.
 *  @return  Returns -1 on error or one of the IapStatusCodes if successful.
 */
int32 Iap_write(uint32 sector, uint32 offset, const void *buffer, uint32 size);
/** Compares data from the flash memory with data in the RAM.
 *  @param sector Specific sector in the flash memory.
 *  @param offset Offset from the beginning of the sector.
 *  @param buffer Pointer to the address where the data is stored.
 *  @param size   Size of the data to compare.
 *  @return  Returns -1 on error or one of the IapStatusCodes if successful.
 */
int32 Iap_compare(uint32 sector, uint32 offset, const void *buffer, uint32 size);
/** Writes data to the flash memory.
 *  @param destinationSector Destination sector in the flash memory.
 *  @param sourceSector Source sector in the flash memory.
 *  @param offset Offset from the beginning of the sector.
 *  @param size   Size of the data to copy.
 *  @return  Returns -1 on error or one of the IapStatusCodes if successful.
 */
int32 Iap_copy(uint32 destinationSector, uint32 sourceSector, uint32 offset, uint32 size);
/** Erases one sector in the flash memory
 *  @param sector The sector the erase.
 *  @return  Returns -1 on error or one of the IapStatusCodes if successful.
 */
int32 Iap_erase(uint32 sector);
/** Checks wheter an sector in the fash memory is empty or not.
 *  @param sector The sector the check.
 *  @return  Returns -1 on error or one of the IapStatusCodes if successful.
 */
int32 Iap_checkBlank(uint32 sector);

/** Reads the serial number of the device from the flash memory.
 *  @return Serialnumber
 **/
int32 Iap_readSerial(void);
/** Reads the version number of the IAP code from the flash memory.
 * @return Version number
 */
int32 Iap_readVersion(void);
/** Reads the id of the IAP code from the flash memory.
 * @return Id
 */
int32 Iap_readId(void);

/** Work in progress */
void  Iap_flashFirmware(const void* data, uint32 size);

/** Saves application data to the flash memory. Sector 27 is used for this purpose.
 * @param Pointer to the data in RAM.
 * @param size Size of the data.
 * @return Return -1 on error or one of the IapStatusCodes if sucsessful.
 */
int8  Iap_saveApplicationSettings(void* data, uint32 size);
/** Loads application form the flash memory. Sector 27 is used for this purpose.
 * @param Pointer to the data in RAM.
 * @param size Size of the data.
 * @return Return -1 on error or one of the IapStatusCodes if sucsessful.
 */
int8  Iap_loadApplicationSettings(void* data, uint32 size);

/**
 * @}
 */
