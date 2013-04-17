#include "wifly.h"

#define WIFLY_STANDARD_BAUD 9600

#define WIFLY_COMMAND_MODE_ENTER_RETRY_ATTEMPTS 5
#define WIFLY_COMMAND_MODE_GUARD_TIME           250     // ms
#define WIFLY_COMMAND_SETTLE_TIME               20
#define WIFLY_VERSION_LENGTH                    4
#define WIFLY_SET_OK                            "AOK"
#define WIFLY_COMMAND_BUFFER_SIZE               256
#define WIFLY_RESPONSE_BUFFER_SIZE              256
#define WIFLY_RESPONSE_TIMEOUT                  1000

char commChar = '$';
char commCloseString[35];
char commOpenString[35];
char commRemoteString[35];
char wiFlyVersion[WIFLY_VERSION_LENGTH + 3];    // <,>, and \0

char commandBuffer[WIFLY_COMMAND_BUFFER_SIZE];
char responseBuffer[WIFLY_RESPONSE_BUFFER_SIZE];
uint16 responseBufferPos;
Uart wiflyUart;

void (* taskFunctionPointer)(char *);

WiFly_State wiFlyState = WiFly_State_Disconnected;

int8 findInResponse(const char* toMatch, uint32 timeout);
int8 responseMatched(const char* toMatch);
int8 findWiFlyVersion(uint32 timeout);
int8 setCommand(char *command);
int8 getCommand(char* command);
int8 otherCommand(char *command, char* awaitedResponse);

void internalProcessTask(char* command);

int8 WiFly_initialize(Uart uart, uint32 baudrate)
{
    if (Uart_initialize(uart, baudrate) == -1)          // 9600 Baud, 1 Stop bit, No parity, No Hardware flow control
        return -1;
    
    wiflyUart = uart;
    
    Uart_flush(wiflyUart);                              // Trash unusefull chars
    
    if (WiFly_actionEnterCommandMode(FALSE) == -1)      // Configure the wlan module
        return -1;
    
    WiFly_setCommOpen("\a*OPEN*\a");        // Set the open command
    WiFly_setCommClose("\a*CLOS*\a");       // Set the close command
    WiFly_setCommRemote("\a*HELLO*\a");     // Set the remote command
    
    WiFly_actionExitCommandMode();
    
    Uart_setProcessFunction(wiflyUart, &internalProcessTask);
    
    /*actionWiFlyEnterCommandMode();           // Enter command mode
    setWiFlyUartBaud(baudrate);              // Set baudrate instantly to target rate
    fileIoWiFlySaveDefault();
    actionWiFlyReboot();
    WIFLY_SET_BAUDRATE(baudrate);*/
    
    wiFlyState = WiFly_State_Disconnected;
    
    return 0;
}

int8 WiFly_setAdhocBeacon(uint32 ms)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set a b %u\r", ms);
    return setCommand(commandBuffer);
}

int8 WiFly_setAdhocProbe(uint32 num)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set a p %u\r", num);
    return setCommand(commandBuffer);
}

int8 WiFly_setBroadcastAddress(char *address)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set b a %s\r", address);
    return setCommand(commandBuffer);
}

int8 WiFly_setBroadcastInterval(uint32 value)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set b i %u\r", value);
    return setCommand(commandBuffer);
}

int8 WiFly_setBroadcastPort(uint16 port)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set b p %u\r", port);
    return setCommand(commandBuffer);
}

int8 WiFly_setComm(char c)
{
    commChar = c;
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set c %c\r", c);
    return setCommand(commandBuffer);
}

int8 WiFly_setCommClose(char *string)
{
    strncpy(commCloseString, string, 10);
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set c c %s\r", string);
    return setCommand(commandBuffer);
}

int8 WiFly_setCommOpen(char *string)
{
    strncpy(commOpenString, string, 10);
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set c o %s\r", string);
    return setCommand(commandBuffer);
}

