# A subset of RISC-V architecture (RV32I) SoC implementation for FPGA 
A tiny version of `RISC-V` for low-resource FPGA developed by the **Integrated System Design lab, School of Electronics, KIIT University** <p>
**Objective**
The main objective of this project is to prototype a RISC-V 32-bit CPU with an **RV32 base integer (RV32I) instruction set**. The CPU is coded using Verilog HDL from scratch and most of the instruction sets are implemented. The code is compatible with the GNU RISC-V toolchain (both assembler and compiler).
## Features of KRV-32 soft IP SOC
1. Supports RISC-V RV-32I extension
2. Compatible with GNU toolchain
3. Soft-IP, configurable as per your FPGA
4. On-board LEDS can be interfaced using C SDK
5. Fully functional UART Transmitter and Receiver (currently support 9600 baud rate only)
6. BRAM based BOOTLOADER support (need not to generate bitstream everytime while changing application code)
7. Crypto core based on 32 bit AES, both encryption and decryption
8. Four-channel PWM output with a frequency of 500HZ and duty cycle 0-100% (testing going on, not included at this stage)
9. Tested in Artix-7 series FPGA embedded in Nexys-4 DDR board
   <p></p>
   
## RV32I CPU – Design Documentation (Version 3.0)
**Architecture:** Multi-cycle controlled by Finite-State-Machine (FSM) 
**ISA Support:** RV32I (Base Integer Instructions)
Last Revision: 24-April-2025
Author: — Subir Kr. Maity
Target: FPGA-friendly, resource-minimal core
<p> </p>

<img width="800" height="600" alt="block" src="https://github.com/user-attachments/assets/89cfa439-5de9-4473-b9c8-fa9fe0eb5fee" />

**1. Overview**

This document describes the function and micro-architecture of a simple RV32I processor implemented using a finite-state machine (FSM). The design is optimized for small FPGAs and uses a single-port synchronous memory with one-cycle latency.

The CPU executes all RV32I instructions:

ALU operations (R-type, I-type)

Load/store

Branch

Jumps (JAL, JALR)

Upper immediate instructions (LUI, AUIPC)

**Major design characteristics**
_Feature	Description_
|Architecture|	FSM-based sequential core|
|------------|----------------------------|
|ISA|	`RV32I` (no multiplication/division)|
|Pipeline|	None (multi-cycle FSM states)|
|Memory|	32-bit synchronous single-cycle|
|Register File	|32 × 32-bit, hardwired x0=0|
|ALU|	supports `ADD/SUB`, logic, shifts, `SLT/SLTU` `Load/Store`	`Byte`, `halfword`, `word` (signed/unsigned) Endianness	Little-endian|

### Known bugs fixed

`LB, LH, SB, SH`: Fixed on 24-Apr-2025<p>
`LBU, LHU`: Implemented but not fully verified<p>
`SRA/SRAI`: Still listed as known issue<p>
Store address alignment: corrected
<p></p>

**2. Top-Level CPU Interface**

```verilog
module cpu(
    input         rst,
    input         clk,
    input [31:0]  mem_rdata,
    output [31:0] mem_addr,
    output [31:0] mem_wdata,
    output        mem_rstrb,
    output reg [31:0] cycle,
    output [3:0]  mem_wstrb
);
```

## Signal Descriptions
|Signal|	Dir|	Width|	Description|
|-------|-------|--------|-------------|
|`clk`	|	in	|1	|System clock|
|`rst`	|	in	|1	|Synchronous reset|
|`mem_rdata`|	in	|32	|Data from memory|
|`mem_addr`|	out	|32|	Address for load/store or instruction fetch|
|`mem_wdata`|	out	|32	|Write data for store instructions|
|`mem_rstrb`|	out	|1	|Memory read request|
|`mem_wstrb`|	out|	4|	Byte-wide write strobes|
|`cycle`|	out|	32|	Cycle counter(for debugging purpose)|

**3. Microarchitecture**<p>

The CPU uses a finite-state machine with 8 states:

|State		|	Code	|Description|
|------------|----------|-----------|
|`RESET`	|		0|	Initialize PC and state|
|`WAIT`		|	1	|Extra cycle for synchronous memory fetch|
|`FETCH`	|		2	|Instruction fetch (mem_rdata is valid)|
|`DECODE`	|	3	|Decode instruction, read register file|
|`EXECUTE`	|	4	|Perform ALU, branch decision, compute next PC|
|`BYTE`	|		5|	Load/store address preparation|
|`WAIT_LOADING`|	6|	Wait for memory access completion|
|`HLT`	|		7	|Halt on system instruction|

