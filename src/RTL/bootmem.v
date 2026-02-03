`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 31.01.2026 21:50:25
// Design Name: 
// Module Name: bootmem
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////

//Verilog code for program memory
//Revised version of Memory to avoid DPB generation during synthesis
//Revised on 16/04/2025
//Author: Prof. Subir Kr. Maity
//Read only Boot Memory for bootloader
module bootmem(
    input rst, clk,
    input [31:0] addr,
    input [31:0] data_in,//this should be same size of data_outto avoid dual port RAM generation during synthesis
    input rd_strobe,
    input [3:0] wr_strobe,
    output reg [31:0] data_out

  );
  parameter MEM_SIZE = 2048;
  reg [31:0] BOOTMEM[0:MEM_SIZE-1]; //define program mem with 1024 location
  wire [29:0] mem_loc = {4'b0000,addr[27:2]};
  initial
  begin
    $readmemh("boot.mem", BOOTMEM);//load firmware into memory
  end
  always @(posedge clk)
  begin
    if(rst)
      data_out <= 32'h0;
    else if(rd_strobe) //data read from memory
      data_out <= BOOTMEM[mem_loc];
  end
  always @(posedge clk)
  begin
    if(wr_strobe[0])
      BOOTMEM[mem_loc][7:0] <= data_in[7:0];
    if(wr_strobe[1])
      BOOTMEM[mem_loc][15:8] <= data_in[15:8];
    if(wr_strobe[2])
      BOOTMEM[mem_loc][23:16] <= data_in[23:16];
    if(wr_strobe[3])
      BOOTMEM[mem_loc][31:24] <= data_in[31:24];

  end
endmodule

