# A subset of RISC-V architecture implementation for low-resource FPGA 
A tiny version of RISC-V for low-resource FPGA developed by the Integrated System Design lab, School of Electronics, KIIT University
**Objective**
The main objective of this project is to prototype a RISC-V 32-bit CPU with an **RV32 base integer (RV32I) instruction set**. The CPU is coded using Verilog HDL from scratch and most of the instruction sets are implemented. The code is compatible with the GNU RISC-V toolchain (both assembler and compiler).

**RISC-V Architecture and Instruction Set** <p>
Refer to the RiSC-V official page and/or other tutorials. Some useful links are given below.
1. https://www2.eecs.berkeley.edu/Pubs/TechRpts/2016/EECS-2016-118.pdf
2. https://www.cs.sfu.ca/~ashriram/Courses/CS295/assets/notebooks/RISCV/RISCV_CARD.pdf
3. https://lists.riscv.org/g/tech-unprivileged/attachment/535/0/unpriv-isa-asciidoc.pdf
4. https://shakti.org.in/docs/risc-v-asm-manual.pdf
5. https://pages.hmc.edu/harris/ddca/ddcarv/DDCArv_AppB_Harris.pdf

**Debugging and Simulation** <p>
For debugging and behavioral simulation, use any Verilog compiler. I have used open-source Icarus Verilog with GTKWave waveform viewer. A sample testbench is added for debugging and test purposes. Modify the testbench as per your requirements.<p>
*Program Memory space*: Default is 128 byte. Each location will contain 8-bit data. However, you can change it in code. Verilog Implementation:* reg[7:0] ram[0:127]* <p>
*Data and I/O memory address space* By default 64 byte. Each location will contain 8-bit data. Verilog implementation: *reg[7:0] datamem [0:63]* <p>
I/O and data memory offset address start from 0x00002000. However, while using data memory for load and store instructions and I/O operations in RTL code, it uses datamem starting from the 0th location. <p>
Note: Data memory location 0 to 3 i.e. 2000 to 2003 is used for output. 2003 is mapped to six onboard LEDs in Gowin Semiconductor's Sipeed Tang-9 series FPGA board.<p>
  **FPGA Implementation**
The 1st version is implemented in Tang 9K series FPGA with a clock speed of 27 MHz. <p>

*Pin Assignment of FPGA*

