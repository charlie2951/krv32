module gpio_controller (
    input  wire        clk,
    input  wire        rst_n,

    // Bus Interface
    input  wire [31:0] addr,
    input  wire [31:0] data_in,
    output reg  [31:0] data_out,
    input  wire        rd_strobe,
    input  wire [3:0]  wr_strobe,

    // Physical Pins
    inout  wire [15:0] gpio_pins
);

    // Internal Registers
    reg [15:0] out_reg;  // Data to drive out
    reg [15:0] dir_reg;  // 1 = Output, 0 = Input (High-Z)
    wire [15:0] in_reg;  // Synced input values

    // Address Decoding
    wire sel_data = (addr == 32'h10000000);
    wire sel_dir  = (addr == 32'h10000004);

    // --- Write Logic ---
    always @(posedge clk ) begin
        if (rst_n) begin
            out_reg <= 16'h0000;
            dir_reg <= 16'h0000; // Default to all Inputs for safety
        end else begin
            if (sel_data && |wr_strobe) begin
                if (wr_strobe[0]) out_reg[7:0]  <= data_in[7:0];
                if (wr_strobe[1]) out_reg[15:8] <= data_in[15:8];
            end
            if (sel_dir && |wr_strobe) begin
                if (wr_strobe[0]) dir_reg[7:0]  <= data_in[7:0];
                if (wr_strobe[1]) dir_reg[15:8] <= data_in[15:8];
            end
        end
    end

    // --- Read Logic ---
    always @(*) begin
        case (addr)
            32'h10000000: data_out = {16'b0, in_reg};
            32'h10000004: data_out = {16'b0, dir_reg};
            default:      data_out = 32'h0;
        endcase
    end

    // --- Tri-state Buffer Logic ---
    // If dir is 1, drive out_reg. If 0, drive high-impedance 'z'.
    generate
        genvar i;
        for (i = 0; i < 16; i = i + 1) begin : tri_state_logic
            assign gpio_pins[i] = dir_reg[i] ? out_reg[i] : 1'bz;
            assign in_reg[i] = gpio_pins[i]; // Feedback to read the actual pin state
        end
    endgenerate

endmodule