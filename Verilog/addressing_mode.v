// Instruction type or Addressing mode decoder
//It decides the type of instruction (R, I, S, L, Jump etc)
//For details refer to Risc-V instruction set manual
//Input is 7 bit opcode
//Output is 3 bit control signal

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
//EBREAK
7'h73:	mode = 11;

default: mode = 9;
endcase
end
endmodule

