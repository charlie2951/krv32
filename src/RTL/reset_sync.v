module reset_sync_n (
    input  wire clk,           // Your CPU/System clock
    input  wire async_reset_in, // Input (Active-High from btnC/PLL)
    output wire sync_reset_n    // Output (Active-Low: 0 = Reset, 1 = Run)
);

    // Stage 1 & 2 flip-flops
    // Initialize to 0 so the CPU starts in Reset immediately at power-up
    reg [1:0] sync_reg = 2'b00; 

    // Trigger on posedge of clk OR posedge of the incoming high reset
    always @(posedge clk or posedge async_reset_in) begin
        if (async_reset_in) begin
            // If button is pressed (1), force internal registers to 0 (Reset)
            sync_reg <= 2'b00;
        end else begin
            // When button is released, shift in 1s to release reset synchronously
            sync_reg <= {sync_reg[0], 1'b1};
        end
    end

    // Output is the 2nd stage
    assign sync_reset_n = sync_reg[1];

endmodule