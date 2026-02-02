`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 11.01.2026 12:12:19
// Design Name: 
// Module Name: DECRYPTION_MODULE
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


module DECRYPTION_MODULE(

    input         clk,        // Clock input
    input         rst_n,      // Active-low reset
    input         valid_in,   // Input valid signal
    input  [31:0] ciphertext, // Input ciphertext
    input  [31:0] key1,       // First decryption key
    input  [31:0] key2,       // Second decryption key
    output reg [31:0] plaintext, // Decrypted output
    output reg    valid_out   // Output valid signal
);
    // Internal wires for decryption stages
    wire [31:0] stage1_dec, stage2_dec, stage3_dec;
    
    // Stage 1: Reverse final mixing (subtract key2)
    assign stage1_dec = ciphertext - key2;
    
    // Stage 2: Reverse rotate and shift operations
    assign stage2_dec[31:27] = stage1_dec[28:24];        // Reverse rotate
    assign stage2_dec[26:24] = stage1_dec[31:29];        // Reverse rotate
    assign stage2_dec[23:16] = stage1_dec[23:16] - key2[31:24]; // Subtract key byte
    assign stage2_dec[15:8]  = stage1_dec[15:8] ^ key2[23:16];  // XOR key byte (self-inverse)
    assign stage2_dec[7:5]   = stage1_dec[2:0];          // Reverse rotate
    assign stage2_dec[4:0]   = stage1_dec[7:3];          // Reverse rotate
    
    // Stage 3: Reverse initial XOR
    assign stage3_dec = stage2_dec ^ key1;
    
    // Sequential output logic
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            plaintext <= 32'h0;
            valid_out <= 1'b0;
        end
        else begin
            if (valid_in) begin
                plaintext <= stage3_dec;
                valid_out <= 1'b1;
            end
            else begin
                valid_out <= 1'b0;
            end
        end
    end
   
endmodule