**4. Instruction Decoding**
Opcode extraction<p>
`wire [4:0] opcode = data[6:2];`<p>

Instruction classification <p>
|Type|	Opcode|	Examples|
|----|---------|---------|
|R-type|	01100|	ADD, SUB, AND, XOR, SLL|
|I-type|	00100|	ADDI, ANDI, ORI, SLTI|
|Load	|00000	|LB, LH, LW, LBU, LHU|
|Store|	01000|	SB, SH, SW|
|Branch|	11000|	BEQ, BNE, BLT|
|JAL|	11011|	Unconditional jump|
|JALR|	11001|	Register jump|
|LUI|	01101|	Load Upper Immediate|
|AUIPC|	00101|	PC-relative immediate|

Immediate formats `(I, S, B, J, U)` are decoded exactly per RISC-V spec.

**5. ALU Design**
Supported operations:
|Operation|	Notes|
|---------|-------|
|ADD, SUB	SUB| uses two's complement addition
|AND, OR, XOR|	Standard logic ops|
|SLL, SRL|	Shift by register or immediate|
|SRA|	Signed right shift (buggy in this version)|
|SLT, SLTU|	Uses sign-bit and carry to determine relation|

### ALU input selection

`alu_in1 = rs1`<p>
`alu_in2 = depends on instruction type (rs2 or immediate)`<p>

**6. Branch and Jump Logic**

Branch decision uses ALU subtraction and comparison signals:<p>

`wire TAKE_BRANCH = ...`<p>


`pcplus4` and `pcplusimm` are computed and used as next PC depending on instruction type.

**7. Memory System**
Load/Store addressing<p>
Address from ALU result:<p>
`load_store_addr = alu_result;`<p>
Byte & halfword extraction<p>
Handles:<p>
LB/LBU<p>
LH/LHU<p>
LW<p>
Using bit index on `load_store_addr`.<p>
Write mask generation<p>
Determines which bytes of the 32-bit bus to write during store instructions.<p>

**8. Register File**

32 registers `(x0–x31)`<p>
Writes occur in `EXECUTE` or `WAIT_LOADING` state <p>
x0 is permanently zero <p>
Write-back data selection:<p>
ALU result<p>
Load data<p>
`PC+4` for jumps<p>
U-type immediates<p>
<p>
	
**9. FSM Operation**
Execution Timeline per Instruction

`WAIT`<p>
Initiates new instruction fetch (memory latency accommodation)<p>

`FETCH`<p>
Gets instruction from memory<p>

`DECODE`<p>
Reads register file and decodes instruction fields<p>

`EXECUTE`<p>
ALU computes result<p>
Branch decision made<p>
Next PC calculated<p>

`BYTE / WAIT_LOADING` <p>
Performs load/store address delay <p>
Generates strobes <p>
Waits for memory read data<p>
Write-back<p>
Register file updated<p>

**10. Bugs / Limitations**
<p></p>
Known Functional Issues<p></p>
LBU, LHU	Implemented but not validated<p></p>
Misaligned access. Not fully supported.<p></p>
No `pipeline`	, Multi-cycle per instruction

## RISC-V Architecture and Instruction Set
Refer to the RiSC-V official page and/or other tutorials. Some useful links are given below.
1. https://www2.eecs.berkeley.edu/Pubs/TechRpts/2016/EECS-2016-118.pdf
2. https://www.cs.sfu.ca/~ashriram/Courses/CS295/assets/notebooks/RISCV/RISCV_CARD.pdf
3. https://lists.riscv.org/g/tech-unprivileged/attachment/535/0/unpriv-isa-asciidoc.pdf
4. https://shakti.org.in/docs/risc-v-asm-manual.pdf
5. https://pages.hmc.edu/harris/ddca/ddcarv/DDCArv_AppB_Harris.pdf

**Debugging and Simulation** <p>
For debugging and behavioral simulation, use any Verilog compiler. I have used open-source Icarus Verilog with GTKWave waveform viewer. A sample testbench is added for debugging and test purposes. Modify the testbench as per your requirements.<p>
*Program Memory space*: Default is 1Kbyte. Each location will contain 32-bit data. However, you can change it in code (progmem.v). Verilog Implementation: *reg[31:0] PROGMEM[0:1023]* <p>

 **FPGA Implementation**
