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

# Bootloader Source Files
BOOT_C_SRC  = bootloader.c
BOOT_S_SRC  = boot.s
BOOT_LDS    = boot.lds

# Startup Source Files
STARTUP_C_SRC = boot_logic_app.c
STARTUP_S_SRC = init.s
STARTUP_LDS   = init.lds

# Application boot code logic
BOOT_LOGIC_OBJ = boot_logic_app.o
DRV_OBJS = \
    $(INC_DIR)/uart.o \
    $(INC_DIR)/gpio.o \
    $(INC_DIR)/i2c.o \
    $(INC_DIR)/mpu6050.o \
    $(INC_DIR)/ssd1306.o \
    $(INC_DIR)/tm1637.o \
    $(INC_DIR)/my_string.o \
    $(INC_DIR)/timer.o \
    $(INC_DIR)/dht11.o \
    $(INC_DIR)/delay.o \
    $(INC_DIR)/soft_spi.o   \
    $(INC_DIR)/soft_w25q.o \
    $(INC_DIR)/spi.o     \
    $(INC_DIR)/w25q.o \
    $(INC_DIR)/adxl345.o \
    $(INC_DIR)/bmp180.o \
    $(INC_DIR)/mpu9265.o \
    $(INC_DIR)/soft_ads1115.o \
    $(INC_DIR)/mcp4725.o \
    $(INC_DIR)/seven_seg.o \
    $(INC_DIR)/printf.o \
    $(INC_DIR)/esp_wifi.o \
    $(INC_DIR)/thingspeak.o\
    $(INC_DIR)/mx25.o \
    $(INC_DIR)/s25fl.o \
    $(INC_DIR)/bme280.o

# -------------------------------------------------
# Compiler / Linker Flags
# ------------------------------------------------- 
CFLAGS = -mabi=ilp32 -march=rv32i -Og --std=c99 \
         -ffreestanding -nostdlib \
         -ffunction-sections -fdata-sections

# Standard LDFLAGS for the main application
LDFLAGS = \
    -Wl,--build-id=none \
    -Wl,-Bstatic \
    -Wl,--gc-sections \
    -Wl,-T,sections.lds \
    -Wl,-Map,$(RESULT_DIR)/firmware.map \
    -Wl,--strip-debug

# Specialized LDFLAGS for the bootloader
BOOT_LDFLAGS = \
    -Wl,--build-id=none \
    -Wl,-Bstatic \
    -Wl,--gc-sections \
    -Wl,-T,$(BOOT_LDS) \
    -Wl,-Map,$(RESULT_DIR)/bootloader.map \
    -Wl,--strip-debug

# Specialized LDFLAGS for the startup
STARTUP_LDFLAGS = \
    -Wl,--build-id=none \
    -Wl,-Bstatic \
    -Wl,--gc-sections \
    -Wl,-T,$(STARTUP_LDS) \
    -Wl,-Map,$(RESULT_DIR)/startup.map \
    -Wl,--strip-debug

# -------------------------------------------------
# Phony Targets
# -------------------------------------------------
.PHONY: all clean build_info bootloader startup upload

# -------------------------------------------------
# Main Targets
# -------------------------------------------------
all: build_info $(RESULT_DIR)/firmware.hex $(RESULT_DIR)/dumpfile

bootloader: $(RESULT_DIR)/boot.mem $(RESULT_DIR)/bootloader_dump

startup: $(RESULT_DIR)/startup.mem $(RESULT_DIR)/startup_dump

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
# Startup Specific Rules (init.s, boot_logic_app.c)
# -------------------------------------------------
$(RESULT_DIR)/startup.mem: $(RESULT_DIR)/startup.bin makehex.py
	python3 makehex.py $< > $@

$(RESULT_DIR)/startup.bin: $(RESULT_DIR)/startup.elf
	$(OBJ_COPY) -O binary $< $@

$(RESULT_DIR)/startup.elf: init_start.o init.o $(DRV_OBJS) $(STARTUP_LDS)
	$(GCC) $(CFLAGS) -o $@ init_start.o init.o $(DRV_OBJS) $(STARTUP_LDFLAGS) -lgcc

init_start.o: $(STARTUP_C_SRC)
	$(GCC) -c $(CFLAGS) -I$(INC_DIR) -o $@ $<

init.o: $(STARTUP_S_SRC)
	$(GCC) -c -mabi=ilp32 -march=rv32i -o $@ $<

$(RESULT_DIR)/startup_dump: $(RESULT_DIR)/startup.elf
	$(OBJ_DUMP) -d $< > $@

# -------------------------------------------------
# Bootloader Specific Rules
# -------------------------------------------------
$(RESULT_DIR)/boot.mem: $(RESULT_DIR)/bootloader.bin makehex.py
	python3 makehex.py $< > $@

$(RESULT_DIR)/bootloader.bin: $(RESULT_DIR)/bootloader.elf
	$(OBJ_COPY) -O binary $< $@

$(RESULT_DIR)/bootloader.elf: bootloader.o boot.o $(DRV_OBJS) $(BOOT_LDS)
	$(GCC) $(CFLAGS) -o $@ boot.o bootloader.o $(DRV_OBJS) $(BOOT_LDFLAGS) -lgcc

bootloader.o: $(BOOT_C_SRC)
	$(GCC) -c $(CFLAGS) -I$(INC_DIR) -o $@ $<

boot.o: $(BOOT_S_SRC)
	$(GCC) -c -mabi=ilp32 -march=rv32i -o $@ $<

$(RESULT_DIR)/bootloader_dump: $(RESULT_DIR)/bootloader.elf
	$(OBJ_DUMP) -d $< > $@

# -------------------------------------------------
# Application Linking
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
# General Compilation Rules
# -------------------------------------------------
$(C_SRC_DIR)/$(APP).o: $(C_SRC_DIR)/$(APP).c
	$(GCC) -c $(CFLAGS) -I$(INC_DIR) -o $@ $<

$(INC_DIR)/%.o: $(INC_DIR)/%.c
	$(GCC) -c $(CFLAGS) -I$(INC_DIR) -o $@ $<

$(ASM_SRC_DIR)/start.o: $(ASM_SRC_DIR)/start.S
	$(GCC) -c -mabi=ilp32 -march=rv32i -o $@ $<

# -------------------------------------------------
# Uploading firmware via UART
# -------------------------------------------------
PORT ?= COM14

upload: $(RESULT_DIR)/firmware.hex
	@echo "Uploading $< to $(PORT)..."
	python3 boot.py $(PORT) $(RESULT_DIR)/firmware.hex
# -------------------------------------------------
# Clean
# -------------------------------------------------
clean:
	@echo "Cleaning project..."
	del *.o $(INC_DIR)\*.o *.elf *.hex *.bin *.map *.mem dumpfile bootloader_dump startup_dump 2>null || \
	rm -f *.o $(INC_DIR)/*.o *.elf *.hex *.bin *.map *.mem dumpfile bootloader_dump startup_dump 