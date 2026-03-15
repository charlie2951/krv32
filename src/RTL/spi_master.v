module spi_master_full (
    input  wire        clk,       // System Clock (e.g., 100MHz)
    input  wire        reset,     // Active high
    input  wire [7:0]  tx_data,   // Data to send
    input  wire        start,     // Trigger pulse
    output reg  [7:0]  rx_data,   // Data received
    output reg         sclk,
    output reg         mosi,
    input  wire        miso,      // Master In Slave Out
    output reg         cs,
    output reg         busy
);

    parameter DIV_MAX = 50; 
    reg [6:0] clk_cnt;
    reg       spi_tick;

    reg [2:0] state;
    reg [3:0] bit_cnt;
    reg [7:0] shift_reg_tx;
    reg [7:0] shift_reg_rx;

    localparam IDLE  = 3'b000, 
               SETUP = 3'b001, 
               LOW   = 3'b010, 
               HIGH  = 3'b011,
               DONE  = 3'b100;

    // Clock Divider Logic
    always @(posedge clk ) begin
        if (reset) begin
            clk_cnt <= 0;
            spi_tick <= 0;
        end else if (state != IDLE) begin
            if (clk_cnt == DIV_MAX - 1) begin
                clk_cnt <= 0;
                spi_tick <= 1;
            end else begin
                clk_cnt <= clk_cnt + 1;
                spi_tick <= 0;
            end
        end else begin
            clk_cnt <= 0;
            spi_tick <= 0;
        end
    end

    // SPI State Machine
    always @(posedge clk) begin
        if (reset) begin
            state <= IDLE;
            sclk <= 0;
            mosi <= 0;
            cs   <= 1;
            busy <= 0;
            rx_data <= 8'b0;
        end else begin
            case (state)
                IDLE: begin
                    busy <= 0;
                    cs   <= 1;
                    sclk <= 0;
                    if (start) begin
                        shift_reg_tx <= tx_data;
                        busy <= 1;
                        state <= SETUP;
                    end
                end

                SETUP: begin
                    cs <= 0;
                    bit_cnt <= 7;
                    // Pre-drive the MSB onto MOSI
                    mosi <= shift_reg_tx[7]; 
                    if (spi_tick) state <= LOW;
                end

                LOW: begin
                    sclk <= 0;
                    mosi <= shift_reg_tx[bit_cnt];
                    if (spi_tick) begin
                        sclk  <= 1;
                        state <= HIGH;
                    end
                end

                HIGH: begin
                    if (spi_tick) begin
                         sclk <= 0;
                        // Sample MISO at the end of the High period
                        shift_reg_rx[bit_cnt] <= miso;
                        if (bit_cnt == 0) begin
                            state <= DONE;
                        end else begin
                            bit_cnt <= bit_cnt - 1;
                            state <= LOW;
                        end
                    end
                end

                DONE: begin
                    // Final tick to ensure CS stays low after the last bit
                    if (spi_tick) begin
                        rx_data <= shift_reg_rx;
                        cs <= 1;
                        state <= IDLE;
                    end
                end
                
                default: state <= IDLE;
            endcase
        end
    end
endmodule