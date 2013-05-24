#!/bin/bash
#kill previous openocd session
PID=`ps aux | grep '[o]penocd' | awk '{print $2}'`
if [ "$PID" != "" ]; then
  kill $PID
fi
#run openocd
#cd ´pwd´
openocd -f DbgCfg/LPC1758_OOCD_CMARM.cfg &

sleep 1
#run telnet
{ echo "halt"; echo "flash erase_sector 0 0 26"; echo "flash write_image firmware.bin 0"; sleep 30; echo "exit"; } | telnet localhost 4444
sleep 1

#kill openocd session again
PID=`ps aux | grep '[o]penocd' | awk '{print $2}'`
if [ "$PID" != "" ]; then
  kill $PID
fi
