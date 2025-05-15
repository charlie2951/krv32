# A subset of RISC-V architecture (RV32I) implementation for low-resource FPGA 
A tiny version of RISC-V for low-resource FPGA developed by the Integrated System Design lab, School of Electronics, KIIT University <p>
**Objective**
The main objective of this project is to prototype a RISC-V 32-bit CPU with an **RV32 base integer (RV32I) instruction set**. The CPU is coded using Verilog HDL from scratch and most of the instruction sets are implemented. The code is compatible with the GNU RISC-V toolchain (both assembler and compiler).
## Features of KRV-32 soft IP SOC
1. Supports RISC-V RV-32I extension
2. Compatible with GNU toolchain
3. Soft-IP, configurable as per your FPGA
4. On-board LEDS can be interfaced using C SDK
5. Fully functional UART Transmitter and Receiver (currently support 9600 baud rate only)
6. Four-channel PWM output with a frequency of 500HZ and duty cycle 0-100%
   <p></p>
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
Synthesized and implemented in Tang 9K series FPGA with a clock speed of 27 MHz. <p>

*Pin Assignment of FPGA*
![image](https://github.com/user-attachments/assets/b1e995d8-2de4-4b03-b7cc-a8b9e1b279ee)


*Resource Utilization*
![image](https://github.com/user-attachments/assets/69265fbb-0129-404b-a391-1bc28b14e25f) <p>

## GPIO mapping along with FPGA pin details

| Peripheral | Base Address |   Tang9K FPGA used Pins   |      Nexys4- DDR FPGA used Pins      |
|------------|--------------|:-------------------------:|:------------------------------------:|
| LED        |  0x1000_0000 |     10,11,13, 14,15,16    | V11,V12, V14, V15, T16,U14, T15, V16 |
| UART_TX    |  0x2000_0000 |             17            |                  D4                  |
| UART_RX    |  0x3000_0000 |             18            |                  C4                  |
| PWM        |  0x4000_0000 | 4 on-board LEDS  are used |     RGB LEDs- N16, R11, G14, N15     |

<p>
LED GPIO  is mapped to six onboard active-low enabled LEDs in Gowin Semiconductor's Sipeed Tang-9 series FPGA board. For Nexys-4 DDR board which uses Artix-7 series FPGA, LEDS are active high and total 8 LEDs are connected. Refer to the table mentioned above. <p>
	
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
Then the corresponding control and status signal is generated depending upon the addressing mode (R-type, I-type, branch, Jump etc). See Verilog file for details. <p>
*REG_RD:* The register read state reads the source register's content and also the immediate data for (I-type) mode. <p>
*ALU:* Performs arithmetical and logical operations as per instructions and stores the result directly to the destination register in the register file (regfile).<p>
*REG_WR:* Perform data memory read and write operation for LOAD and STORE type instructions (LB, LW, LH, SB, SW, SH etc.) <p>
*PC_UPDATE:* Update the program counter (address value) to fetch the next instruction code from program memory.<p>
*BRANCHING:* State to control/handle branching instructions. Note that for this type of instruction, the program counter is updated inside the same state. <p>
*JUMPING:* Jump instructions are managed here along with the next address update.<p>
Other states are LUI and AUIPC to implement the corresponding instructions. <p>
## Steps for Generating HEX code from assembly language
Use any RISC-V assembler to convert your assembly code into Hex dump. One of such online assembler can be found here https://riscvasm.lucasteske.dev/  . Copy the code hex dump, and paste it inside firmware.hex file in Verilog directory. Then run simulation. You can use any other assembler such as RISC-V toolchain etc.
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
Make sure that */opt/riscv* path has rw access. Otherwise, change it to your preferred path. <p>
To compile a toolchain for multilib support (both 32-bit and 64-bit), use the following <p>
```
./configure --prefix=/opt/riscv --enable-multilib
make linux
```
The multilib compiler will have the prefix riscv64-unknown-elf- or riscv64-unknown-linux-gnu- but will be able to target both 32-bit and 64-bit systems. It will support the most common -march/-mabi options, which can be seen by using the --print-multi-lib flag on either cross-compiler.<p>

**Test a Sample counter program** <p>
Open a test editor and add the following piece of code. Save the code as *main.c*<p>
```c
#include <stdint.h>
#include <stdlib.h>

#define LEDS_START_ADDR 0x10000000
#define LEDS_DATA_REG_OFFSET 0
#define LEDS_DATA_REG *((volatile unsigned int *)(LEDS_START_ADDR + LEDS_DATA_REG_OFFSET))

int main(void)
{

    LEDS_DATA_REG = 0b000000;
    int  i=0, j=0, k=0;

    while (1)
    {
	if(LEDS_DATA_REG == 64)
	LEDS_DATA_REG = 0;
       
       for(i=0;i<1000;i++)
	{
		for(j=0;j<1000;j++){
		}
	}
     	LEDS_DATA_REG = LEDS_DATA_REG + 1;		
}
    return 0;
}

```
Here #define LEDS_START_ADDR 0x10000000 points to the location where LED ports are connected (2000-2003). Initially, LEDS_DATA_REG = 0b000000, i.e., all LEDS are off. The,n two nested for loops are provided to include a delay so that changes can be observed in the  eye. The while(1) provides an infinite loop, which is common for any embedded CPU. Once all LEDS are ON i.e., count=63, then the counter will be cleared and again it will start counting from the beginning. <p>

***Compiling the source code*** <p>
A dedicated makefile is provided (inside testcase located at (testcase/RISCV_GCC_testcases/counter/Makefile) to automate the task. Execute the following commands serially to generate the hex code with an 8-bit chunk. Note: if you are using 32-bit toolchain without multilib support then replace the *riscv64*  keyword by using *riscv32* and also check the GCC toolchain path mentioned in the Makefile. In my case, it is *RISCV_TOOLCHAIN_DIR = /home/kiit/riscv/bin*. Change it as per your installation.

```shell
make clean
make
```
Copy the content of the generated firmware.txt file (you may exclude zeros) into your FPGA's firmware.txt and run the FPGA design flow or simulate it for debugging. You may open the *dumpfile* in text editor to see the generated assembly code from C code. This is useful for debugging. <p>

***Points to Remember*** <p>
1. *Program memory size:* The default size is 1KB. See the generated firmware.hex file. If it crosses 256 lines (= 256 x 4), excluding the last rows of ZEROS, the code will not fit into 1KB memory space. You have to increase it. For that, open Verilog file *progmem.v* and change the line *parameter MEM_SIZE=1024* to the required value. Also, edit the *Makefile* and change the variable *MEM_SIZE = 1024* to the required value. Then open the loader script *sections.lds* and change the LENGTH variable *mem : ORIGIN = 0x00000000, LENGTH = 1K* to the required value. <p>
2. *Whether Verilog core support toolchain generated opcode?* :  See the *dumpfile* after C compilation and check the registers and instructions used. This version of CPU supports almost all commonly used instructions unless mentioned at the top of cpu.v <p>
3. Traps or any other interrupts are not supported. CSR instructions are not supported in the current version. <p>
***N.B.*** This is the initial version of CPU and may contain additional bugs. <p>

## Schematic of GOWIN Tang 9k FPGA
![image](https://github.com/user-attachments/assets/23155624-dbff-4b06-b6cf-676d198d0315)
## FPGA Implementation Examples
Go through the ***fpga*** directory to explore the compiled and implemented project in the Tang9K FPGA from GOWIN. Note that Tang9k uses an active low reset. Also LED port is 6 bits. So there are minor changes in reset logic (!rst instead of rst) and top module (top.v).

## Revision Note
Version-4.1 is more hardware efficient in terms of FPGA resources. Previous versions (V1.0) consume 34% LUT, V2 takes only 14%. Program memory is isolated from the CPU core to bring more flexibility. GPIO logic is also isolated and implemented at top.v file (top module).

***Known issues/bugs*** <p>
Date: 16/04/2025: Bug: Arithmetic right shift SRA, SRAI not working<p>