int8 WiFly_setCommRemote(char *string)
{
    strncpy(commRemoteString, string, 10);
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set c r %s\r", string);
    return setCommand(commandBuffer);
}

int8 WiFly_setCommIdle(uint32 secs)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set c i %u\r", secs);
    return setCommand(commandBuffer);
}

int8 WiFly_setCommMatch(uint32 value)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set c m %u\r", value);
    return setCommand(commandBuffer);
}

int8 WiFly_setCommSize(uint32 value)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set c s %u\r", value);
    return setCommand(commandBuffer);
}

int8 WiFly_setCommTime(uint32 num)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set c t %u\r", num);
    return setCommand(commandBuffer);
}

int8 WiFly_setDnsAddr(char *addr)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set d a %s\r", addr);
    return setCommand(commandBuffer);
}

int8 WiFly_setDnsName(char *string)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set d n %s\r", string);
    return setCommand(commandBuffer);
}

int8 WiFly_setDnsBackup(char *string)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set d b %s\r", string);
    return setCommand(commandBuffer);
}

int8 WiFly_setFtpAddr(char *addr)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set f a %s\r", addr);
    return setCommand(commandBuffer);
}

int8 WiFly_setFtpDir(char *string)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set f d %s\r", string);
    return setCommand(commandBuffer);
}

int8 WiFly_setFtpFilename(char *file)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set f f %s\r", file);
    return setCommand(commandBuffer);
}

int8 WiFly_setFtpMode(uint32 mask)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set f m %x\r", mask);
    return setCommand(commandBuffer);
}

int8 WiFly_setFtpRemote(uint32 port)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set f r %u\r", port);
    return setCommand(commandBuffer);
}

int8 WiFly_setFtpTime(uint32 value)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set f t %u\r", value);
    return setCommand(commandBuffer);
}

int8 WiFly_setFtpUser(char *name)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set f u %s\r", name);
    return setCommand(commandBuffer);
}

int8 WiFly_setFtpPass(char *pass)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set f p %s\r", pass);
    return setCommand(commandBuffer);
}

int8 WiFly_setIpAddress(char *addr)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set i a %s\r", addr);
    return setCommand(commandBuffer);
}

int8 WiFly_setIpBackup(char *addr)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set i b %s\r", addr);
    return setCommand(commandBuffer);
}

int8 WiFly_setIpDhcp(uint32 value)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set i d %u\r", value);
    return setCommand(commandBuffer);
}

int8 WiFly_setIpFlags(uint32 value)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set i f %x\r", value);
    return setCommand(commandBuffer);
}

int8 WiFly_setIpGateway(char *addr)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set i g %s\r", addr);
    return setCommand(commandBuffer);
}

int8 WiFly_setIpHost(char *addr)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set i h %s\r", addr);
    return setCommand(commandBuffer);
}

int8 WiFly_setIpLocalport(uint32 num)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set i l %u\r", num);
    return setCommand(commandBuffer);
}

int8 WiFly_setIpNetmask(char *value)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set i n %s\r", value);
    return setCommand(commandBuffer);
}

int8 WiFly_setIpProtocol(uint32 value)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set i p %u\r", value);
    return setCommand(commandBuffer);
}

int8 WiFly_setIpRemote(uint32 value)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set i r %u\r", value);
    return setCommand(commandBuffer);
}

int8 WiFly_setIpTcpMode(uint32 mask)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set i t %x\r", mask);
    return setCommand(commandBuffer);
}

int8 WiFly_setOptJointmr(uint32 msecs)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set o j %u\r", msecs);
    return setCommand(commandBuffer);
}

int8 WiFly_setOptFormat(uint32 value)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set o f %u\r", value);
    return setCommand(commandBuffer);
}

int8 WiFly_setOptReplace(char character)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set o r %c\r", character);
    return setCommand(commandBuffer);
}

