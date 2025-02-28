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
The CPU is implemented in a straightforward way by keeping the code simple and understandable. Further optimization may be done to save hardware sources and speed. In this implementation, a multi-clock cycle is required to execute a single instruction. Currently, the R-type (ADD, SUB, AND, OR etc) and Immediate instructions (I-type) instructions (ADDI, ANDI, ORI, etc) consume 5 clock cycles, Load and store type instructions take 4 cycles, Branch and Jump instructions take 3 cycles, and others (LUI, AUIPC) take 4 cycles. Pipelining may be implemented in the future to speed up the execution.
