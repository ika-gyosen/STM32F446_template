# Minimal makefile for STM32F3 Neucleo board.
TARGET := arm-none-eabi

# devtools
CC := $(TARGET)-gcc
LD := $(TARGET)-ld
AS := $(TARGET)-as
OBJCOPY := $(TARGET)-objcopy
STFLASH := st-flash
STUTIL := st-util
PORT := /dev/ttyACM0

CFLAGS = -g3 -mcpu=cortex-m4 -mthumb -nostdlib -O0 -ffunction-sections -fdata-sections -Wl,--gc-sections -std=c99
INCLUDE = -I./include

OBJDIR = obj

SOURCES = $(shell find * -name "*.c")
SRCDIR = $(dir $(SOURCES))
BINDIR = $(addprefix $(OBJDIR)/, $(SRCDIR))

ASMS = $(shell find * -name "*.s")
ASMDIR = $(dir $(ASMS))
ASMBIN = $(addprefix $(OBJDIR)/, $(ASMDIR))

OBJECTS = $(addprefix $(OBJDIR)/, $(patsubst %.c, %.o, $(SOURCES))) $(addprefix $(OBJDIR)/, $(patsubst %.s, %.o, $(ASMS))) 
DEPENDS = $(OBJECTS:.o=.d)

ifeq "$(strip $(OBJDIR))" ""
	OBJDIR = .
endif

MEMORYMAP = STM32F446XE.ld
OBJECTNAME = STM

default :
	@[ -d $(OBJDIR) ] || mkdir -p $(OBJDIR)
	@[ -d "$(BINDIR)" ] || mkdir -p $(BINDIR)
	@[ -d "$(ASMBIN)" ] || mkdir -p $(ASMBIN)
	@make all --no-print-directory

all: $(OBJECTS) $(OBJECTNAME).bin $(OBJECTNAME).elf

$(OBJECTNAME).elf: $(OBJECTNAME).bin
	$(OBJCOPY) -I binary -O elf32-little $(OBJECTNAME).bin $(OBJECTNAME).elf

$(OBJECTNAME).bin: $(OBJECTNAME).out
	$(OBJCOPY) $(OBJECTNAME).out -I ihex -O binary $(OBJECTNAME).bin

$(OBJECTNAME).out: $(OBJECTS)
	$(LD) -T $(MEMORYMAP) $(OBJECTS) -o $(OBJECTNAME).out

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(OBJDIR)/%.o: %.s
	$(AS) -g $< -o $@

.PHONY: clean flash

st-flash: $(OBJECTNAME).bin
	$(STFLASH) write $(OBJECTNAME).bin 0x8000000

st-util: $(OBJECTNAME).bin
	$(STUTIL) $(OBJECTNAME).bin

clean:
	rm -rf $(OBJDIR) *.out *.bin *.elf