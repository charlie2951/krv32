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
I/O and data memory offset address start from 0x00002000. However in RTL code, while using data memory for load and store instructions and I/O operations, it uses datamem starting from the 0th location. <p>
Note: Data memory location 0 to 3 i.e. 0x2000 to 0x2003 is used for output. 0x2003 is mapped to six onboard LEDs in Gowin Semiconductor's Sipeed Tang-9 series FPGA board.<p>
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
The instruction memory (ram) accepts 8 bit data as shown below. So, in firmware.hex file arrange the instruction code in 8 bit chunk. You can use a python script (split.py) available inside GCC folder to split the 32 bit hex code into 8 bit chunk. <p>
*Sample instruction data format to be loaded in program memory (content of firmware.hex)*
```
08 00 01 13
00 80 00 EF
00 10 00 73
7C 00 28 23
03 C0 00 6F
```

