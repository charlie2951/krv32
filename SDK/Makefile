GCC = riscv64-unknown-elf-gcc
OBJ_DUMP = riscv64-unknown-elf-objdump
OBJ_COPY = riscv64-unknown-elf-objcopy

# Directories
C_SRC_DIR = .
INC_DIR = include
ASM_SRC_DIR = .
LDS_SRC_DIR = . 
RESULT_DIR = .
MEM_SIZE = 1024

# New: List of driver object files
DRV_OBJS = $(INC_DIR)/uart.o $(INC_DIR)/led_driver.o $(INC_DIR)/delay.o

all: $(RESULT_DIR)/firmware.hex $(RESULT_DIR)/dumpfile

$(RESULT_DIR)/firmware.hex: $(RESULT_DIR)/firmware.bin makehex.py
	python3 makehex.py $< > $@

$(RESULT_DIR)/firmware.bin: $(C_SRC_DIR)/firmware.elf
	$(OBJ_COPY) -O binary $< $@

$(RESULT_DIR)/dumpfile: $(C_SRC_DIR)/firmware.elf
	$(OBJ_DUMP) -d $^ > $@

# Linker Stage: Added $(DRV_OBJS) to the dependencies and the command
$(C_SRC_DIR)/firmware.elf: $(C_SRC_DIR)/main.o $(ASM_SRC_DIR)/start.o $(DRV_OBJS) sections.lds
	$(GCC) -Og -mabi=ilp32 -march=rv32i -ffreestanding -nostdlib -o $@ \
		-Wl,--build-id=none,-Bstatic,-T,sections.lds,-Map,$(RESULT_DIR)/firmware.map,--strip-debug \
		$(ASM_SRC_DIR)/start.o $(C_SRC_DIR)/main.o $(DRV_OBJS) -lgcc

# Compile main.c
$(C_SRC_DIR)/main.o: $(C_SRC_DIR)/main.c
	$(GCC) -c -I$(INC_DIR) -mabi=ilp32 -march=rv32i -Og --std=c99 -ffreestanding -nostdlib -o $@ $<

# New: Pattern rule to compile any .c file inside the include directory
$(INC_DIR)/%.o: $(INC_DIR)/%.c
	$(GCC) -c -I$(INC_DIR) -mabi=ilp32 -march=rv32i -Og --std=c99 -ffreestanding -nostdlib -o $@ $<

# Compile start.S
$(ASM_SRC_DIR)/start.o: $(ASM_SRC_DIR)/start.S
	$(GCC) -c -mabi=ilp32 -march=rv32i -o $@ $<

clean:
	del *.o $(INC_DIR)\*.o dumpfile *.elf *.hex *.bin *.map