int8 WiFly_setOptDeviceid(char *string)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set o d %s\r", string);
    return setCommand(commandBuffer);
}

int8 WiFly_setOptPassword(char *string)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set o p %s\r", string);
    return setCommand(commandBuffer);
}

int8 WiFly_setSysAutoconn(uint32 value)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set s a %u\r", value);
    return setCommand(commandBuffer);
}

int8 WiFly_setSysAutosleep(uint32 value)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set s a %u\r", value);
    return setCommand(commandBuffer);
}

int8 WiFly_setSysIofunc(uint32 value)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set s i %u\r", value);
    return setCommand(commandBuffer);
}

int8 WiFly_setSysMask(uint32 mask)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set s m %x\r", mask);
    return setCommand(commandBuffer);
}

int8 WiFly_setSysPrintlvl(uint32 value)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set s p %u\r", value);
    return setCommand(commandBuffer);
}

int8 WiFly_setSysOutput(uint32 value, uint32 mask)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set s o %x %x\r", value, mask);
    return setCommand(commandBuffer);
}

int8 WiFly_setSysSleep(uint32 value)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set s s %u\r", value);
    return setCommand(commandBuffer);
}

int8 WiFly_setSysTrigger(uint32 value)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set s t %u\r", value);
    return setCommand(commandBuffer);
}

int8 WiFly_setSysValue(uint32 value)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set s v %u\r", value);
    return setCommand(commandBuffer);
}

int8 WiFly_setSysWake(uint32 secs)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set s w %u\r", secs);
    return setCommand(commandBuffer);
}

int8 WiFly_setTimeAddress(char *addr)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set t a %s\r", addr);
    return setCommand(commandBuffer);
}

int8 WiFly_setTimePort(uint32 num)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set t p %u\r", num);
    return setCommand(commandBuffer);
}

int8 WiFly_setTimeEnable(uint32 value)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set t e %u\r", value);
    return setCommand(commandBuffer);
}

int8 WiFly_setTimeRaw(uint32 value)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set t r %u\r", value);
    return setCommand(commandBuffer);
}

int8 WiFly_setUartBaud(uint32 rate)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set u b %u\r", rate);
    return setCommand(commandBuffer);
}

int8 WiFly_setUartInstant(uint32 rate)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set u i %u\r", rate);
    return setCommand(commandBuffer);
}

int8 WiFly_setUartRaw(uint32 rate)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set u r %u\r", rate);
    return setCommand(commandBuffer);
}

int8 WiFly_setUartFlow(uint8 flow)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set u f %u\r", flow);
    return setCommand(commandBuffer);
}

int8 WiFly_setUartMode(uint16 mode)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set u m %x\r", mode);
    return setCommand(commandBuffer);
}

int8 WiFly_setUartTx(uint8 enabled)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set u t %u\r", enabled);
    return setCommand(commandBuffer);
}

int8 WiFly_setWlanAuth(uint8 value)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set w a %u\r", value);
    return setCommand(commandBuffer);
}

int8 WiFly_setWlanChannel(uint8 value)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set w c %u\r", value);
    return setCommand(commandBuffer);
}

int8 WiFly_setWlanExtAntenna(uint8 enabled)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set w e %u\r", enabled);
    return setCommand(commandBuffer);
}

int8 WiFly_setWlanJoin(uint8 value)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set w j %u\r", value);
    return setCommand(commandBuffer);
}

int8 WiFly_setWlanHide(uint8 enabled)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set w h %u\r", enabled);
    return setCommand(commandBuffer);
}

int8 WiFly_setWlanKey(char *value)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set w k %s\r", value);
    return setCommand(commandBuffer);
}

int8 WiFly_setWlanLinkmon(uint32 value)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set w l %u\r", value);
    return setCommand(commandBuffer);
}

int8 WiFly_setWlanMask(uint32 mask)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set w m %x\r", mask);
    return setCommand(commandBuffer);
}

