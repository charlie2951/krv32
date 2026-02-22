`timescale 1ns/1ps

module timer1_mmio (
    input  wire        clk,
    input  wire        rst_n,

    // ===============================
    // RISC-V MMIO Bus Interface
    // ===============================
    input  wire [31:0] addr,
    input  wire [31:0] data_in,
    output reg  [31:0] data_out,
    input  wire        rd_strobe,
    input  wire [3:0]  wr_strobe
);

// ======================================================
// Base Address
// ======================================================
//localparam BASE_ADDR = 32'hB000_0010;

// Offsets
localparam REG_CTRL      = 4'h0;
localparam REG_TIMER_VAL = 4'h4;
localparam REG_COUNT_VAL = 4'h8;
localparam REG_STATUS    = 4'hC;

// ======================================================
// Internal Registers
// ======================================================
reg        start_reg;
reg        mode_reg;
reg [31:0] timer_val_reg;
reg        done_status;

// ======================================================
// Address decode
// ======================================================
//wire [31:0] offset = addr - BASE_ADDR;
wire isTimer1 = (addr[31:4]==28'hB000001);
wire [3:0] offset = addr[3:0];
wire        write_en = |wr_strobe;

// ======================================================
// Timer Instance
// ======================================================
wire done_wire;
wire [31:0] count_value;

timer_counter u_timer (
    .rst_n       (rst_n),
    .clk         (clk),
    .start       (start_reg),
    .mode        (mode_reg),
    .timer_val   (timer_val_reg),
    .done        (done_wire),
    .count_value (count_value)
);

// ======================================================
// Write Logic
// ======================================================
always @(posedge clk or negedge rst_n) begin
    if(!rst_n) begin
        start_reg      <= 0;
        mode_reg       <= 0;
        timer_val_reg  <= 0;
        done_status    <= 0;
    end
    else begin
        // Latch done from timer
        if(done_wire)
            done_status <= 1;

        // Register writes
       // if(write_en && (addr >= BASE_ADDR)) begin
            if(write_en && isTimer1) begin
            case(offset)

                REG_CTRL: begin
                    if(wr_strobe[0]) begin
                        start_reg <= data_in[0];
                        mode_reg  <= data_in[1];
                    end
                end

                REG_TIMER_VAL: begin
                    if(wr_strobe[0]) timer_val_reg[7:0]   <= data_in[7:0];
                    if(wr_strobe[1]) timer_val_reg[15:8]  <= data_in[15:8];
                    if(wr_strobe[2]) timer_val_reg[23:16] <= data_in[23:16];
                    if(wr_strobe[3]) timer_val_reg[31:24] <= data_in[31:24];
                end
                 
                REG_STATUS: begin
                    // Write-1-to-Clear
                    //if(|wr_strobe && data_in[0])
                     //   done_status <= 0;
                     if(|wr_strobe) done_status <= data_in[0];
                    
                end
                
            endcase
        end

        // Auto-clear start (1-cycle pulse)
        //start_reg <= 0;
    end
end

// ======================================================
// Read Logic
// ======================================================
always @(posedge clk) begin
    if(rd_strobe && isTimer1) begin
        case(offset)
          /*
            REG_CTRL:
                data_out = {30'b0, mode_reg, 1'b0};

            REG_TIMER_VAL:
                data_out = timer_val_reg;
         */
            REG_COUNT_VAL:
                data_out <= count_value;

            REG_STATUS:
                data_out <= {31'b0, done_status};

            default:
                data_out <= 32'b0;
        endcase
    end
    else
        data_out <= 32'b0;
end

endmodule
