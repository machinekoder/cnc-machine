################################################################################
# Makefile for the LPC1758 / eStick2 by Martin Horauer
#

################################################################################
# Sytem files
SRC  = usb_app.c
SRC += 

################################################################################
#don't edit below

INCPATH  = -I"Libraries/BSP"
INCPATH += -I"Libraries/BSP/LCD"
INCPATH += -I"Libraries/CFG"
INCPATH += -I"Libraries/CM3/inc"
INCPATH += -I"Libraries/serial_lite"
INCPATH += -I"Libraries/uC-CPU"
INCPATH += -I"Libraries/uC-CSP"
INCPATH += -I"Libraries/uC-CSP/LPC17xx"
INCPATH += -I"Libraries/uC-CSP/LPC17xx/USB/inc"
INCPATH += -I"Libraries/uC-LIB"
INCPATH += -I"Libraries/uC-OSIII"
INCPATH += -I"Libraries/uC-Serial"
INCPATH += -I"Libraries/uC-Probe"

ASRC += Libraries/CM3/src/startup_LPC17xx.asm
ASRC += Libraries/uC-CPU/cpu_a.asm
ASRC += Libraries/uC-CPU/os_cpu_a.asm

SSRC += Libraries/uC-CPU/os_cpu_c.c
SSRC += Libraries/uC-CPU/cpu_c.c
SSRC += Libraries/uC-CPU/cpu_core.c
SSRC += Libraries/BSP/LCD/Images/img_tbl.c
SSRC += Libraries/BSP/LCD/Images/img_nxp_logo.c
SSRC += Libraries/BSP/LCD/Images/img_os_logo.c
SSRC += Libraries/BSP/LCD/Images/img_micirum_logo.c
SSRC += Libraries/BSP/LCD/Images/img_keil_logo.c
SSRC += Libraries/BSP/LCD/Fonts/font_terminal_8x8.c
SSRC += Libraries/BSP/LCD/Fonts/font_tbl.c
SSRC += Libraries/BSP/LCD/Fonts/font_comic_18x18.c
SSRC += Libraries/BSP/LCD/Fonts/font_swiss_16x16.c
SSRC += Libraries/BSP/LCD/bsp_lcd.c
SSRC += Libraries/BSP/cpu_bsp.c
SSRC += Libraries/BSP/bsp.c
SSRC += Libraries/CM3/src/hw_timer.c
SSRC += Libraries/CM3/src/core_cm3.c
SSRC += Libraries/CM3/src/lpc17xx_nvic.c
SSRC += Libraries/CM3/src/system_LPC17xx.c
SSRC += Libraries/CM3/src/libnosys_gnu.c
SSRC += Libraries/CM3/src/NVICInit.c
SSRC += Libraries/uC-LIB/lib_math.c
SSRC += Libraries/uC-LIB/lib_ascii.c
SSRC += Libraries/uC-LIB/lib_str.c
SSRC += Libraries/uC-LIB/lib_mem.c
SSRC += Libraries/uC-OSIII/os_prio.c
SSRC += Libraries/uC-OSIII/os_tmr.c
SSRC += Libraries/uC-OSIII/os_var.c
SSRC += Libraries/uC-OSIII/os_pend_multi.c
SSRC += Libraries/uC-OSIII/os_task.c
SSRC += Libraries/uC-OSIII/os_time.c
SSRC += Libraries/uC-OSIII/os_sem.c
SSRC += Libraries/uC-OSIII/os_flag.c
SSRC += Libraries/uC-OSIII/os_stat.c
SSRC += Libraries/uC-OSIII/os_cfg_app.c
SSRC += Libraries/uC-OSIII/os_mutex.c
SSRC += Libraries/uC-OSIII/os_q.c
SSRC += Libraries/uC-OSIII/os_dbg.c
SSRC += Libraries/uC-OSIII/os_core.c
SSRC += Libraries/uC-OSIII/os_tick.c
SSRC += Libraries/uC-OSIII/os_msg.c
SSRC += Libraries/uC-OSIII/os_mem.c
SSRC += Libraries/uC-OSIII/os_int.c
SSRC += Libraries/CFG/os_app_hooks.c
SSRC += Libraries/serial_lite/os_serial_lite.c
SSRC += Libraries/uC-CSP/os_csp.c
SSRC += Libraries/uC-CSP/csp.c
SSRC += Libraries/uC-CSP/LPC17xx/lpc17xx_clkpwr.c
SSRC += Libraries/uC-CSP/LPC17xx/csp_gpio.c
SSRC += Libraries/uC-CSP/LPC17xx/csp_tmr.c
SSRC += Libraries/uC-CSP/LPC17xx/lpc17xx_pinsel.c
SSRC += Libraries/uC-CSP/LPC17xx/lpc17xx_gpio.c
SSRC += Libraries/uC-CSP/LPC17xx/lpc17xx_emac.c
SSRC += Libraries/uC-CSP/LPC17xx/lpc17xx_uart.c
SSRC += Libraries/uC-CSP/LPC17xx/lpc17xx_ssp.c
SSRC += Libraries/uC-CSP/LPC17xx/csp_int.c
SSRC += Libraries/uC-CSP/LPC17xx/csp_pm.c
SSRC += Libraries/uC-CSP/LPC17xx/csp_dma.c
SSRC += Libraries/uC-CSP/LPC17xx/USB/src/intenable.c
SSRC += Libraries/uC-CSP/LPC17xx/USB/src/usbcontrol.c
SSRC += Libraries/uC-CSP/LPC17xx/USB/src/usbhw_lpc.c
SSRC += Libraries/uC-CSP/LPC17xx/USB/src/usbinit.c
SSRC += Libraries/uC-CSP/LPC17xx/USB/src/usblibs.c
SSRC += Libraries/uC-CSP/LPC17xx/USB/src/usbstdreq.c