int8 WiFly_setWlanNum(uint8 value)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set w n %u\r", value);
    return setCommand(commandBuffer);
}

int8 WiFly_setWlanPhrase(char *string)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set w p %s\r", string);
    return setCommand(commandBuffer);
}

int8 WiFly_setWlanRate(uint8 value)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set w r %u\r", value);
    return setCommand(commandBuffer);
}

int8 WiFly_setWlanSsid(char *string)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set w s %s\r", string);
    return setCommand(commandBuffer);
}

int8 WiFly_setWlanTx(uint8 value)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set w t %u\r", value);
    return setCommand(commandBuffer);
}

int8 WiFly_setWlanWindow(uint32 value)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set w w %u\r", value);
    return setCommand(commandBuffer);
}

int8 WiFly_setQSensor(uint32 mask)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set q s %x\r", mask);
    return setCommand(commandBuffer);
}

int8 WiFly_setQPower(uint8 value)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set q p %u\r", value);
    return setCommand(commandBuffer);
}

int8 WiFly_getAdhoc()
{
    return getCommand("get a\r");
}

int8 WiFly_getBroadcast()
{
    return getCommand("get b\r");
}

int8 WiFly_getCom()
{
    return getCommand("get c\r");
}

int8 WiFly_getDns()
{
    return getCommand("get d\r");
}

int8 WiFly_getEverything()
{
    return getCommand("get e\r");
}

int8 WiFly_getFtp()
{
    return getCommand("get f\r");
}

int8 WiFly_getIp(bool a)
{
    if (a)
    {
        return getCommand("get i a\r");
    }
    else
    {
        return getCommand("get i\r");
    }
}

int8 WiFly_getMac()
{
    return getCommand("get m\r");
}

int8 WiFly_getOption()
{
    return getCommand("get o\r");
}

int8 WiFly_getSys()
{
    return getCommand("get s\r");
}

int8 WiFly_getTime()
{
    return getCommand("get t\r");
}

int8 WiFly_getWlan()
{
    return getCommand("get w\r");
}

int8 WiFly_getUart()
{
    return getCommand("get u\r");
}

int8 WiFly_getVer()
{
    return getCommand("ver\r");
}

int8 WiFly_showBattery()
{
    return getCommand("show b\r");
}

int8 WiFly_showConnection()
{
    return getCommand("show c\r");
}

int8 WiFly_showIo()
{
    return getCommand("show i\r");
}

int8 WiFly_showNet(uint8 n)
{
    return getCommand("show n\r");
}

int8 WiFly_showRssi()
{
    return getCommand("show r\r");
}

int8 WiFly_showStats()
{
    return getCommand("show s\r");
}

int8 WiFly_showTime()
{
    return getCommand("show t\r");
}

int8 WiFly_showQ(uint8 num)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "show q %u\r", num);
    return getCommand(commandBuffer);
}

int8 WiFly_showQ0x1(uint8 mask)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "show q 0x1%u\r", mask);
    return getCommand(commandBuffer);
}

int8 WiFly_actionEnterCommandMode(uint8 isAfterReboot)
{
    uint8 retryCount;
    
    for (retryCount = 0; 
         retryCount < WIFLY_COMMAND_MODE_ENTER_RETRY_ATTEMPTS; 
         retryCount++)
    {
        if (isAfterReboot == 1)
        {
            Timer_delayMs(1000);  // This delay is so characters aren't missed after a reboot.
        }
        
        Timer_delayMs(WIFLY_COMMAND_MODE_GUARD_TIME);
    
        Uart_flush(wiflyUart);
        Uart_printf(wiflyUart, "%c%c%c",commChar,commChar,commChar);  // Print the command chars
        
        Timer_delayMs(WIFLY_COMMAND_MODE_GUARD_TIME);
        
        Uart_printf(wiflyUart, "\r\r");                               // Print 2 carriage return to make shure it has entered command mode

        // This is used to determine whether command mode has been entered
        // successfully.
        Uart_printf(wiflyUart, "ver\r");
        
        if (findInResponse("WiFly Ver ", 1000) == 0)
        {
            return findWiFlyVersion(1000);
        }
    }
    
    return -1;
}

