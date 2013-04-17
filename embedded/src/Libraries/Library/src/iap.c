#include "iap.h"

#define CCLK_KHZ SystemCoreClock/1000

static const uint32 flashSectorAddress[] = {
    0x0000,    0x1000,    0x2000,    0x3000,
    0x4000,    0x5000,    0x6000,    0x7000,
    0x8000,    0x9000,    0xa000,    0xb000,
    0xc000,    0xd000,    0xe000,    0xf000,
    0x10000,    0x18000,    0x20000,    0x28000,
    0x30000,    0x38000,    0x40000,    0x48000,
    0x50000,    0x58000,    0x60000,    0x68000,
    0x70000,    0x78000
};

static const uint32 flashSectorSizes[] = {
    0x1000,    0x1000,    0x1000,    0x1000,
    0x1000,    0x1000,    0x1000,    0x1000,
    0x1000,    0x1000,    0x1000,    0x1000,
    0x1000,    0x1000,    0x1000,    0x1000,
    0x8000,    0x8000,    0x8000,    0x8000,
    0x8000,    0x8000,    0x8000,    0x8000,
    0x8000,    0x8000,    0x8000,    0x8000,
    0x8000,    0x8000
};

enum IapCommandCodes{
    IapCommandPrepareSectorForWriteOperation = 50,
    IapCommandCopyRamToFlash = 51,
    IapCommandEraseSector = 52,
    IapCommandBlankCheckSector = 53,
    IapCommandReadPartId = 54,
    IapCommandReadBootCodeVersion = 55,
    IapCommandCompare = 56,
    IapCommandReinvokeIsp = 57,
    IapCommandReadDeviceSerialNumber = 58
};

typedef void (*iapEntry_t)(uint32 [], uint32 []);

static iapEntry_t iapEntry = (iapEntry_t)0x1fff1ff1;

int32 Iap_readId(void)
{
    uint32 cmd[5];
    uint32 res[5];
    
    cmd[0]= IapCommandReadPartId;
    iapEntry(cmd,res);
    
    return ((int32)res[1]);
}

int32 Iap_readVersion(void)
{
    uint32 cmd[5];
    uint32 res[5];
    
    cmd[0] = IapCommandReadBootCodeVersion;
    iapEntry(cmd,res);
    
    return ((int32)res[1]);
}

int32 Iap_readSerial(void)
{
    uint32 cmd[5];
    uint32 res[5];
    
    cmd[0] = IapCommandReadDeviceSerialNumber;
    iapEntry(cmd,res);
    
    return ((int32)res[1]);
}

int32 Iap_checkBlank(uint32 sector)
{
    uint32 cmd[5];
    uint32 res[5];
    
    cmd[0] = IapCommandBlankCheckSector;
    cmd[1] = sector;
    cmd[2] = sector;
    iapEntry(cmd,res);
    
    return ((int32)res[0]);
}

static int32 prepareIap(uint32 sector)
{
    uint32 command[5];
    uint32 result[5];
    
    command[0] = IapCommandPrepareSectorForWriteOperation;
    command[1] = sector;
    command[2] = sector;
    iapEntry(command,result);
    
#ifdef DEBUG_IAP
    if (result[0] != IapStatusCommandSuccess)
    {
        printfUart0("prepareIap result: %d\n",result[0]);
    }
#endif
    
    return ((int32)result[0]);
}

int32 Iap_erase(uint32 sector)
{
    uint32 command[5];
    uint32 result[5];
    
    prepareIap(sector);
    
    command[0] = IapCommandEraseSector;
    command[1] = sector;
    command[2] = sector;
    command[3] = CCLK_KHZ;
    iapEntry(command,result);
    
#ifdef DEBUG_IAP
    if (result[0])
    {
        printfUart0("eraseIap: result: %d\n",result[0]);
    }
#endif
    
    while (Iap_checkBlank(sector))
    {
        ;
    }
    
    return ((int32)result[0]);
}