LDFILE = LPC1758.ld
LDPATH = Linker

################################################################################
# definitions
TARGET = firmware
MCU = cortex-m3
FORMAT = binary

CROSS_COMPILE = arm-none-eabi-
AS = $(CROSS_COMPILE)gcc
CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)gcc
SIZE = $(CROSS_COMPILE)size
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
FLASHTOOL = flash_lpc1343.sh

OPT = 0
DEBUG = dwarf-2

OPTFLAGS = -O$(OPT)
CFLAGS += -fmessage-length=0
CFLAGS += -Wall
CFLAGS += -Wa,-adhlns=$(<:%.c=%.o.lst)
CFLAGS += -g$(DEBUG)

ALL_CFLAGS = $(INCPATH) $(CFLAGS) $(OPTFLAGS) -mcpu=$(MCU) -mthumb -g3 
ALL_CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)"

ALL_AFLAGS = -x assembler-with-cpp -Wall -Wa,-adhlns="$(<:%.asm=%.o.lst)"
ALL_AFLAGS += -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" 
ALL_AFLAGS += -mcpu=$(MCU) -mthumb -g3 -g$(DEBUG)

################################################################################
# Compile: create object files from C source files.

all: start $(TARGET).elf $(TARGET).hex $(TARGET).lst $(TARGET).siz end


start:
	@echo "--------------------------------------------------------------------"
	@echo "Starting Build process ..."
	@echo "--------------------------------------------------------------------"

end:
	@echo "--------------------------------------------------------------------"
	@echo "End of Build process ..."
	@echo "--------------------------------------------------------------------"

# Compile: create object files from C source files.
.IGNORE: %.asm
%.o : %.c
	@echo ""
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
	@echo "Building file: $(@:%.o=%.c)"
	$(CC) -c $(ALL_CFLAGS) -o $@ $(@:%.o=%.c)
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
	@echo ""

# Compile: create object files from ASM source files.
.IGNORE: %.c
%.o : %.asm
	@echo ""
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
	@echo "Building file: $(@:%.o=%.asm)"
	$(CC) $(ALL_AFLAGS) -o $@ $(@:%.o=%.asm)
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
	@echo ""