int8 WiFly_actionExitCommandMode()
{
    Uart_flush(wiflyUart);
    Uart_printf(wiflyUart, "exit\r");
    Timer_delayMs(WIFLY_COMMAND_SETTLE_TIME);
    
    if (findInResponse("EXIT", 1000) == 0)
    { 
        return 0;
    }

    return -1;
}

void WiFly_actionCloseTcpConnection()
{
    Uart_printf(wiflyUart, "close\r");
}

void WiFly_actionFactoryReset()
{
    Uart_printf(wiflyUart, "factory RESET\r");
}

void WiFly_actionJoin()
{
    Uart_printf(wiflyUart, "join\r");
}

void WiFly_actionJoinSsid(char *ssid)
{
    Uart_printf(wiflyUart, "join %s\r", ssid);
}

void WiFly_actionJoinNum(uint8 num)
{
    Uart_printf(wiflyUart, "join # %u\r", num);
}

void WiFly_actionLeaveAccesPoint()
{
    Uart_printf(wiflyUart, "leave\r");
}

void WiFly_actionLites()
{
    Uart_printf(wiflyUart, "lites\r");
}

void WiFly_actionLookup(char *hostname)
{
    Uart_printf(wiflyUart, "lookup %s\r", hostname);
}

void WiFly_actionOpenTcpConnection()
{
    Uart_printf(wiflyUart, "open\r");
}

void WiFly_actionOpenTcpConnectionAddr(char *addr, uint16 port)
{
    Uart_printf(wiflyUart, "open %s %u\r", addr, port);
}

void WiFly_actionPing(char *parameters, uint16 num)
{
    Uart_printf(wiflyUart, "ping  %s %u\r", parameters, num);
}

#define WIFLY_SOFTWARE_REBOOT_RETRY_ATTEMPTS 5
int8 WiFly_actionReboot()
{
    uint8 retryCount;
    
    for (retryCount = 0; retryCount < WIFLY_SOFTWARE_REBOOT_RETRY_ATTEMPTS; retryCount++)
    {
        if (WiFly_actionEnterCommandMode(1) == -1)
        {
            return -1;  // If the included retries have failed we give up
        }
        
        Uart_printf(wiflyUart, "reboot\r");
            
        // For some reason the full "*Reboot*" message doesn't always
        // seem to be received so we look for the later "*READY*" message instead.
    
        if (findInResponse("*READY*", 2000))
        {
            wiFlyState = WiFly_State_Disconnected;    // Now it also should be disconnected
            return 0;
        }
    }
    
    return -1;
}

void WiFly_actionScan(uint32 time, bool passive)
{
    if (passive)
        Uart_printf(wiflyUart, "scan %u P\r", time);
    else
        Uart_printf(wiflyUart, "scan %u\r", time);
}

void WiFly_actionSleep()
{
    Uart_printf(wiflyUart, "sleep\r");
}

void WiFly_actionTime()
{
    Uart_printf(wiflyUart, "time\r");
}

void WiFly_fileIoDel(char *name)
{
    Uart_printf(wiflyUart, "del %s\r", name);
}

void WiFly_fileIoLoad(char *name)
{
    Uart_printf(wiflyUart, "load %s\r", name);
}

void WiFly_fileIoLs()
{
    Uart_printf(wiflyUart, "ls\r");
}

int8 WiFly_fileIoSaveDefault()
{
    return otherCommand("save config\r", "Storing in config");
}

int8 WiFly_fileIoSave(char *name)
{
    char buffer[100];
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "save %s\r", name);
    snprintf(buffer, 100, "Storing in %s", name);
    return otherCommand(commandBuffer, buffer);
}

