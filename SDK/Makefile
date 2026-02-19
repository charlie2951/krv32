# -------------------------------------------------
# Toolchain
# -------------------------------------------------
GCC       = riscv64-unknown-elf-gcc
OBJ_DUMP  = riscv64-unknown-elf-objdump
OBJ_COPY  = riscv64-unknown-elf-objcopy

# -------------------------------------------------
# Configuration
# -------------------------------------------------
APP ?= main

C_SRC_DIR   = .
INC_DIR     = include
ASM_SRC_DIR = .
RESULT_DIR  = .

DRV_OBJS = \
	$(INC_DIR)/uart.o \
	$(INC_DIR)/gpio.o \
	$(INC_DIR)/softi2c.o \
	$(INC_DIR)/mpu6050.o \
	$(INC_DIR)/ssd1306.o \
	$(INC_DIR)/tm1637.o \
	$(INC_DIR)/my_string.o \
	$(INC_DIR)/timer.o \
	$(INC_DIR)/dht11.o \
	$(INC_DIR)/delay.o

# -------------------------------------------------
# Compiler / Linker Flags
# -------------------------------------------------
CFLAGS = -mabi=ilp32 -march=rv32i -Og --std=c99 \
         -ffreestanding -nostdlib \
         -ffunction-sections -fdata-sections

LDFLAGS = \
	-Wl,--build-id=none \
	-Wl,-Bstatic \
	-Wl,--gc-sections \
	-Wl,-T,sections.lds \
	-Wl,-Map,$(RESULT_DIR)/firmware.map \
	-Wl,--strip-debug

# -------------------------------------------------
# Phony Targets
# -------------------------------------------------
.PHONY: all clean build_info

# -------------------------------------------------
# Main Targets
# -------------------------------------------------
all: build_info $(RESULT_DIR)/firmware.hex $(RESULT_DIR)/dumpfile

build_info:
	@echo "--------------------------------------"
	@echo "Building Application: $(APP).c"
	@echo "--------------------------------------"

# -------------------------------------------------
# Output Conversions
# -------------------------------------------------
$(RESULT_DIR)/firmware.hex: $(RESULT_DIR)/firmware.bin makehex.py
	python3 makehex.py $< > $@

$(RESULT_DIR)/firmware.bin: $(RESULT_DIR)/firmware.elf
	$(OBJ_COPY) -O binary $< $@

$(RESULT_DIR)/dumpfile: $(RESULT_DIR)/firmware.elf
	$(OBJ_DUMP) -d $< > $@

# -------------------------------------------------
# Linking
# -------------------------------------------------
$(RESULT_DIR)/firmware.elf: \
	$(C_SRC_DIR)/$(APP).o \
	$(ASM_SRC_DIR)/start.o \
	$(DRV_OBJS) \
	sections.lds
	$(GCC) $(CFLAGS) -o $@ \
		$(ASM_SRC_DIR)/start.o \
		$(C_SRC_DIR)/$(APP).o \
		$(DRV_OBJS) \
		$(LDFLAGS) \
		-lgcc

# -------------------------------------------------
# Compilation Rules
# -------------------------------------------------
$(C_SRC_DIR)/$(APP).o: $(C_SRC_DIR)/$(APP).c
	$(GCC) -c $(CFLAGS) -I$(INC_DIR) -o $@ $<

$(INC_DIR)/%.o: $(INC_DIR)/%.c
	$(GCC) -c $(CFLAGS) -I$(INC_DIR) -o $@ $<

$(ASM_SRC_DIR)/start.o: $(ASM_SRC_DIR)/start.S
	$(GCC) -c -mabi=ilp32 -march=rv32i -o $@ $<

# -------------------------------------------------
# Clean
# -------------------------------------------------
clean:
	@echo "Cleaning project..."
	del *.o $(INC_DIR)\*.o *.elf *.hex *.bin *.map dumpfile 2>nul || \
	rm -f *.o $(INC_DIR)/*.o *.elf *.hex *.bin *.map dumpfile
