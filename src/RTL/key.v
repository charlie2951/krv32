module key (
    input  wire clk,
    input  wire rst,
    input  wire enable,
    output wire [63:0] roundk // Changed to wire
);

    // CHANGE THIS PARAMETER TO CHANGE YOUR FIXED KEY
    parameter [79:0] MASTER_KEY = 80'hFFFFFFFFFFFFFFFFFFFF;

    reg [79:0] kreg;
    reg [4:0]  rcount;

    // Combinational Round Key Output (Immediate)
    assign roundk = kreg[79:16];

    // Helper: Rotate Left 61 (== Rotate Right 19)
    wire [79:0] rotated = {kreg[18:0], kreg[79:19]};
    
    // Sbox instance for Key Schedule
    wire [3:0] sbox_out;
    sbox_1 key_sbox (.datain(rotated[79:76]), .sbox_out(sbox_out));

    always @(posedge clk) begin
        if (rst) begin
            kreg   <= MASTER_KEY; // Reset to fixed hardcoded key
            rcount <= 1;
        end else if (enable) begin
            // PRESENT-80 Key Update:
            // 1. Rotate Left 61
            // 2. S-Box top 4 bits
            // 3. XOR bits [19:15] with round_counter
            
            kreg <= {
                sbox_out,                   // [79:76]
                rotated[75:20],             // [75:20]
                rotated[19:15] ^ rcount,    // [19:15]
                rotated[14:0]               // [14:0]
            };

            rcount <= rcount + 1;
        end
    end
endmodule