void WiFly_fileIoBootImage(uint16 num)
{
    Uart_printf(wiflyUart, "boot image %u\r", num);
}

void WiFly_fileIoFtpUpdate(char *name)
{
    Uart_printf(wiflyUart, "ftp update %s\r", name);
}

inline int8 WiFly_putchar(char c)
{
    return Uart_putchar(wiflyUart, c);
}

inline int8 WiFly_writeData(void *data, uint32 length)
{
    return Uart_writeData(wiflyUart, data, length);
}

inline int8 WiFly_getchar(char* c)
{
    return Uart_getchar(wiflyUart, c);
}

int8 WiFly_printf(char *format, ...)
{
    static char buffer[WIFLY_PRINTF_BUFFER_SIZE];
    
    va_list arg_ptr;
    uint16 i = 0;
    
    va_start(arg_ptr,format);
    vsnprintf(buffer, WIFLY_PRINTF_BUFFER_SIZE, format, arg_ptr);
    va_end(arg_ptr);
    
    while (buffer[i] != 0)      // Loop through until reach string's zero terminator
    {
        if (WiFly_putchar(buffer[i]) == -1)
            return -1;
        i++;
    }
    
    return 0;
}

void WiFly_setProcessFunction(void (* func)(char *))
{
    taskFunctionPointer = func;
    //WIFLY_SET_PROCESS_FUNCTION(func);
}

void WiFly_setErrorFunction(void (* func)())
{
    Uart_setErrorFunction(wiflyUart, func);
}

inline void WiFly_processTask()
{
    Uart_processTask(wiflyUart);
}

int8 findInResponse(const char* toMatch, uint32 timeout)
{
    uint32 timeoutTarget;
    uint16 offset;
    char   byteRead;
    
    responseBufferPos = 0;                                  //clear the responseBuffer
    
    for (offset = 0; offset < strlen(toMatch); offset++)
    {
        timeoutTarget = Timeout_msecs() + timeout;
        
        while (WiFly_getchar(&byteRead) != 0)
        {
            // Wait with timeout
            if (timeout > 0)
            {
                if (Timeout_msecs() > timeoutTarget)
                {
                    return -1;
                }
            }
            Timer_delayMs(2); // Improves reliability, 1ms just makes this thing freak out
        }
        
        Timer_delayMs(10);    // Seems to be necessary, don't know why
        
        if (byteRead != toMatch[offset]) {
            offset = 0;
            
            responseBuffer[responseBufferPos] = byteRead;   // Store unmachtech bytes
            if (responseBufferPos < WIFLY_RESPONSE_BUFFER_SIZE-1)
                responseBufferPos++;
            else
                responseBufferPos = 0;
            
            // Ignore character read if it's not a match for the start of the string
            if (byteRead != toMatch[offset])
            {
                offset = -1;
            }
            continue;
        }
    }
    
    responseBuffer[responseBufferPos] = '\0';   // Add the string terminator
    return 0;
}


int8 findWiFlyVersion(uint32 timeout)
{
    uint32 timeoutTarget;
    char   byteRead;    
    uint8 i;
    
    for (i = 0; i < WIFLY_VERSION_LENGTH; i++)
    {
        timeoutTarget = Timeout_msecs() + timeout;
        
        while (WiFly_getchar(&byteRead) != 0)
        {
            // Wait with timeout
            if (timeout > 0)
            {
                if (Timeout_msecs() > timeoutTarget)
                {
                    return -1;
                }
            }
            Timer_delayMs(1); // Improves reliability
        }
        
        wiFlyVersion[1 + i] = byteRead;
    }
    wiFlyVersion[0] = '<';
    wiFlyVersion[WIFLY_VERSION_LENGTH + 1] = '>';
    wiFlyVersion[WIFLY_VERSION_LENGTH + 2] = '\0';
    
    return 0;
}

