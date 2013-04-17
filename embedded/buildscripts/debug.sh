#!/usr/bin/env bash
make
make debug
xterm -e openocd -f DbgCfg/LPC1758_OOCD_CMARM.cfg &
sleep 2
arm-none-eabi-gdb -q --command=DbgCfg/lpc1758.gdb firmware.elf

