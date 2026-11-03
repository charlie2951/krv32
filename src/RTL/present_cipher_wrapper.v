`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 15.01.2026 23:09:39
// Design Name: 
// Module Name: present_cipher_wrapper
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


module present_cipher_wrapper(
    input [31:0] addr,
    input rst, clk,
    input [31:0] data_in,
    input rd_strobe,
    input [3:0] wr_strobe,
    output reg [31:0] data_out
    );
    wire done;
    reg [31:0] en;
    reg [63:0] plaintxt;
    wire [63:0] ciphertxt;
    //address mapping
  wire isDONE= (addr[31:24]==8'h12);
  wire isEN= (addr[31:24]==8'h15);
  //64 bit plaintxt input flags
  wire isD_IN_01= (addr[31:24]==8'h16);//lower 32 bit
  wire isD_IN_02= (addr[31:24]==8'h17);//upper 32 bit
  //64 bit cipher txt flags
  wire isD_OUT_01= (addr[31:24]==8'h13);//lower 32bit
  wire isD_OUT_02= (addr[31:24]==8'h14);//upper 32 bit
    
  always @(posedge clk) begin
    if(rst) begin
    plaintxt <= 64'h0;
    en <=32'h0;
    end
    else if(rd_strobe && isDONE)
      data_out <= {31'h0, done};
      //collect 64 bit output
     else if(rd_strobe && isD_OUT_01)
      data_out <= ciphertxt[31:0];//lower 32 bit of output
    else if(rd_strobe && isD_OUT_02)
        data_out <= ciphertxt[63:32];//upper 32 bit of output
      //collect 64bit input
    else if(|wr_strobe && isD_IN_01)
        plaintxt[31:0] <= data_in;//lower 32bit
    else if(|wr_strobe && isD_IN_02)
        plaintxt[63:32] <= data_in;//upper 32 bit
    else if(|wr_strobe && isEN)
        en <= data_in;
    end
    
    //Instantiate cipher module
    present_top encrypt0(
    .clk(clk),
    .rst(rst),
    .enable(en[0]),
    .plaintext(plaintxt),
    .ciphertext(ciphertxt),
    .done(done)
);
endmodule