int8 responseMatched(const char* toMatch)
{
    int8 matchFound = 0;
    uint32 timeout;
    uint16 offset;
    char byteRead;
    
    for (offset = 0; offset < strlen(toMatch); offset++)
    {
        timeout = Timeout_msecs();
        while (WiFly_getchar(&byteRead) != 0)
        {
            // Wait, with optional timeout
            if ((Timeout_msecs() - timeout) > 5000)
            {
                return -1;
            }
            Timer_delayMs(1);
        }
        
        if (byteRead != toMatch[offset])
        {
            matchFound = -1;
            break;
        }
    }
    
    return matchFound;
}

int8 setCommand(char* command)
{
    Uart_flush(wiflyUart);          // Empty buffers
    Uart_printf(wiflyUart, command);  // Send command
    Timer_delayMs(WIFLY_COMMAND_SETTLE_TIME);
    return findInResponse(WIFLY_SET_OK, WIFLY_RESPONSE_TIMEOUT);
}

int8 getCommand(char* command)
{
    Uart_flush(wiflyUart);          // Empty buffers
    Uart_printf(wiflyUart, command);  // Send command
    return findInResponse(wiFlyVersion, WIFLY_RESPONSE_TIMEOUT);
}

int8 otherCommand(char* command, char* awaitedResponse)
{
    Uart_flush(wiflyUart);          // Empty buffers
    Uart_printf(wiflyUart, command);  // Send command
    return findInResponse(awaitedResponse, WIFLY_RESPONSE_TIMEOUT);
}

char* WiFly_getResponse()
{
    return responseBuffer;
}

void internalProcessTask(char *command)
{
    if (command[0] == '*')
    {
        if (command[1] == commOpenString[2])
        {
            wiFlyState = WiFly_State_Connected;
        }
        else if (command[1] == commCloseString[2])
        {
            wiFlyState = WiFly_State_Disconnected;
        }
    }
    else if (wiFlyState == WiFly_State_Connected)
    {
        (*taskFunctionPointer)(command);
    }
    // else it is unnecessary
}

uint8 WiFly_isConnected()
{
    return (wiFlyState == WiFly_State_Connected);
}

int8 WiFly_setAdhocParams()
{
    if (WiFly_setWlanJoin(0) == -1)
        return -1;
    if (WiFly_setWlanAuth(0) == -1)
        return -1;
    if (WiFly_setIpDhcp(2) == -1)
        return -1;
    
    return 0;
}

int8 WiFly_setInfrastructureParams()
{
    if (WiFly_setWlanChannel(0) == -1)
        return -1;
    
    return 0;
}

int8 WiFly_createAdhocNetwork(char *ssid)
{
    if (WiFly_actionEnterCommandMode(FALSE) == -1)
        return -1;
    
    if (WiFly_actionReboot() == -1)
        return -1;
    
    if (WiFly_actionEnterCommandMode(TRUE) == -1)
        return -1;
    
    if (WiFly_setWlanAuth(0) == -1)
        return -1;
    
    if (WiFly_setWlanJoin(4) == -1)  // Turn on Adhoc mode
        return -1;
    
    if (WiFly_setWlanSsid(ssid) == -1)   // Set SSID of Adhoc Network
        return -1;
    
    if (WiFly_setWlanChannel(1) == -1)   // Set Channel for Adhoc Network
        return -1;
    
    if (WiFly_setIpAddress("169.254.1.1") == -1) // Set Ip for Adhoc Network
        return -1;
    
    if (WiFly_setIpNetmask("255.255.0.0") == -1)
        return -1;
    
    if (WiFly_setIpDhcp(0) == -1)
        return -1;
    
    if (WiFly_setCommRemote(0) == -1)    // Turn off remote message
        return -1;
    
    if (WiFly_fileIoSaveDefault() == -1)
        return -1;
    
    if (WiFly_actionReboot() == -1)
        return -1;
    
    return 0;
}
