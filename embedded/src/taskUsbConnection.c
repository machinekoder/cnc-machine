#include "taskUsbConnection.h"
#include "app.h"

#define COMMAND_BUFFER_SIZE 200u

void commandSplitter(char* data, uint32 length);
void processCommand(char *buffer);

void App_TaskUsbConnection (void *p_arg)
{
    OS_ERR       err;
    (void)p_arg;                                             /* Prevent Compiler Warning */

    if(CSP_IntVectReg((CSP_DEV_NBR   )CSP_INT_CTRL_NBR_MAIN,
                      (CSP_DEV_NBR   )CSP_INT_SRC_NBR_USB_00,
                      (CPU_FNCT_PTR  )USB_IRQHandler,
                      (void         *)0) != DEF_OK) {
        while(DEF_TRUE);
    }                                                                                             /* register Interrupt Handler in RTOS */

    CSP_IntEn(CSP_INT_CTRL_NBR_MAIN, CSP_INT_SRC_NBR_USB_00);   /* Enable USB Interrupt. */

    USBHwConnect(TRUE);

    (void)p_arg;                                              /* Prevent Compiler Warning */
    while(DEF_TRUE)
    {
        if(usbReceiveBufferSize > 0)
        {   /* if a Message was received */
            commandSplitter((char*)&usbReceiveBuffer[2], usbReceiveBufferSize);
            usbReceiveBufferSize = 0;                                    /* reset the Message length of the incoming buffer */
        }
        OSTimeDlyHMSM(0u, 0u, 0u, 100u, OS_OPT_TIME_HMSM_STRICT, &err);

    }
}

void commandSplitter(char* data, uint32 length)
{
    uint32 i;
    uint32 commandBufferPos = 0;
    static char commandBuffer[COMMAND_BUFFER_SIZE];

    for (i = 0; i < length; ++i)
    {
        if (data[i] == '\n')
        {
            processCommand(commandBuffer);
            commandBufferPos = 0;
        }
        else
        {
            commandBuffer[commandBufferPos] = data[i];
            commandBufferPos++;
        }
    }
}

void printUnknownCommand(void)
{
    USB_printf("CMD?\n");
}

void printParameterMissing(void)
{
    USB_printf("Missing parameter.\n");
}

void printAcknowledgement(void)
{
    USB_printf("ACK\n");
}

void printError(char *message)
{
    USB_printf("ERR: %s\n", message);
}

void printAliveMessage(void)
{
    USB_printf("yes\n");
}

void printOk(void)
{
    USB_printf("ok\n");
}

bool compareBaseCommand(char *original, char *received)
{
    return (strcmp(original,received) == 0);
}

bool compareExtendedCommand(char *original, char *received)
{
    return (((strlen(received) == 1) && (strncmp(original,received,1) == 0)) ||
            (strcmp(original,received) == 0));
}

bool parseParameter(char *rawData, int32 *x, int32 *y, int32 *z, uint32 *f)
{
    switch (rawData[0])
    {
    case 'X':
        *x = (int32)(atof(&rawData[1])*1000.0);
        return TRUE;
    case 'Y':
        *y = (int32)(atof(&rawData[1])*1000.0);
        return TRUE;
    case 'Z':
        *z = (int32)(atof(&rawData[1])*1000.0);
        return TRUE;
    case 'F':
        *f = (uint32)(atoi(&rawData[1]));
        return TRUE;
    default:
        return FALSE;
    }

    return FALSE;
}