Synthesized and implemented in `Artix-7 series FPGA in Nexys4-DDR board` with a clock speed of `100 MHz`. <p>

## GPIO mapping along with FPGA pin details

| Peripheral | Nexys4- DDR FPGA used Pins      |
|------------|:------------------------------------:|
| LED        |  U16, U17, V17, R18, N14, J13, K15, H17  |
| UART_TX    |  D4                  |
| UART_RX    |  C4                  |
| BOOT_Enable| J15 (High=BOOT mode, Low=Execution mode)|
|AES Crypto IP| Internally accessable via memory mapped registers|


<p>
For Nexys-4 DDR board which uses Artix-7 series FPGA, LEDS are active high and total 8 LEDs are connected. Refer to the table mentioned above. <p>
	
**Flowchart** <p>
The CPU is implemented in a straightforward way by keeping the code simple and understandable. Further optimization may be done to save hardware resources and speed. In this implementation, a multi-clock cycle is required to execute a single instruction. Currently, the R-type (ADD, SUB, AND, OR etc) and Immediate instructions (I-type) instructions (ADDI, ANDI, ORI, etc) consume 5 clock cycles, Load and store type instructions take 9 cycles, Branch and Jump instructions take 3 cycles, and others (LUI, AUIPC) take 4 cycles. Pipelining may be implemented in the future to speed up the execution. <p>
**Execution Flow in the State machine** <p>
The initial state is *RESET* state. After that the control goes to *FETCH* state.<p>
*FETCH:* The 32-bit instruction code is loaded from the program memory into 32 bit instruction register named data <p>
```verilog
FETCH: //Fetch data from progmem RAM
      begin
        data <= mem_rdata; //latch mem read data into reg
        state <= DECODE;
      end

```
*DECODE:* The decoding of instruction takes place in this state. A separate mode detector module is attached to decide the addressing mode.
```verilog
 wire [4:0] opcode = data[6:2];
  wire [4:0] rd = data[11:7];
  wire [2:0] funct3 = data[14:12];
  wire [6:0] funct7 = data[31:25];
  wire [31:0] I_data = {{21{data[31]}},data[30:20]}; //sign extended data
  wire [31:0] B_data = {{20{data[31]}},data[7],data[30:25],data[11:8],1'b0}; //sign extended branch data
  wire [31:0] S_data = {{21{data[31]}},data[30:25],data[11:7]};//sign extended imm data for S-type
  wire [31:0] J_data = {{12{data[31]}},data[19:12],data[20],data[30:21],1'b0};//sign extended jump data
  wire [31:0] U_data = {data[31],data[30:12],12'h000};//LUI, AUIPC , 12 bit shifted imm data
  // check whether opcode is for R type or I type  or B-type
  wire isRtype = (opcode == 5'b01100);
  wire isItype = (opcode == 5'b00100);
  wire isBtype = (opcode == 5'b11000);
  wire isSystype = (opcode == 5'b11100);
  wire isStype = (opcode == 5'b01000);
  wire isLtype = (opcode == 5'b00000);
  wire isJAL   = (opcode == 5'b11011);
  wire isJALR  = (opcode == 5'b11001);
  wire isLUI = (opcode == 5'b01101);
  wire isAUIPC = (opcode == 5'b00101);
```
## Steps for Generating HEX code from assembly language
Use any RISC-V assembler to convert your assembly code into Hex dump. One of such online assembler can be found here https://riscvasm.lucasteske.dev/  . Copy the code hex dump, and paste it inside `firmware.hex` file in Verilog directory. Then run simulation. You can use any other assembler such as RISC-V toolchain etc.
## Steps for Generating Hex code from RISC-V C code
1. Install prerequisites for Ubuntu
```
$ sudo apt-get install autoconf automake autotools-dev curl python3 python3-pip python3-tomli libmpc-dev libmpfr-dev libgmp-dev gawk build-essential bison flex texinfo gperf libtool patchutils bc zlib1g-dev libexpat-dev ninja-build git cmake libglib2.0-dev libslirp-dev
```
2. Install RISC-V gcc toolchain. You need a Linux PC or Windows, Windows Subsystem for Linux with Ubuntu support. Follow the steps to install the toolchain <p>
Clone the official RISC-V GNU toolchain and compile for 32-bit arch with ILP32 option (RV32I) without the multiplication support that we have implemented into FPGA.
```
$ git clone https://github.com/riscv/riscv-gnu-toolchain
./configure --prefix=/opt/riscv --with-arch=rv32i --with-abi=ilp32
make linux
```
Make sure that `/opt/riscv` path has rw access. Otherwise, change it to your preferred path. <p>
To compile a toolchain for multilib support (both 32-bit and 64-bit), use the following <p>
```
./configure --prefix=/opt/riscv --enable-multilib
make linux
```
The multilib compiler will have the prefix `riscv64-unknown-elf-` or `riscv64-unknown-linux-gnu-` but will be able to target both 32-bit and 64-bit systems at a cost of installation size and build time. It will support the most common `-march/-mabi` options, which can be seen by using the `--print-multi-lib` flag on either cross-compiler.<p>

