//Verilog code for program memory
//Revised version of Memory to avoid DPB generation during synthesis
//Revised on 16/04/2025
//Author: Prof. Subir Kr. Maity
module progmem(
    input rst, clk,
    input [31:0] addr,
    input [31:0] data_in,//this should be same size of data_outto avoid dual port RAM generation during synthesis
    input rd_strobe,
    input [3:0] wr_strobe,
    output reg [31:0] data_out

  );
  parameter MEM_SIZE = 1024;
  reg [31:0] PROGMEM[0:MEM_SIZE-1]; //define program mem with 1024 location
  wire [29:0] mem_loc = addr[31:2];
  initial
  begin
    $readmemh("firmware.hex", PROGMEM);//load firmware into memory
  end
  always @(posedge clk)
  begin
    if(rst)
      data_out <= 32'h0;
    else if(rd_strobe) //data read from memory
      data_out <= PROGMEM[mem_loc];
  end

  always @(posedge clk)
  begin
    if(wr_strobe[0])
      PROGMEM[mem_loc][7:0] <= data_in[7:0];
    if(wr_strobe[1])
      PROGMEM[mem_loc][15:8] <= data_in[15:8];
    if(wr_strobe[2])
      PROGMEM[mem_loc][23:16] <= data_in[23:16];
    if(wr_strobe[3])
      PROGMEM[mem_loc][31:24] <= data_in[31:24];

  end






endmodule