void processCommand(char *buffer)
{
    char *dataPointer;
    char *dataPointer1;
    char *dataPointer2;
    char *dataPointer3;
    char *savePointer;

    dataPointer = strtok_r(buffer, " ", &savePointer);

    if (compareBaseCommand("G00", dataPointer))
    {
        uint8 commandCount = 0;
        int32 x = currentX;
        int32 y = currentY;
        int32 z = currentZ;

        if ((dataPointer = strtok_r(NULL, " ", &savePointer)) == NULL)
        {
            printUnknownCommand();
            return;
        }
        else if ((dataPointer1 = strtok_r(NULL, " ", &savePointer)) == NULL)
        {
            commandCount = 1;
        }
        else if ((dataPointer2 = strtok_r(NULL, " ", &savePointer)) == NULL)
        {
            commandCount = 2;
        }
        else if ((dataPointer3 = strtok_r(NULL, " ", &savePointer)) == NULL)
        {
            commandCount = 3;
        }
        else
        {
            printUnknownCommand();
            return;
        }

        parseParameter(dataPointer, &x, &y, &z, NULL);
        if (commandCount > 1)
        {
            parseParameter(dataPointer1, &x, &y, &z, NULL);
        }
        if (commandCount > 2)
        {
            parseParameter(dataPointer2, &x, &y, &z, NULL);
        }

        App_putIntoCommandPuffer(x, y, z, MAX_FEED_RATE);

        return;
    }
    else if (compareBaseCommand("G01", dataPointer))
    {
        uint8 commandCount = 0;
        int32 x = currentX;
        int32 y = currentY;
        int32 z = currentZ;
        uint32 feed = 0u;    // makes it current speed at the time of execution

        if ((dataPointer = strtok_r(NULL, " ", &savePointer)) == NULL)
        {
            printUnknownCommand();
            return;
        }
        else if ((dataPointer1 = strtok_r(NULL, " ", &savePointer)) == NULL)
        {
            commandCount = 1;
        }
        else if ((dataPointer2 = strtok_r(NULL, " ", &savePointer)) == NULL)
        {
            commandCount = 2;
        }
        else if ((dataPointer3 = strtok_r(NULL, " ", &savePointer)) == NULL)
        {
            commandCount = 3;
        }
        else
        {
            commandCount = 4;
            return;
        }

        parseParameter(dataPointer, &x, &y, &z, &feed);
        if (commandCount > 1)
        {
            parseParameter(dataPointer1, &x, &y, &z, &feed);
        }
        if (commandCount > 2)
        {
            parseParameter(dataPointer2, &x, &y, &z, &feed);
        }
        if (commandCount > 3)
        {
            parseParameter(dataPointer3, &x, &y, &z, &feed);
        }

        App_putIntoCommandPuffer(x, y, z, feed);

        return;
    }
    else if (compareBaseCommand("move", dataPointer))
    {
        dataPointer = strtok_r(NULL, " ", &savePointer);
        if (compareExtendedCommand("x",dataPointer))
        {
            // We have a set command
            dataPointer = strtok_r(NULL, " ", &savePointer);
            if (dataPointer != NULL)
            {
                int32 movement = (int32)(atof(dataPointer)*1000.0);
                
                if (movement != 0) 
                {
                    targetX += movement;
                    if (targetX < 0)
                    {
                        targetX = 0;
                    }
                    applicationState = ApplicationState_Working;
                }
            }
            else
            {
                printUnknownCommand();
            }
            return;
        }
        else if (compareExtendedCommand("y",dataPointer))
        {
            // We have a set command
            dataPointer = strtok_r(NULL, " ", &savePointer);
            if (dataPointer != NULL)
            {
                int32 movement = (int32)(atof(dataPointer)*1000.0);
                
                if (movement != 0) 
                {
                    targetY += movement;
                    if (targetY < 0)
                    {
                        targetY = 0;
                    }
                    applicationState = ApplicationState_Working;
                }
            }
            else
            {
                printUnknownCommand();
            }
            return;
        }
        else if (compareExtendedCommand("z",dataPointer))
        {
            // We have a set command
            dataPointer = strtok_r(NULL, " ", &savePointer);
            if (dataPointer != NULL)
            {
                int32 movement = (int32)(atof(dataPointer)*1000.0);
                
                if (movement != 0) 
                {
                    targetZ += movement;
                    if (targetZ < 0)
                    {
                        targetZ = 0;
                    }
                    applicationState = ApplicationState_Working;
                }
            }
            else
            {
                printUnknownCommand();
            }
            return;
        }
        else
        {
            printUnknownCommand();
            return;
        }
    }
    else if (compareBaseCommand("set", dataPointer))
    {
        dataPointer = strtok_r(NULL, " ", &savePointer);
        if (compareExtendedCommand("x",dataPointer))
        {
            // We have a set command
            dataPointer = strtok_r(NULL, " ", &savePointer);
            if (dataPointer != NULL)
            {
                int32 newTarget = (int32)(atof(dataPointer)*1000.0);;
                
                if (targetX != newTarget)
                {
                    targetX = newTarget;
                    if (targetX < 0)
                    {
                        targetX = 0;
                    }
                    applicationState = ApplicationState_Working;
                }
            }
            else
            {
                printUnknownCommand();
            }
            return;
        }
        else if (compareExtendedCommand("y",dataPointer))
        {
            // We have a set command
            dataPointer = strtok_r(NULL, " ", &savePointer);
            if (dataPointer != NULL)
            {
                int32 newTarget = (int32)(atof(dataPointer)*1000.0);;
                
                if (targetY != newTarget)
                {
                    targetY = newTarget;
                    if (targetY < 0)
                    {
                        targetY = 0;
                    }
                    applicationState = ApplicationState_Working;
                }
            }
            else
            {
                printUnknownCommand();
            }
            return;
        }
        else if (compareExtendedCommand("z",dataPointer))
        {
            // We have a set command
            dataPointer = strtok_r(NULL, " ", &savePointer);
            if (dataPointer != NULL)
            {
                int32 newTarget = (int32)(atof(dataPointer)*1000.0);;
                
                if (targetZ != newTarget)
                {
                    targetZ = newTarget;
                    if (targetZ < 0)
                    {
                        targetZ = 0;
                    }
                    applicationState = ApplicationState_Working;
                }
            }
            else
            {
                printUnknownCommand();
            }
            return;
        }
        else
        {
            printUnknownCommand();
            return;
        }
    }
    else if (compareBaseCommand("home", dataPointer))
    {
        dataPointer = strtok_r(NULL, " ", &savePointer);
        if (compareExtendedCommand("x",dataPointer))
        {
            if (applicationState == ApplicationState_Idle)
            {
                homeX();
            }
            return;
        }
        else if (compareExtendedCommand("y",dataPointer))
        {
            if (applicationState == ApplicationState_Idle)
            {
                homeY();
            }
            return;
        }
        else if (compareExtendedCommand("z",dataPointer))
        {
            if (applicationState == ApplicationState_Idle)
            {
                homeZ();
            }
            return;
        }
        else if (compareExtendedCommand("all",dataPointer))
        {
            if (applicationState == ApplicationState_Idle)
            {
                homeAll();
            }
            return;
        }
        else
        {
            printUnknownCommand();
            return;
        }
    }
    else if (compareBaseCommand("calibrate", dataPointer))
    {
        if (((dataPointer = strtok_r(NULL, " ", &savePointer)) == NULL) ||
                ((dataPointer1 = strtok_r(NULL, " ", &savePointer)) == NULL) ||
                ((dataPointer2 = strtok_r(NULL, " ", &savePointer)) == NULL) )
            return;
        
        if (applicationState == ApplicationState_Idle)
        {
            cncCalibrateZentool(atof(dataPointer), atof(dataPointer1), atof(dataPointer2));
        }

        return;
    }
    else if (compareBaseCommand("stop", dataPointer))
    {
        stopMachine();
        return;
    }
    else if (compareBaseCommand("test", dataPointer))
    {
        testing = TRUE;
        return;
    }
    else
    {
        printUnknownCommand();
    }
}
