`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 23.07.2025 11:34:27
// Design Name: 
// Module Name: led_gpio
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


//LED GPIO unit to access on board LEDs
//address of LED reg is 0x1000_0000
module  led_gpio(
    input [31:0] addr,
    input rst, clk,
    input [31:0] data_in,
    input rd_strobe,
    input [3:0] wr_strobe,
    output reg [31:0] data_out,
    output [5:0] leds
  );

  reg [5:0] led_data_reg;

  always @(posedge clk)
  begin
    if(!rst)
      led_data_reg <= 0;
    else if(rd_strobe)
      data_out <= {26'h0,led_data_reg};
    else if(|wr_strobe)
      led_data_reg <= data_in[5:0];
  end

  assign leds = led_data_reg;
endmodule