#ifdef DEBUG_IAP
static int32 compIap(uint32 sourceAddress, uint32 destinationAddress, uint32 size)
{
    uint32 command[5];
    uint32 result[5];
    
    command[0] = IapCommandCompare;
    command[1] = destinationAddress;
    command[2] = sourceAddress;
    command[3] = size;
    iapEntry(command,result);
    

    if (result[0] != IapStatusCommandSuccess)
    {
        printfUart0("compIap(0x%x,0x%x,0x%x) result: %d,pos[0x%x]\n",
            sourceAddress, destinationAddress, size, result[0], result[1]
        );
    }
    
    return ((int32)result[0]);
}
#endif

int32 Iap_write(uint32 sector, uint32 offset, const void *buffer, uint32 size)
{
    uint32 command[5];
    uint32 result[5];
    char wbuf[256]; //this buffer is to make 256byte boundary, according to IAP requirement
    uint32 i;
    uint32 destinationAddress = flashSectorAddress[sector] + offset;
    
    for (i = 0; i < size; i += sizeof(wbuf))
    {
        uint32 s = size-i;
        prepareIap(sector);
        if (s > sizeof(wbuf))
            s = sizeof(wbuf);
        memset(wbuf, 0, sizeof(wbuf));
        memcpy(wbuf, buffer+i, s);
        
        command[0] = IapCommandCopyRamToFlash;
        command[1] = (uint32)destinationAddress + i;
        command[2] = (uint32)wbuf;
        command[3] = sizeof(wbuf);
        command[4] = CCLK_KHZ;
        iapEntry(command,result);
        
#ifdef DEBUG_IAP
        if (result[0] != IapStatusCommandSuccess)
        {
            printfUart0("cmd[%x,%x,%x],result:%d\n",command[1],command[2],command[3],result[0]);
        }
        
        if (compIap(destinationAddress+i, (uint32)wbuf, sizeof(wbuf)))
        {
            printfUart0("writeIap: Compare went wrong\n");
            return -1;
        }
        
        if (memcmp((const void *)(destinationAddress+i), wbuf, sizeof(wbuf)))
        {
            printfUart0("writeIap failed\n");
            return -1;
        }
#endif
        
        if (result[0] != Iap_Status_CommandSuccess)
            return (int32)result[0];
    }
    
    return ((int32)result[0]);
}

void *Iap_getPointer(uint32 sector, uint32 offset)
{
    uint32 sourceAddress = flashSectorAddress[sector] + offset;
    return (void *)sourceAddress;
}

int32 Iap_read(uint32 sector, uint32 offset, void *buffer, uint32 size)
{
    memcpy(buffer, Iap_getPointer(sector,offset), size);
    return 0;
}

int32 Iap_copy(uint32 destinationSector, uint32 sourceSector, uint32 offset, uint32 size)
{
    uint32 i;
    char buffer[256];
    uint32 s;
    
    for (i = 0; i < size; i += sizeof(buffer))
    {
        s = sizeof(buffer);
        if (i + s > size)
            s = size-i;
        Iap_read(sourceSector, offset+i, buffer, s);
        prepareIap(destinationSector);
        Iap_write(destinationSector, offset+i, buffer, s);
    }
    
    return 0;
}

int32 Iap_compare(uint32 sector, uint32 offset, const void *buffer, uint32 size)
{
    return memcmp(buffer, Iap_getPointer(sector, offset), size);
}

void Iap_flashFirmware(const void* data, uint32 size)
{
    uint8 i;
    
    __disable_irq();            // disable interrupts to avoid memory corruption
    
    for (i = 0; i < 27; i++)    // erase sector 0 to 26 before programming
    {
        Iap_erase(i);
    }
    
    Iap_write(0, 0, data, size);
    
    __enable_irq();
}

int8 Iap_saveApplicationSettings(void* data, uint32 size)
{
    int8 result;
    
    __disable_irq();
    
    result = Iap_erase(27);
    if (result == -1)
    {
        __enable_irq();
        return -1;
    }
    
    result = Iap_write(27,0,data,size);
    
    __enable_irq();
    
    return result;
}

int8 Iap_loadApplicationSettings(void* data, uint32 size)
{
    int8 result;
    
    __disable_irq();
    
    result = Iap_read(27,0,data,size);
    
    __enable_irq();
    
    return result;
}
