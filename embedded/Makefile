################################################################################
# Makefile for the LPC1758 / eStick2 by Martin Horauer
#
SOURCEPATH = src
BINPATH = bin
SCRIPTPATH = buildscripts

################################################################################
# Sytem files
SRC = $(SOURCEPATH)/app.c
SRC += $(SOURCEPATH)/taskMachine.c
SRC += $(SOURCEPATH)/taskSupervisor.c
SRC += $(SOURCEPATH)/taskStart.c
SRC += $(SOURCEPATH)/taskBlink.c
SRC += $(SOURCEPATH)/taskButton.c

################################################################################
#don't edit below
SYSPATH = $(SOURCEPATH)/Libraries/SYS/src
LIBRARYPATH = $(SOURCEPATH)/Libraries/Library/src
DRVPATH = $(SOURCEPATH)/Libraries/Driver/LPC17xx/src

BUILDPATH = build/

INCPATH  = -I"$(SOURCEPATH)/Libraries/BSP"
INCPATH += -I"$(SOURCEPATH)/Libraries/BSP/LCD"
INCPATH += -I"$(SOURCEPATH)/Libraries/CFG"
INCPATH += -I"$(SOURCEPATH)/Libraries/CM3/inc"
INCPATH += -I"$(SOURCEPATH)/Libraries/serial_lite"
INCPATH += -I"$(SOURCEPATH)/Libraries/uC-CPU"
INCPATH += -I"$(SOURCEPATH)/Libraries/uC-CSP"
INCPATH += -I"$(SOURCEPATH)/Libraries/uC-CSP/LPC17xx"
INCPATH += -I"$(SOURCEPATH)/Libraries/uC-LIB"
INCPATH += -I"$(SOURCEPATH)/Libraries/uC-OSIII"
INCPATH += -I"$(SOURCEPATH)/Libraries/uC-Serial"
INCPATH += -I"$(SOURCEPATH)/Libraries/uC-Probe"
INCPATH += -I"$(SOURCEPATH)/Libraries/Driver/LPC17xx/inc"
INCPATH += -I"$(SOURCEPATH)/Libraries/Library/inc"
INCPATH += -I"$(SOURCEPATH)/Libraries/SYS/inc"
INCPATH += -I"$(SOURCEPATH)"

ASRC += $(SOURCEPATH)/Libraries/CM3/src/startup_LPC17xx.asm
ASRC += $(SOURCEPATH)/Libraries/uC-CPU/cpu_a.asm
ASRC += $(SOURCEPATH)/Libraries/uC-CPU/os_cpu_a.asm

