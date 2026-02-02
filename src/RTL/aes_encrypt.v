`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 11.01.2026 11:32:12
// Design Name: 
// Module Name: ENCRYPTION_MODULE
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


module ENCRYPTION_MODULE(
    input         clk,        // Clock input
    input         rst_n,      // Active-low reset
    input         valid_in,   // Input valid signal
    input  [31:0] plaintext,  // Input plaintext
    input  [31:0] key1,       // First encryption key
    input  [31:0] key2,       // Second encryption key
    output reg [31:0] ciphertext, // Encrypted output
    output reg    valid_out   // Output valid signal
);
    // Internal registers for encryption stages
    reg [31:0] stage1, stage2, stage3;
    wire [31:0] encrypted_data;
    
    // Multi-stage encryption using combinational logic
    always @(*) begin
        // Stage 1: XOR with first key and bit manipulation
        stage1 = plaintext ^ key1;
        
        // Stage 2: Rotate and shift operations
        stage2[31:24] = {stage1[26:24], stage1[31:27]};  // Rotate bits
        stage2[23:16] = stage1[23:16] + key2[31:24];     // Add key byte
        stage2[15:8]  = stage1[15:8] ^ key2[23:16];      // XOR key byte
        stage2[7:0]   = {stage1[4:0], stage1[7:5]};      // Rotate bits
        
        // Stage 3: Final mixing with second key
        stage3 = stage2 + key2;
    end
    
    assign encrypted_data = stage3;
    
    // Sequential output logic
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            ciphertext <= 32'h0;
            valid_out <= 1'b0;
        end
        else begin
            if (valid_in) begin
                ciphertext <= encrypted_data;
                valid_out <= 1'b1;
            end
            else begin
                valid_out <= 1'b0;
            end
        end
    end
endmodule