module spi_master_divided #(
    parameter CLK_DIV = 50 // Divide system clock by 50 (e.g., 100MHz -> 2MHz SPI)
)(
    input  wire       clk,      
    input  wire       reset,    
    input  wire       start,    
    input  wire [7:0] tx_data,  
    output reg  [7:0] rx_data,  
    output reg        busy,     
    
    // SPI Physical Interface
    output reg        sclk,
    output reg        mosi,
    input  wire       miso
);

    // --- Clock Divider / Enable Logic ---
    reg [15:0] clk_cnt;
    wire tick; // High for one system clock cycle every CLK_DIV cycles

    assign tick = (clk_cnt == CLK_DIV - 1);

    always @(posedge clk ) begin
        if (reset) 
            clk_cnt <= 0;
        else if (busy)
            clk_cnt <= tick ? 0 : clk_cnt + 1;
        else
            clk_cnt <= 0;
    end

    // --- FSM States ---
    parameter   IDLE    = 3'b000,
                SETUP   = 3'b001, 
                SAMPLE  = 3'b010, 
                HOLD    = 3'b011, // Added for symmetric clock timing
                DONE    = 3'b100;
    

    reg [2:0] state;
    reg [3:0] bit_cnt;

    always @(posedge clk ) begin
        if (reset) begin
            state    <= IDLE;
            sclk     <= 0;
            mosi     <= 0;
            busy     <= 0;
            rx_data  <= 0;
        end else begin
            case (state)
                IDLE: begin
                    busy <= 0;
                    sclk <= 0;
                    if (start) begin
                        bit_cnt <= 7;
                        busy    <= 1;
                        state   <= SETUP;
                    end
                end

                SETUP: begin
                    if (tick) begin
                        mosi  <= tx_data[bit_cnt]; // 1. Setup MOSI
                        state <= SAMPLE;
                    end
                end

                SAMPLE: begin
                    if (tick) begin
                        sclk  <= 1;                // 2. Rising Edge
                        rx_data[bit_cnt] <= miso;  // 3. Sample MISO
                        state <= HOLD;
                    end
                end

                HOLD: begin
                    if (tick) begin
                        sclk <= 0;                 // 4. Falling Edge
                        if (bit_cnt == 0)
                            state <= DONE;
                        else begin
                            bit_cnt <= bit_cnt - 1;
                            state   <= SETUP;
                        end
                    end
                end
/*
                DONE: begin
                    if (tick) begin
                        busy  <= 0;
                        state <= IDLE;
                    end
                end
                    */
                DONE: begin
                    busy <= 1; // Stay busy
                    if (!start) begin // Only return to IDLE once CPU drops the start signal
                        state <= IDLE;
                    end
                end
            endcase
        end
    end
endmodule