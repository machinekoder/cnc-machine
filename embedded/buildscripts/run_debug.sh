#!/bin/bash
#kill previous openocd session
PID=`ps aux | grep '[o]penocd' | awk '{print $2}'`
echo $PID
if [ "$PID" != "" ]; then
  kill $PID
fi
#run openocd
openocd -f DbgCfg/LPC1758_OOCD_CMARM.cfg &
sleep 1

#run telnet
{ echo "target remote localhost:3333"; echo "monitor halt"; echo "load firmware.elf"; sleep 30; echo "continue"; } | arm-none-eabi-gdb -q firmware.elf