![image](https://github.com/user-attachments/assets/0739668c-773d-4017-ae45-c3b8ade32404)

*Resource Utilization*
![image](https://github.com/user-attachments/assets/bd8db6f2-b492-446d-97b7-45cee75a54ea)

**Flowchart** <p>
The CPU is implemented in a straightforward way by keeping the code simple and understandable. Further optimization may be done to save hardware sources and speed. In this implementation, a multi-clock cycle is required to execute a single instruction. Currently, the R-type (ADD, SUB, AND, OR etc) and Immediate instructions (I-type) instructions (ADDI, ANDI, ORI, etc) consume 5 clock cycles, Load and store type instructions take 4 cycles, Branch and Jump instructions take 3 cycles, and others (LUI, AUIPC) take 4 cycles. Pipelining may be implemented in the future to speed up the execution. <p>
**Execution Flow in the State machine** <p>
The initial state is *RESET* state. After that the control goes to *FETCH* state.<p>
*FETCH:* The 32-bit instruction code is loaded from the program memory into 32 bit instruction register named data <p>
```verilog
FETCH: 
      begin
        data[31:24] <= ram[addr];
        data[23:16] <= ram[addr+1];
        data[15:8] <= ram[addr+2];
        data[7:0] <= ram[addr+3];
        state <= DECODE;
      end
```
*DECODE:* The decoding of instruction takes place in this state. A separate mode detector module is attached to decide the addressing mode.
```verilog
module addressing_mode
(
	input [6:0] opcode,
	output reg [3:0] mode
);

always @(opcode)
begin
case(opcode)
//R type
7'b0110011: mode = 1;
//I-type
7'b0010011: mode = 2;
//Load instruction
7'b0000011: mode = 3;
//Store
7'b0100011: mode = 4;
//Branch
7'b1100011: mode = 5;
//Jump
7'b1101111: mode = 6;
//LUI 
7'b0110111: mode = 7;
//AUIPC
7'b0010111: mode = 8;
//Reset
7'b0000000: mode = 0;
//JALR
7'b1100111: mode = 10;

default: mode = 9;
endcase
end
endmodule
```
Then the corresponding control and status signal is generated depending upon the addressing mode (R-type, I-type, branch, Jump etc). See Verilog file for details. <p>
*REG_RD:* The register read state reads the source register's content and also immediate data for (I-type) mode. <p>
*ALU:* Performs arithmetical and logical operations as per instructions and stores the result directly to the destination register in the register file (regfile).<p>
*REG_WR:* Perform data memory read and write operation for LOAD and STORE type instructions (LB, LW, LH, SB, SW, SH etc.) <p>
*PC_UPDATE:* Update the program counter (address value) to fetch the next instruction code from program memory.<p>
*BRANCHING:* State to control/handle branching instructions. Note that for this type of instruction, the program counter is updated inside the same state. <p>
*JUMPING:* Jump instructions are managed here along with the next address update.<p>
Other states are LUI and AUIPC to implement corresponding instructions. <p>
**Important** <p>
The default hex file generated by C compiler is 32 bit length in each field. The instruction memory (ram) accepts 8-bit data as shown below. So, in firmware.txt file, arrange the instruction code in 8 bit chunk. You can use a python script (split.py) available inside GCC folder (testcase/RISCV_GCC_testcases/counter/split.py)  to split the 32 bit hex code into 8 bit chunk.  <p>
If you are not familiar with hex code (machine code) generation from assembly code or C code, see the separate section mentioned below.<p>
*Sample instruction data format to be loaded in program memory (content of firmware.txt)*
```
08 00 01 13
00 80 00 EF
00 10 00 73
7C 00 28 23
03 C0 00 6F
```

## Steps for Generating HEX code from assembly language
Use any RISC-V assembler to convert your assembly code into Hex dump. One of such online assembler can be found here https://riscvasm.lucasteske.dev/  . Copy the code hex dump, reformat it into 8-bit chunk manually or using split.py and paste it inside firmware.txt file in Verilog directory. Then run simulation. You can use any other assembler such as RISC-V toolchain etc.
## Steps for Generating Hex code from RISC-V C code
1. Install prerequisite for Ubuntu
```
$ sudo apt-get install autoconf automake autotools-dev curl python3 python3-pip python3-tomli libmpc-dev libmpfr-dev libgmp-dev gawk build-essential bison flex texinfo gperf libtool patchutils bc zlib1g-dev libexpat-dev ninja-build git cmake libglib2.0-dev libslirp-dev
```
2. Install RISC-V gcc toolchain. You need a Linux PC or in Windows, Windows subsystem for linux with Ubuntu support. Follow the steps to install the toolchain <p>
Clone the official Risc-V gnu toolchain and compile for 32-bit arch with ilp32 option (RV32I) without multiplication support that we have implemented into FPGA.
```
$ git clone https://github.com/riscv/riscv-gnu-toolchain
./configure --prefix=/opt/riscv --with-arch=rv32i --with-abi=ilp32
make linux
```
Make sure that */opt/riscv* path having rw access. Otherwise change it to your preferred path. <p>
To compile toolchain for multilib support (both 32 bit and 64 bit), use the following <p>
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

#define LEDS_START_ADDR 2000
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
Here #define LEDS_START_ADDR 2000 point to the location where LED ports are connected (2000-2003). Initially, LEDS_DATA_REG = 0b000000 i.e. all LEDS are off. Then two nested for loops are provided to include delay so that changes can be observed in the  eye. The while(1) provides an infinite loop which is common for any embedded CPU. Once all LEDS are ON i.e. count=63, then the counter will be cleared and again it will start counting from the beginning. <p>

***Compiling the source code*** <p>
A dedicated makefile is provided (inside testcase located at (testcase/RISCV_GCC_testcases/counter/Makefile) to automate the task. Execute the following commands serially to generate the hex code with an 8-bit chunk. Note: if you are using 32-bit toolchain without multilib support then replace the *riscv64*  keyword by using *riscv32* and also check the GCC toolchain path mentioned in the Makefile. In my case, it is *RISCV_TOOLCHAIN_DIR = /home/kiit/riscv/bin*. Change it as per your installation.

```shell
make clean
make
python3 split.py
```
Copy the content of the generated firmware.txt file (you may exclude zeros) into your FPGA's firmware.txt and run the FPGA design flow or simulate it for debugging. You may open the *dumpfile* in text editor to see the generated assembly code from C code. This is useful for debugging. <p>

***Points to Remember*** <p>
1. *Program memory size:* The default size is 128 byte. See the generated firmware.txt file. If it crosses 32 lines (=32 x 4) excluding the last rows of ZEROS, the code will not fit into 128 byte memory space. You have to increase it. For that, open Verilog file *cpu.v* and change the line *reg[7:0] ram [0:127]* to the required value i.e. *reg[7:0] ram [0:255]*. Also, edit the *Makefile* and change the variable *MEM_SIZE = 128* to the required value. Then open the loader script *sections.lds* and change the LENGTH variable *mem : ORIGIN = 0x00000000, LENGTH = 128* to the required value. Also, do not write any data into *datamem* starting from location 0-3 (actually in address space it is 2000-2003). It is reserved for LED port.
2. *Number of Registers in Register file* : The full RISC-V (RV-32I) architecture having total 32 register (X0-X31). In the current version of CPU, we have used a total 16 registers (X0-X15). If you need more registers (all 32 regs) then edit the *cpu.v* line *reg [31:0] regfile[0:15]* to *reg [31:0] regfile[0:31]*. However, this will require more hardware resources on FPGA. <p>
3. Traps or any other interrupts are not supported. CSR instructions are not supported in the current version. <p>
***N.B.*** This is the initial version of cpu and may contain additional bug.