# Link: create ELF output file from object files.
.SECONDARY : $(TARGET).elf
.PRECIOUS : $(OBJ)
$(TARGET).elf : $(SRC:%.c=%.o) $(SSRC:%.c=%.o) $(ASRC:%.asm=%.o)
	@echo ""
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~"
	@echo "Linking file ... $<"
	$(LD) -T"./$(LDPATH)/$(LDFILE)" -nostartfiles -L"./$(LDPATH)" -Wl,-Map,$(TARGET).map -mcpu=$(MCU) -mthumb -g3 -g$(DEBUG) $^ -o $@
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~"
	@echo ""


# Create final output files (.hex, .eep) from ELF output file.
$(TARGET).hex: $(TARGET).elf
	@echo ""
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~"
	@echo "Creating HEX file ... $@"
	$(OBJCOPY) -O $(FORMAT) $< $@
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~"
	@echo ""

# Create extended listing file from ELF output file.
$(TARGET).lst: $(TARGET).elf
	@echo ""
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~"
	@echo "Creating Listing ... $@"
	$(OBJDUMP) -h -S $< > $@
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~"
	@echo ""

# Create size information
$(TARGET).siz: $(TARGET).elf
	@echo ""
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~"
	@echo "Size output ... $<"
	$(SIZE) --format=berkely -x -t $<
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~"
	@echo ""

# Program the eStick2
debug:
	@echo ""
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
	@echo ""
	@echo " 1) Connect the eStick2 in bootloader mode.     "
	@echo " 2) WAIT until the eStick2 is mounted by the OS!"
	@echo "    >> be patient it may take several seconds <<"
	@echo " 3) Press any key to continue.                  "
	@echo ""
	@read -n 1 -s 
	@echo " ... flashing the debugger to the LPC1343       "
	@echo ""
	flash_lpc1343.sh Config/LPC1343_CMARMJTAG_V8.bin
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
	@echo " Press RESET on the eStick2 !                   "
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
	@echo ""
	@echo " a) In a separate terminal invoke openocd using "
	@echo "    openocd -f DbgCfg/LPC1758_OOCD_CMARM.cfg    "
	@echo ""
	@echo " b) Start the debugger, e.g.:                   "
	@echo "    arm-none-eabi-gdb firmware.elf              "
	@echo "    OR                                          "
	@echo "    arm-none-eabi-gdbtui firmware.elf           "
	@echo "    OR                                          "
	@echo "    a GUI debugger like nemiver, insight, etc.  "
	@echo ""
	@echo " c) In the debugger enter the following commands"
	@echo "    target remote localhost:3333                "
	@echo "    monitor halt                                "
	@echo "    load firmware.elf                           "
	@echo "    break main                                  "
	@echo "    continue                                    "
	@echo "    step                                        "
	@echo "    ...                                         "
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"

# Calculate only the CRC
crc:
	@echo ""
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~"
	@echo "Patching the CRC of the HEX file ..."
	crc.exe firmware.hex
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~"
	@echo ""

# print some usage message
help:
	@echo "UNIX                                            "
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
	@echo ""
	@echo "To build the project type:                      "
	@echo "make"
	@echo ""
 	@echo "To flash the debugger type:"
	@echo "make debug"
	@echo ""
	@echo "To clean the project type:"
	@echo "make clean"
	@echo ""
	@echo "WINDOWS                                         "
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
	@echo ""
	@echo "To build the project type:                      "
	@echo "cs-make"
	@echo ""
	@echo "To patch the HEX file type:"
	@echo "make crc"
	@echo ""
	@echo "To clean the project type:"
	@echo "make clean"
	@echo ""

.PHONY: all Makefile

ifdef SystemRoot
   RM = del /S /Q
else
   ifeq ($(shell uname), Linux)
      RM = shopt -s globstar; rm -rf 
   endif
endif


clean:
	$(RM) $(TARGET).hex $(TARGET).elf $(TARGET).bin
	$(RM) ./**/*~ ./**/*.d ./**/*.map ./**/*.o ./**/*.lst

