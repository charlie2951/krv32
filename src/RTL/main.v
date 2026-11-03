module present_top (
    input  wire        clk,
    input  wire        rst,
    input  wire        enable,
    input  wire [63:0] plaintext,
    output reg  [63:0] ciphertext,
    output reg         done
);
    
    reg  [63:0] state;
    wire [63:0] after_addkey;
    wire [63:0] sbox_out;
    wire [63:0] p_layer_out;
    wire [63:0] round_key_from_keymod;
    reg  [5:0]  round_count; 

    // Internal reset pulse: Triggers on global reset OR during the final wait state
    wire auto_key_rst = rst || (round_count == 34);

    // Instantiate key module
    key key_sched (
        .clk(clk),
        .rst(auto_key_rst), 
        .enable(enable && (round_count > 0) && (round_count <= 31)), 
        .roundk(round_key_from_keymod)
    );

    assign after_addkey = state ^ round_key_from_keymod;

    // 16 S-Boxes
    genvar i;
    generate
        for (i = 0; i < 16; i = i + 1) begin : sbox_instances
            sbox_1 s (.datain(after_addkey[i*4 +: 4]), .sbox_out(sbox_out[i*4 +: 4]));
        end
    endgenerate

    // P-layer
    p_layer p_inst (
        .data_in(sbox_out),
        .data_out(p_layer_out)
    );

    // Control Logic with 2-Cycle Auto-Reset
    always @(posedge clk) begin
        if (rst) begin
            state       <= 64'b0;
            round_count <= 0;
            done        <= 0;
            ciphertext  <= 64'b0;
        end else begin
            if (round_count == 0) begin
                // IDLE / LOAD PHASE
                if (enable) begin
                    state <= plaintext;
                    round_count <= 1;
                    done <= 0;
                end
            end else if (round_count <= 31) begin
                // NORMAL ROUNDS
                state <= p_layer_out;
                round_count <= round_count + 1;
            end else if (round_count == 32) begin
                // FINAL WHITENING (Post-processing)
                ciphertext <= state ^ round_key_from_keymod;
                done <= 1;         // Assert done
                round_count <= 33; // Enter Wait Cycle 1
            end else if (round_count == 33) begin
                // WAIT CYCLE 1
                round_count <= 34; // Enter Wait Cycle 2
            end else if (round_count == 34) begin
                // WAIT CYCLE 2 & AUTO-RESET
                done <= 0;         // Clear done flag
                round_count <= 0;  // Reset FSM to Idle
            end
        end
    end

endmodule
