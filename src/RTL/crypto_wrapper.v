//*** ENCRYPTION ADDRESS RANGE ***//
//AES GPIO unit to control AES ENCRYPTION
//address range: ENC_DATA_IN: 0x0100_0000
//ENC_VALID_IN: 0x0200_0000
//ENC_KEY1 : 0x0300_0000
//ENC_KEY2: 0x0400_0000
//ENC_VALID_OUT: 0X0500_0000
//ENC_DATA_OUT: 0X0600_0000
//*** DECRYPTION ADDRESS RANGE***//
//AES GPIO unit to control AES DECRYPTION
//address range: DEC_DATA_IN: 0x0700_0000
//DEC_VALID_IN: 0x0800_0000
//DEC_KEY1 : 0x0900_0000
//DEC_KEY2: 0x0A00_0000
//DEC_VALID_OUT: 0X0B00_0000
//DEC_DATA_OUT: 0X0C00_0000

module  CRYPTO_GPIO  
(
    input [31:0] addr,
    input rst, clk,
    input [31:0] data_in,
    input rd_strobe,
    input [3:0] wr_strobe,
    output reg [31:0] data_out
    );

  reg [31:0] enc_data_in,dec_data_in;
  wire [31:0] enc_data_out, dec_data_out;
  reg [31:0] enc_key1, enc_key2,dec_key1,dec_key2;
  reg [31:0] enc_valid_in;
  reg [31:0] dec_valid_in;
  wire enc_valid_out, dec_valid_out;
  //flags
  //Encryption part
  wire isenc_data_in = (addr==32'h01000000);
  wire isenc_valid_in =(addr==32'h02000000);
  wire isenc_key1 =    (addr==32'h03000000);
  wire isenc_key2 =    (addr==32'h04000000);
  wire isenc_valid_out=(addr==32'h05000000);
  wire isenc_data_out =(addr==32'h06000000);
  //Decryption part
  wire isdec_data_in = (addr==32'h07000000);
  wire isdec_valid_in= (addr==32'h08000000);
  wire isdec_key1    = (addr==32'h09000000);
  wire isdec_key2  =   (addr==32'h0a000000);
  wire isdec_valid_out=(addr==32'h0b000000);
  wire isdec_data_out= (addr==32'h0c000000);
  
  initial
  begin
    dec_valid_in <= 0;
    enc_valid_in <= 0;
    //enc_data_out <= 0;
    //dec_data_out <= 0;
     end

  always @(posedge clk)
  begin
    if(!rst)
    begin
      
    dec_valid_in <= 0;
    enc_valid_in <= 0;
    //enc_data_out <= 0;
    //dec_data_out <= 0;
    end
    else begin
// decryption part
     if(rd_strobe && isdec_valid_out)
      data_out <= {31'b0,dec_valid_out};
     else if(rd_strobe && isdec_data_out)
      data_out <= dec_data_out;
     else if(|wr_strobe && isdec_data_in)
      dec_data_in <= data_in;
     else if(|wr_strobe && isdec_valid_in)
      dec_valid_in <= data_in;
     else if(|wr_strobe && isdec_key1)
      dec_key1 <= data_in;
     else if(|wr_strobe && isdec_key2)
      dec_key2 <= data_in;
//encryption part logic
     else if(rd_strobe && isenc_data_out)
      data_out <= enc_data_out;
   //  else if(rd_strobe && isenc_valid_out)
    //  data_out <= {31'b0,enc_valid_out};
     else if(|wr_strobe && isenc_data_in)
      enc_data_in <= data_in;
     else if(|wr_strobe && isenc_valid_in)
      enc_valid_in <= data_in;
      else if(|wr_strobe && isenc_key1)
      enc_key1 <= data_in;
      else if(|wr_strobe && isenc_key2)
      enc_key2 <= data_in;  
      end
end

 //Instantiate Encryption module here
ENCRYPTION_MODULE enc0(
   .clk(clk),        // Clock input
   .rst_n(rst),      // Active-low reset
   .valid_in(enc_valid_in[0]),   // Input valid signal
   .ciphertext(enc_data_out), // encrypted output
   .key1(enc_key1),       // First encryption key
   .key2(enc_key2),       // Second encryption key
   .plaintext(enc_data_in), // plaintext input as 32 bit
   .valid_out(enc_valid_out)   // Output valid signal
);
 //Instantiate Decryption module here
  DECRYPTION_MODULE dec0(
   .clk(clk),        // Clock input
   .rst_n(rst),      // Active-low reset
   .valid_in(dec_valid_in[0]),   // Input valid signal
   .ciphertext(dec_data_in), // Input ciphertext
   .key1(dec_key1),       // First decryption key
   .key2(dec_key2),       // Second decryption key
   .plaintext(dec_data_out), // Decrypted output
   .valid_out(dec_valid_out)   // Output valid signal
);
endmodule