SSRC += $(SOURCEPATH)/Libraries/uC-CPU/os_cpu_c.c
SSRC += $(SOURCEPATH)/Libraries/uC-CPU/cpu_c.c
SSRC += $(SOURCEPATH)/Libraries/uC-CPU/cpu_core.c
SSRC += $(SOURCEPATH)/Libraries/BSP/LCD/Images/img_tbl.c
SSRC += $(SOURCEPATH)/Libraries/BSP/LCD/Images/img_nxp_logo.c
SSRC += $(SOURCEPATH)/Libraries/BSP/LCD/Images/img_os_logo.c
SSRC += $(SOURCEPATH)/Libraries/BSP/LCD/Images/img_micirum_logo.c
SSRC += $(SOURCEPATH)/Libraries/BSP/LCD/Images/img_keil_logo.c
SSRC += $(SOURCEPATH)/Libraries/BSP/LCD/Fonts/font_terminal_8x8.c
SSRC += $(SOURCEPATH)/Libraries/BSP/LCD/Fonts/font_tbl.c
SSRC += $(SOURCEPATH)/Libraries/BSP/LCD/Fonts/font_comic_18x18.c
SSRC += $(SOURCEPATH)/Libraries/BSP/LCD/Fonts/font_swiss_16x16.c
SSRC += $(SOURCEPATH)/Libraries/BSP/LCD/bsp_lcd.c
SSRC += $(SOURCEPATH)/Libraries/BSP/cpu_bsp.c
SSRC += $(SOURCEPATH)/Libraries/BSP/bsp.c
SSRC += $(SOURCEPATH)/Libraries/CM3/src/hw_timer.c
SSRC += $(SOURCEPATH)/Libraries/CM3/src/core_cm3.c
#SSRC += $(SOURCEPATH)/Libraries/CM3/src/lpc17xx_nvic.c
SSRC += $(SOURCEPATH)/Libraries/CM3/src/system_LPC17xx.c
SSRC += $(SOURCEPATH)/Libraries/CM3/src/libnosys_gnu.c
SSRC += $(SOURCEPATH)/Libraries/CM3/src/NVICInit.c
SSRC += $(SOURCEPATH)/Libraries/uC-LIB/lib_math.c
SSRC += $(SOURCEPATH)/Libraries/uC-LIB/lib_ascii.c
SSRC += $(SOURCEPATH)/Libraries/uC-LIB/lib_str.c
SSRC += $(SOURCEPATH)/Libraries/uC-LIB/lib_mem.c
SSRC += $(SOURCEPATH)/Libraries/uC-OSIII/os_prio.c
SSRC += $(SOURCEPATH)/Libraries/uC-OSIII/os_tmr.c
SSRC += $(SOURCEPATH)/Libraries/uC-OSIII/os_var.c
SSRC += $(SOURCEPATH)/Libraries/uC-OSIII/os_pend_multi.c
SSRC += $(SOURCEPATH)/Libraries/uC-OSIII/os_task.c
SSRC += $(SOURCEPATH)/Libraries/uC-OSIII/os_time.c
SSRC += $(SOURCEPATH)/Libraries/uC-OSIII/os_sem.c
SSRC += $(SOURCEPATH)/Libraries/uC-OSIII/os_flag.c
SSRC += $(SOURCEPATH)/Libraries/uC-OSIII/os_stat.c
SSRC += $(SOURCEPATH)/Libraries/uC-OSIII/os_cfg_app.c
SSRC += $(SOURCEPATH)/Libraries/uC-OSIII/os_mutex.c
SSRC += $(SOURCEPATH)/Libraries/uC-OSIII/os_q.c
SSRC += $(SOURCEPATH)/Libraries/uC-OSIII/os_dbg.c
SSRC += $(SOURCEPATH)/Libraries/uC-OSIII/os_core.c
SSRC += $(SOURCEPATH)/Libraries/uC-OSIII/os_tick.c
SSRC += $(SOURCEPATH)/Libraries/uC-OSIII/os_msg.c
SSRC += $(SOURCEPATH)/Libraries/uC-OSIII/os_mem.c
SSRC += $(SOURCEPATH)/Libraries/uC-OSIII/os_int.c
SSRC += $(SOURCEPATH)/Libraries/CFG/os_app_hooks.c
SSRC += $(SOURCEPATH)/Libraries/serial_lite/os_serial_lite.c
SSRC += $(SOURCEPATH)/Libraries/uC-CSP/os_csp.c
SSRC += $(SOURCEPATH)/Libraries/uC-CSP/csp.c
SSRC += $(SOURCEPATH)/Libraries/uC-CSP/LPC17xx/lpc17xx_clkpwr.c
SSRC += $(SOURCEPATH)/Libraries/uC-CSP/LPC17xx/csp_gpio.c
SSRC += $(SOURCEPATH)/Libraries/uC-CSP/LPC17xx/csp_tmr.c
SSRC += $(SOURCEPATH)/Libraries/uC-CSP/LPC17xx/lpc17xx_pinsel.c
SSRC += $(SOURCEPATH)/Libraries/uC-CSP/LPC17xx/lpc17xx_gpio.c
#SSRC += $(SOURCEPATH)/Libraries/uC-CSP/LPC17xx/lpc17xx_emac.c
SSRC += $(SOURCEPATH)/Libraries/uC-CSP/LPC17xx/lpc17xx_uart.c
#SSRC += $(SOURCEPATH)/Libraries/uC-CSP/LPC17xx/lpc17xx_ssp.c
SSRC += $(SOURCEPATH)/Libraries/uC-CSP/LPC17xx/csp_int.c
SSRC += $(SOURCEPATH)/Libraries/uC-CSP/LPC17xx/csp_pm.c
SSRC += $(SOURCEPATH)/Libraries/uC-CSP/LPC17xx/csp_dma.c

SSRC += $(DRVPATH)/lpc17xx_nvic.c
SSRC += $(DRVPATH)/lpc17xx_dac.c
SSRC += $(DRVPATH)/zentoolworksDriver.c

