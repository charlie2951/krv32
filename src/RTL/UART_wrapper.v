`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 13.01.2026 23:08:51
// Design Name: 
// Module Name: UART_wrapper
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


module  uart0_wrapper(
    input [31:0] addr,
    input rst, clk,
    input [31:0] data_in,
    input rd_strobe,
    input [3:0] wr_strobe,
    output [31:0] data_out,
    output uart_tx,
    input uart_rx
  );
  
  wire [31:0] tx_data_out, rx_data_out;
  
  assign data_out = (addr==32'h20000010)|(addr==32'h20000018)?rx_data_out:
                    (addr==32'h20000008)? tx_data_out:32'h0;
  //uart tx gpio instantiation
  uart0_tx_gpio uart0_tx_regs(
    .addr(addr),
    .rst(rst), .clk(clk),
    .data_in(data_in),
    .rd_strobe(rd_strobe),
    .wr_strobe(wr_strobe),
    .data_out(tx_data_out),
    .uart_tx(uart_tx)
  );
  
  //uart Rx gpio instantiation
  uart0_rx_gpio uart0_rx_regs(
    .addr(addr),
    .rst(rst), .clk(clk),
    .data_in(data_in),
    .rd_strobe(rd_strobe),
    .wr_strobe(wr_strobe),
    .data_out(rx_data_out),
    .uart_rx(uart_rx)
  );
  
endmodule
