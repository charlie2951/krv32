`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 7.04.2026 21:50:25
// Design Name: 
// Module Name: startmem
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

//Verilog code for startup memory
//Revised version of Memory to avoid DPB generation during synthesis
//Revised on 16/04/2025
//Author: Prof. Subir Kr. Maity
//Memory to store startup code whicg copies flash content to BRAM
module startmem(
    input rst, clk,
    input [31:0] addr,
    input [31:0] data_in,//this should be same size of data_outto avoid dual port RAM generation during synthesis
    input rd_strobe,
    input [3:0] wr_strobe,
    output reg [31:0] data_out

  );
  parameter MEM_SIZE = 2048; //8kb start memory
  reg [31:0] STARTMEM[0:MEM_SIZE-1]; //define program mem with 1024 location
  wire [29:0] mem_loc = {4'b0000,addr[27:2]};
  initial
  begin
    $readmemh("startup.mem", STARTMEM);//load firmware into memory
  end
  always @(posedge clk)
  begin
    if(rst)
      data_out <= 32'h0;
    else if(rd_strobe) //data read from memory
      data_out <= STARTMEM[mem_loc];
  end
  always @(posedge clk)
  begin
    if(wr_strobe[0])
      STARTMEM[mem_loc][7:0] <= data_in[7:0];
    if(wr_strobe[1])
      STARTMEM[mem_loc][15:8] <= data_in[15:8];
    if(wr_strobe[2])
     STARTMEM[mem_loc][23:16] <= data_in[23:16];
    if(wr_strobe[3])
      STARTMEM[mem_loc][31:24] <= data_in[31:24];

  end
endmodule

