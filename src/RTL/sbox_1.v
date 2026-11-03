`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 07.08.2025 23:58:52
// Design Name: 
// Module Name: sbox_1
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


module sbox_1(
    input  wire [3:0] datain,
    output reg  [3:0] sbox_out
    );

    always @(*) begin
        case(datain)
            4'h0: sbox_out = 4'hC;
            4'h1: sbox_out = 4'h5;
            4'h2: sbox_out = 4'h6;
            4'h3: sbox_out = 4'hB;
            4'h4: sbox_out = 4'h9;
            4'h5: sbox_out = 4'h0;
            4'h6: sbox_out = 4'hA;
            4'h7: sbox_out = 4'hD;
            4'h8: sbox_out = 4'h3;
            4'h9: sbox_out = 4'hE;
            4'hA: sbox_out = 4'hF;
            4'hB: sbox_out = 4'h8;
            4'hC: sbox_out = 4'h4;
            4'hD: sbox_out = 4'h7;
            4'hE: sbox_out = 4'h1;
            4'hF: sbox_out = 4'h2;
            default: sbox_out = 4'h0;
        endcase
    end
endmodule