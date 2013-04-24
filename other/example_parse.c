void processCommand(char *buffer)
{
    char *dataPointer;
    char *savePointer;
    
    Led_set(LedYellow);  // set the yellow led to indicate incoming data status
    
    dataPointer = strtok_r(buffer, " ", &savePointer);
    
    if (compareBaseCommand("alive", dataPointer))
    {
        // We have a keep alive command
        printAliveMessage();
    }
    else if (compareBaseCommand("run", dataPointer))
    {
        // We have a run command
        dataPointer = strtok_r(NULL, " ", &savePointer);
        if (dataPointer != NULL)
        {
            uint16 commandSize = strlen(dataPointer);
            uint16 i;
            for (i = 0; i < commandSize; i+=2)
            {
                ((char*)(&currentCommand))[i/2] = (char)hex2int(dataPointer+i,2);
            }
        }
        startState(ApplicationState_RunCommand);
    }
    else if (compareBaseCommand("capture", dataPointer))
    {
        dataPointer = strtok_r(NULL, " ", &savePointer);
        if (dataPointer == NULL)
        {
            printUnknownCommand();
            return;
        }
        else if (compareExtendedCommand("ir",dataPointer))
        {
            startState(ApplicationState_CaptureIrCommand);
        }
        else if (compareExtendedCommand("radio433", dataPointer))
        {
            startState(ApplicationState_CaptureRadio433MhzCommand);
        }
        else if (compareExtendedCommand("radio868", dataPointer))
        {
            startState(ApplicationState_CaptureRadio868MhzCommand);
        }
        else
        {
            printUnknownCommand();
            return;
        }
    }
    else if (compareBaseCommand("stop", dataPointer))
    {
        startState(ApplicationState_Idle);
    }
    else if (compareBaseCommand("flash", dataPointer))
    {
        // We have a flash command
        char buffer[100];
        uint16 receivedChecksum;
        uint16 calculatedChecksum;
        
        dataPointer = strtok_r(NULL, " ", &savePointer);
        if (dataPointer != NULL)
        {
            uint16 commandSize = strlen(dataPointer);
            uint16 i;
            for (i = 0; i < commandSize; i+=2)
            {
                buffer[i/2] = (char)hex2int(dataPointer+i,2);
            }
            
            dataPointer = strtok_r(NULL, " ", &savePointer);
            if (dataPointer != NULL)
            {
                receivedChecksum = (uint16)hex2int(dataPointer,4);;
                calculatedChecksum = Crc_fast(buffer, 100);
                if (receivedChecksum == calculatedChecksum)
                    printAcknowledgement();
                else
                    printfData("%u %u %u\r",commandSize,receivedChecksum,calculatedChecksum);
            }
        }
        
    }
    else if (compareBaseCommand("set", dataPointer))
    {
        // starting a set command
        dataPointer = strtok_r(NULL, " ", &savePointer);
        if (dataPointer == NULL)
        {
            printUnknownCommand();
            return;
        }
        else if (compareExtendedCommand("wlan",dataPointer))
        {
            // set wlan
            dataPointer = strtok_r(NULL, " ", &savePointer);
            if (dataPointer == NULL)
            {
                printUnknownCommand();
                return;
            }
            else if (compareExtendedCommand("ssid",dataPointer))
            {
                // set ssid
                dataPointer = strtok_r(NULL, " ", &savePointer);
                if (dataPointer != NULL)
                {
                    strncpy(applicationSettings.wlanSsid, dataPointer, 100);
                    printAcknowledgement();
                    return;
                }
                else
                {
                    printUnknownCommand();
                    return;
                }
            }
            else if (compareExtendedCommand("phrase",dataPointer))
            {
                // set ssid
                dataPointer = strtok_r(NULL, " ", &savePointer);
                if (dataPointer != NULL)
                {
                    strncpy(applicationSettings.wlanPhrase, dataPointer, 100);
                    printAcknowledgement();
                    return;
                }
                else
                {
                    printUnknownCommand();
                    return;
                }
            }
            else if (compareExtendedCommand("key",dataPointer))
            {
                // set ssid
                dataPointer = strtok_r(NULL, " ", &savePointer);
                if (dataPointer != NULL)
                {
                    strncpy(applicationSettings.wlanKey, dataPointer, 100);
                    printAcknowledgement();
                    return;
                }
                else
                {
                    printUnknownCommand();
                    return;
                }
            }
            else if (compareExtendedCommand("hostname",dataPointer))
            {
                // set hostname
                dataPointer = strtok_r(NULL, " ", &savePointer);
                if (dataPointer != NULL)
                {
                    strncpy(applicationSettings.wlanHostname, dataPointer, 100);
                    printAcknowledgement();
                    return;
                }
                else
                {
                    printUnknownCommand();
                    return;
                }
            }
            else if (compareExtendedCommand("auth",dataPointer))
            {
                // set auth
                dataPointer = strtok_r(NULL, " ", &savePointer);
                if (dataPointer != NULL)
                {
                    applicationSettings.wlanAuth = atoi(dataPointer);
                    printAcknowledgement();
                    return;
                }
                else
                {
                    printUnknownCommand();
                    return;
                }
            }
            else if (compareExtendedCommand("dhcp",dataPointer))
            {
                // set auth
                dataPointer = strtok_r(NULL, " ", &savePointer);
                if (dataPointer != NULL)
                {
                    applicationSettings.wlanDhcp = atoi(dataPointer);
                    printAcknowledgement();
                    return;
                }
                else
                {
                    printUnknownCommand();
                    return;
                }
            }
            else if (compareExtendedCommand("ip",dataPointer))
            {
                // set auth
                dataPointer = strtok_r(NULL, " ", &savePointer);
                if (dataPointer != NULL)
                {
                    strncpy(applicationSettings.wlanIp, dataPointer, 20);
                    printAcknowledgement();
                    return;
                }
                else
                {
                    printUnknownCommand();
                    return;
                }
            }
            else if (compareExtendedCommand("mask",dataPointer))
            {
                // set auth
                dataPointer = strtok_r(NULL, " ", &savePointer);
                if (dataPointer != NULL)
                {
                    strncpy(applicationSettings.wlanMask, dataPointer, 20);
                    printAcknowledgement();
                    return;
                }
                else
                {
                    printUnknownCommand();
                    return;
                }
            }
            else if (compareExtendedCommand("gateway",dataPointer))
            {
                // set auth
                dataPointer = strtok_r(NULL, " ", &savePointer);
                if (dataPointer != NULL)
                {
                    strncpy(applicationSettings.wlanGateway, dataPointer, 20);
                    printAcknowledgement();
                    return;
                }
                else
                {
                    printUnknownCommand();
                    return;
                }
            }
            else if (compareExtendedCommand("dns",dataPointer))
            {
                // set auth
                dataPointer = strtok_r(NULL, " ", &savePointer);
                if (dataPointer != NULL)
                {
                    strncpy(applicationSettings.wlanPrimaryDnsAddress, dataPointer, 20);
                    printAcknowledgement();
                    return;
                }
                else
                {
                    printUnknownCommand();
                    return;
                }
            }
            else if (compareExtendedCommand("backupDns",dataPointer))
            {
                // set auth
                dataPointer = strtok_r(NULL, " ", &savePointer);
                if (dataPointer != NULL)
                {
                    strncpy(applicationSettings.wlanSecondaryDnsAddress, dataPointer, 20);
                    printAcknowledgement();
                    return;
                }
                else
                {
                    printUnknownCommand();
                    return;
                }
            }
            else
            {
                printUnknownCommand();
                return;
            }
        }
        else if (compareExtendedCommand("ir",dataPointer))
        {
            // set ir
            dataPointer = strtok_r(NULL, " ", &savePointer);
            if (dataPointer == NULL)
            {
                printUnknownCommand();
                return;
            }
            else if (compareExtendedCommand("receiveTimeout",dataPointer))
            {

                dataPointer = strtok_r(NULL, " ", &savePointer);
                if (dataPointer != NULL)
                {
                    applicationSettings.irReceiveTimeout = atoi(dataPointer)*1000;
                    RemoteControl_setReceiveTimeout(RemoteControl_Medium_Ir, applicationSettings.irReceiveTimeout);
                    printAcknowledgement();
                    return;
                }
                else
                {
                    printParameterMissing();
                    return;
                }
            }
            else if (compareExtendedCommand("sendTimeout",dataPointer))
            {

                dataPointer = strtok_r(NULL, " ", &savePointer);
                if (dataPointer != NULL)
                {
                    applicationSettings.irSendTimeout = atoi(dataPointer)*1000;
                    RemoteControl_setSendTimeout(RemoteControl_Medium_Ir, applicationSettings.irSendTimeout);
                    printAcknowledgement();
                    return;
                }
                else
                {
                    printParameterMissing();
                    return;
                }
            }
            else if (compareExtendedCommand("count",dataPointer))
            {

                dataPointer = strtok_r(NULL, " ", &savePointer);
                if (dataPointer != NULL)
                {
                    applicationSettings.irRepeatCount = atoi(dataPointer);
                    RemoteControl_setRepeatCount(RemoteControl_Medium_Ir, applicationSettings.irRepeatCount);
                    printAcknowledgement();
                    return;
                }
                else
                {
                    printParameterMissing();
                    return;
                }
            }
            else
            {
                printUnknownCommand();
                return;
            }
        }
        else if (compareExtendedCommand("433",dataPointer))
        {
            // set ir
            dataPointer = strtok_r(NULL, " ", &savePointer);
            if (dataPointer == NULL)
            {
                printUnknownCommand();
                return;
            }
            else if (compareExtendedCommand("receiveTimeout",dataPointer))
            {

                dataPointer = strtok_r(NULL, " ", &savePointer);
                if (dataPointer != NULL)
                {
                    applicationSettings.radio433ReceiveTimeout = atoi(dataPointer)*1000;
                    RemoteControl_setReceiveTimeout(RemoteControl_Medium_433Mhz, applicationSettings.radio433ReceiveTimeout);
                    printAcknowledgement();
                    return;
                }
                else
                {
                    printParameterMissing();
                    return;
                }
            }
            else if (compareExtendedCommand("sendTimeout",dataPointer))
            {

                dataPointer = strtok_r(NULL, " ", &savePointer);
                if (dataPointer != NULL)
                {
                    applicationSettings.radio433SendTimeout = atoi(dataPointer)*1000;
                    RemoteControl_setSendTimeout(RemoteControl_Medium_433Mhz, applicationSettings.radio433SendTimeout);
                    printAcknowledgement();
                    return;
                }
                else
                {
                    printParameterMissing();
                    return;
                }
            }
            else if (compareExtendedCommand("count",dataPointer))
            {

                dataPointer = strtok_r(NULL, " ", &savePointer);
                if (dataPointer != NULL)
                {
                    applicationSettings.radio433RepeatCount = atoi(dataPointer);
                    RemoteControl_setRepeatCount(RemoteControl_Medium_433Mhz, applicationSettings.radio433RepeatCount);
                    printAcknowledgement();
                    return;
                }
                else
                {
                    printParameterMissing();
                    return;
                }
            }
            else
            {
                printUnknownCommand();
                return;
            }
        }
        else if (compareExtendedCommand("868",dataPointer))
        {
            // set ir
            dataPointer = strtok_r(NULL, " ", &savePointer);
            if (dataPointer == NULL)
            {
                printUnknownCommand();
                return;
            }
            else if (compareExtendedCommand("receiveTimeout",dataPointer))
            {

                dataPointer = strtok_r(NULL, " ", &savePointer);
                if (dataPointer != NULL)
                {
                    applicationSettings.radio868ReceiveTimeout = atoi(dataPointer)*1000;
                    RemoteControl_setReceiveTimeout(RemoteControl_Medium_868Mhz, applicationSettings.radio868ReceiveTimeout);
                    printAcknowledgement();
                    return;
                }
                else
                {
                    printParameterMissing();
                    return;
                }
            }
            else if (compareExtendedCommand("sendTimeout",dataPointer))
            {

                dataPointer = strtok_r(NULL, " ", &savePointer);
                if (dataPointer != NULL)
                {
                    applicationSettings.radio868SendTimeout = atoi(dataPointer)*1000;
                    RemoteControl_setSendTimeout(RemoteControl_Medium_868Mhz, applicationSettings.radio868SendTimeout);
                    printAcknowledgement();
                    return;
                }
                else
                {
                    printParameterMissing();
                    return;
                }
            }
            else if (compareExtendedCommand("count",dataPointer))
            {

                dataPointer = strtok_r(NULL, " ", &savePointer);
                if (dataPointer != NULL)
                {
                    applicationSettings.radio868RepeatCount = atoi(dataPointer);
                    RemoteControl_setRepeatCount(RemoteControl_Medium_868Mhz, applicationSettings.radio868RepeatCount);
                    printAcknowledgement();
                    return;
                }
                else
                {
                    printParameterMissing();
                    return;
                }
            }
            else
            {
                printUnknownCommand();
                return;
            }
        }
        else
        {
            printUnknownCommand();
            return;
        }
    }
    else if (compareBaseCommand("get", dataPointer))            // starting a get command
    {
        dataPointer = strtok_r(NULL, " ", &savePointer);
        if (dataPointer == NULL)
        {
            printUnknownCommand();
            return;
        }
        else if (compareExtendedCommand("wlan",dataPointer))
        {
            // get wlan
            dataPointer = strtok_r(NULL, " ", &savePointer);
            if (dataPointer == NULL)
            {
                printUnknownCommand();
                return;
            }
            else if (compareExtendedCommand("ssid",dataPointer))
            {
                printfData("%s\r", applicationSettings.wlanSsid);
                printAcknowledgement();
                return;
            }
            else if (compareExtendedCommand("phrase",dataPointer))
            {
                printfData("%s\r", applicationSettings.wlanPhrase);
                printAcknowledgement();
                return;
            }
            else if (compareExtendedCommand("key",dataPointer))
            {
                printfData("%s\r", applicationSettings.wlanKey);
                printAcknowledgement();
                return;
            }
            else if (compareExtendedCommand("hostname",dataPointer))
            {
                printfData("%s\r", applicationSettings.wlanHostname);
                printAcknowledgement();
                return;
            }
            else if (compareExtendedCommand("auth",dataPointer))
            {
                printfData("%u\r", applicationSettings.wlanAuth);
                printAcknowledgement();
                return;
            }
            else if (compareExtendedCommand("dhcp",dataPointer))
            {
                printfData("%u\r", applicationSettings.wlanDhcp);
                printAcknowledgement();
                return;
            }
            else if (compareExtendedCommand("ip",dataPointer))
            {
                printfData("%s\r", applicationSettings.wlanIp);
                printAcknowledgement();
                return;
            }
            else if (compareExtendedCommand("mask",dataPointer))
            {
                printfData("%s\r", applicationSettings.wlanMask);
                printAcknowledgement();
                return;
            }
            else if (compareExtendedCommand("gateway",dataPointer))
            {
                printfData("%s\r", applicationSettings.wlanGateway);
                printAcknowledgement();
                return;
            }
            else if (compareExtendedCommand("dns",dataPointer))
            {
                printfData("%s\r", applicationSettings.wlanPrimaryDnsAddress);
                printAcknowledgement();
                return;
            }
            else if (compareExtendedCommand("backupDns",dataPointer))
            {
                printfData("%s\r", applicationSettings.wlanSecondaryDnsAddress);
                printAcknowledgement();
                return;
            }
            else
            {
                printUnknownCommand();
                return;
            }
        }
        else if (compareExtendedCommand("ir",dataPointer))
        {
            // set ir
            dataPointer = strtok_r(NULL, " ", &savePointer);
            if (dataPointer == NULL)
            {
                printUnknownCommand();
                return;
            }
            else if (compareExtendedCommand("receiveTimeout",dataPointer))
            {
                printfData("%u\r", applicationSettings.irReceiveTimeout/1000);
                printAcknowledgement();
                return;
            }
            else if (compareExtendedCommand("sendTimeout",dataPointer))
            {
                printfData("%u\r", applicationSettings.irSendTimeout/1000);
                printAcknowledgement();
                return;
            }
            else if (compareExtendedCommand("count",dataPointer))
            {
                printfData("%u\r", applicationSettings.irRepeatCount);
                printAcknowledgement();
                return;
            }
            else
            {
                printUnknownCommand();
                return;
            }
        }
        else if (compareExtendedCommand("433",dataPointer))
        {
            // set ir
            dataPointer = strtok_r(NULL, " ", &savePointer);
            if (dataPointer == NULL)
            {
                printUnknownCommand();
                return;
            }
            else if (compareExtendedCommand("receiveTimeout",dataPointer))
            {
                printfData("%u\r", applicationSettings.radio433ReceiveTimeout/1000);
                printAcknowledgement();
                return;
            }
            else if (compareExtendedCommand("sendTimeout",dataPointer))
            {
                printfData("%u\r", applicationSettings.radio433SendTimeout/1000);
                printAcknowledgement();
                return;
            }
            else if (compareExtendedCommand("count",dataPointer))
            {
                printfData("%u\r", applicationSettings.radio433RepeatCount);
                printAcknowledgement();
                return;
            }
            else
            {
                printUnknownCommand();
                return;
            }
        }
        else if (compareExtendedCommand("868",dataPointer))
        {
            // set ir
            dataPointer = strtok_r(NULL, " ", &savePointer);
            if (dataPointer == NULL)
            {
                printUnknownCommand();
                return;
            }
            else if (compareExtendedCommand("receiveTimeout",dataPointer))
            {
                printfData("%u\r", applicationSettings.radio868ReceiveTimeout/1000);
                printAcknowledgement();
                return;
            }
            else if (compareExtendedCommand("sendTimeout",dataPointer))
            {
                printfData("%u\r", applicationSettings.radio868SendTimeout/1000);
                printAcknowledgement();
                return;
            }
            else if (compareExtendedCommand("count",dataPointer))
            {
                printfData("%u\r", applicationSettings.radio868RepeatCount);
                printAcknowledgement();
                return;
            }
            else
            {
                printUnknownCommand();
                return;
            }
        }
        else
        {
            printUnknownCommand();
            return;
        }
    }
    else if (compareBaseCommand("start", dataPointer))
    {
        // starting a start command
        dataPointer = strtok_r(NULL, " ", &savePointer);
        if (dataPointer == NULL)
        {
            printUnknownCommand();
            return;
        }
        else if (compareExtendedCommand("wlan",dataPointer))
        {
            // set wlan
            dataPointer = strtok_r(NULL, " ", &savePointer);
            if (dataPointer == NULL)
            {
                printUnknownCommand();
                return;
            }
            else if (compareExtendedCommand("adhoc",dataPointer))
            {
                if (startWlanAdhocMode() == 0)
                    printAcknowledgement();
                else
                    printError("entering adhoc mode failed");
                    
                return;
            }
            else if (compareExtendedCommand("infrastructure",dataPointer))
            {
                if (startWlanInfrastructureMode(&applicationSettings) == 0)
                    printAcknowledgement();
                else
                    printError("entering infrastructure mode failed");
                    
                return;
            }
            else
            {
                printUnknownCommand();
                return;
            }
        }
        else if (compareExtendedCommand("flash", dataPointer))
        {
            startState(ApplicationState_FlashFirmware);
            printAcknowledgement();
            
            return;
        }
        else
        {
            printUnknownCommand();
            return;
        }
    }
    else if (compareBaseCommand("save", dataPointer))
    {
        // starting a save command
        dataPointer = strtok_r(NULL, " ", &savePointer);
        if (dataPointer == NULL)
        {
            printUnknownCommand();
            return;
        }
        else if (compareExtendedCommand("config",dataPointer))
        {
            // save config
            if (Iap_saveApplicationSettings(&applicationSettings, sizeof(ApplicationSettings)) == 0)
            {
                printAcknowledgement();
            }
            else
            {
                printError("saving settings failed");
            }
        }
        else
        {
            printUnknownCommand();
            return;
        }
    }
    else if (compareBaseCommand("test", dataPointer))
    {
        dataPointer = strtok_r(NULL, " ", &savePointer);
        if (dataPointer == NULL)
        {
            printUnknownCommand();
            return;
        }
        else if (compareExtendedCommand("wifly",dataPointer))
        {
            startState(ApplicationState_WiFlyTest);
        }
        else if (compareExtendedCommand("response",dataPointer))
        {
            WiFly_actionEnterCommandMode(false);
            WiFly_getAdhoc();
            printfData(WiFly_getResponse());
            WiFly_actionExitCommandMode();
        }
        else
        {
            printUnknownCommand();
            return;
        }
    }
    else
    {
        printUnknownCommand();
    }
}
 