## Test a Sample counter program (see SDK for updated GPIO access API using C program) 
Open a test editor and add the following piece of code. Save the code as *main.c*<p>
Blinking on-board LEDs
```c
// C code to blink LED with gpio
//Customize the code as per your requirement

#include <krv32.h>


int main()
{
   volatile uint32_t count = 0;
	      
    while (1)
    {
	
        digitalwrite(count); //send data to LED gpio
        delay(1000000);//user defined delay approx 1sec
        count = count + 1; //update count by 1
    }
}
```

## Compiling the source code
A dedicated makefile is provided (inside testcase located at (testcase/RISCV_GCC_testcases/counter/Makefile) to automate the task. Execute the following commands serially to generate the hex code with an 8-bit chunk. Note: if you are using 32-bit toolchain without multilib support then replace the *riscv64*  keyword by using *riscv32* and also check the GCC toolchain path mentioned in the Makefile. In my case, it is *RISCV_TOOLCHAIN_DIR = /home/kiit/riscv/bin*. Change it as per your installation.

```shell
make clean
make
```
Copy the content of the generated firmware.txt file (you may exclude zeros) into your FPGA's firmware.mem and run the FPGA design flow or simulate it for debugging. You may open the *dumpfile* in text editor to see the generated assembly code from C code. This is useful for debugging. <p>

***Points to Remember*** <p>
1. `Program memory size:` The default size is 2KB. See the generated `firmware.hex` file. If it crosses 512 lines (= 512 x 4), excluding the last rows of ZEROS, the code will not fit into 2KB memory space. Additionally you need to allocate some space for stack pointer which is generally pointing to last memory location as per start-up script in assembler. In that case, You have to increase memory size. For that, open Verilog file `progmem.v` and change the line `parameter MEM_SIZE=1024` to the required value. Also, edit the *Makefile* and change the variable *MEM_SIZE = 1024* to the required value. Then open the loader script *sections.lds* and change the LENGTH variable `mem : ORIGIN = 0x00000000, LENGTH = 1K` to the required value. Also open `start.s` file and change the MEM_SIZE line `.equ MEM_SIZE, 0x400 ' to `0x800` i.e 2kb <p>
2. `Whether Verilog core support toolchain generated opcode?` :  See the `dumpfile` after C compilation and check the registers and instructions used. This version of CPU supports almost all commonly used instructions unless mentioned at the top of cpu.v <p>
3. Traps or any other interrupts are not supported. CSR instructions are not supported in the current version. <p>
***N.B.*** This is the working version of soft SoC and may contain additional bugs. <p>

## FPGA Implementation Examples
Go through the ***fpga*** directory to explore the compiled and implemented project in the Tang9K FPGA from GOWIN. Note that Tang9k uses an active low reset. Also LED port is 6 bits. So there are minor changes in reset logic (!rst instead of rst) and top module (top.v).

## Revision Note
SoC Version-4 is more hardware efficient in terms of FPGA resources and more enriched with added peripherals. Previous versions (V1.0) consume 34% LUT, V2 takes only 14%. Program memory is isolated from the CPU core to bring more flexibility. GPIO logic is also isolated and implemented at top.v file (top module).

***Known issues/bugs*** <p>
Date: 16/04/2025: Bug: Arithmetic right shift SRA, SRAI not working<p>
Date: 5/6/2025: Bug fixed `SRA` and `SRAI` instructions. No known bug.
Date: 2/2/2026: Bug fixed in ALU

No. of Visitor: <p>

![Visitor Count](https://profile-counter.glitch.me/charlie2951/count.svg)

