`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 14.02.2026 10:04:45
// Design Name: 
// Module Name: top_test
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
//sample testbench template of SOC

module top_test;
reg rst, clk, boot_en;
wire [15:0] gpio;
    
   pullup p1[15:0](gpio); 
    top dut(
    .rst(rst), .clk(clk),
    .boot_en(boot_en),
    .uart0_rx(),
    .uart0_tx(),
    .uart1_rx(),
    .uart1_tx(),
    .gpio(gpio)
    );
    
    initial begin
    rst=0; clk=0; boot_en=0;
    #100;
    rst=1;
    #10000;
    $finish;
    end
    always #5 clk=!clk;
endmodule