#SSRC += $(LIBRARYPATH)/capcom.c
SSRC += $(LIBRARYPATH)/timer.c
SSRC += $(LIBRARYPATH)/timeout.c
SSRC += $(LIBRARYPATH)/led.c
SSRC += $(LIBRARYPATH)/uart.c
SSRC += $(LIBRARYPATH)/circularbuffer.c
SSRC += $(LIBRARYPATH)/adc.c
SSRC += $(LIBRARYPATH)/dac.c
SSRC += $(LIBRARYPATH)/pwm.c
SSRC += $(LIBRARYPATH)/gpio.c
SSRC += $(LIBRARYPATH)/pincon.c
SSRC += $(LIBRARYPATH)/iap.c
SSRC += $(LIBRARYPATH)/wifly.c
SSRC += $(LIBRARYPATH)/button.c
SSRC += $(LIBRARYPATH)/crc.c
SSRC += $(LIBRARYPATH)/ssp.c
SSRC += $(LIBRARYPATH)/rfm12.c

LDFILE = LPC1758.ld
LDPATH = Linker

################################################################################
# definitions
TARGET = bin/firmware
MCU = cortex-m3
FORMAT = binary

CROSS_COMPILE = arm-none-eabi-
#CROSS_COMPILE = arm-eabi-
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
CFLAGS += -Wa,-adhlns=$(<:%.c=$(BUILDPATH)%.o.lst)
CFLAGS += -g$(DEBUG)
#CFLAGS += -fno-rtti
CFLAGS += -fno-exceptions

ALL_CFLAGS = $(INCPATH) $(CFLAGS) $(OPTFLAGS) -mcpu=$(MCU) -mthumb -g3 
ALL_CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)"

ALL_AFLAGS = -x assembler-with-cpp -Wall -Wa,-adhlns="$(<:%.asm=%.o.lst)"
ALL_AFLAGS += -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" 
ALL_AFLAGS += -mcpu=$(MCU) -mthumb -g3 -g$(DEBUG)

#ifdef SystemRoot
#   RM = del /Q
#   MKDIR = mkdir
#else
#   ifeq ($(shell uname), Linux)
      RM = rm -f
      MKDIR = mkdir -p
#   endif
#endif

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
$(BUILDPATH)%.o : %.c
	@echo ""
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
	@echo "Building file: $(@:%.o=%.c)"
	$(MKDIR) `dirname $@`
	#TMPNAME=`echo ${$(@:%.o=%.c)#*/}`
	$(CC) -c $(ALL_CFLAGS) -o $@ `echo "$(@:%.o=%.c)" | sed 's,^[^/]*/,,'`
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
	@echo ""

# Compile: create object files from ASM source files.
.IGNORE: %.c
$(BUILDPATH)%.o : %.asm
	@echo ""
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
	@echo "Building file: $(@:%.o=%.asm)"
	$(MKDIR) `dirname $@`
	$(CC) $(ALL_AFLAGS) -o $@ `echo "$(@:%.o=%.asm)" | sed 's,^[^/]*/,,'`
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
	@echo ""


# Link: create ELF output file from object files.
.SECONDARY : $(TARGET).elf
.PRECIOUS : $(OBJ)
$(TARGET).elf : $(SRC:%.c=$(BUILDPATH)%.o) $(SSRC:%.c=$(BUILDPATH)%.o) $(ASRC:%.asm=$(BUILDPATH)%.o)
	@echo ""
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~"
	@echo "Linking file ... $<"
	$(LD) -T"./$(LDPATH)/$(LDFILE)" -nostartfiles -L"./$(LDPATH)" $(LD_FLAGS) -Wl,-Map,$(TARGET).map -mcpu=$(MCU) -mthumb -g3 -g$(DEBUG) $^ -o $@
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

# Calculate only the CRC
crc:
	@echo ""
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~"
	@echo "Patching the CRC of the HEX file ..."
	rm -f $(TARGET).bin
	cp $(TARGET).hex $(TARGET).bin
	crc $(TARGET).bin
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~"
	@echo ""

run:
	@echo ""
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~"
	@echo "Flashing file to device ..."
	$(SCRIPTPATH)/run.sh
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~"
	@echo ""

# Program the eStick2
debug:
	@echo ""
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
	@echo ""
	@echo " Starting debug.                                "
	$(SCRIPTPATH)/run_debug.sh
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"

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

clean:
	$(RM) $(TARGET).map $(TARGET).lst
	$(RM) -R $(BUILDPATH)*

