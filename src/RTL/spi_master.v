module spi_master_universal (
    input  wire        clk,      // System Clock (e.g., 100MHz)
    input  wire        reset,    
    input  wire        start,    
    input  wire [7:0]  tx_data,  
    output reg  [7:0]  rx_data,  
    output reg         busy,     
    
    // Configuration Inputs (Accessible via C/Registers)
    input  wire [15:0] clk_div,  // Clock Divider value
    input  wire        cpol,     // 0: Idle Low, 1: Idle High
    input  wire        cpha,     // 0: Sample on 1st edge, 1: Sample on 2nd edge
    
    // SPI Physical Interface
    output reg         sclk,
    output reg         mosi,
    input  wire        miso
);

    // --- Dynamic Clock Divider ---
    reg [15:0] clk_cnt;
    wire tick = (clk_cnt >= clk_div);

    always @(posedge clk) begin
        if (reset || !busy) 
            clk_cnt <= 0;
        else 
            clk_cnt <= tick ? 0 : clk_cnt + 1;
    end

    // --- FSM ---
    reg[2:0] state;
    parameter   IDLE   = 3'b000,
                LEAD   = 3'b001, // First edge of SCLK
                TRAIL  = 3'b010, // Second edge of SCLK
                DONE   = 3'b011;
    

    
    reg [3:0] bit_cnt;
    reg       mid_tick; // Used to handle CPHA setup timing

    always @(posedge clk) begin
        if (reset) begin
            state   <= IDLE;
            sclk    <= 0;
            mosi    <= 0;
            busy    <= 0;
            rx_data <= 0;
        end else begin
            case (state)
                IDLE: begin
                    busy <= 0;
                    sclk <= cpol; // Set idle polarity
                    if (start) begin
                        busy    <= 1;
                        bit_cnt <= 7;
                        // For CPHA=0, MOSI must be ready BEFORE the first edge
                        if (!cpha) mosi <= tx_data[7]; 
                        state   <= LEAD;
                    end
                end

                LEAD: begin
                    if (tick) begin
                        sclk <= !cpol; // Flip to active level
                        // CPHA=0: Sample on Lead | CPHA=1: Setup on Lead
                        if (!cpha) rx_data[bit_cnt] <= miso;
                        else       mosi <= tx_data[bit_cnt];
                        state <= TRAIL;
                    end
                end

                TRAIL: begin
                    if (tick) begin
                        sclk <= cpol; // Return to idle level
                        // CPHA=0: Setup next bit | CPHA=1: Sample
                        if (!cpha) begin
                            if (bit_cnt > 0) begin
                                bit_cnt <= bit_cnt - 1;
                                mosi    <= tx_data[bit_cnt-1];
                                state   <= LEAD;
                            end else state <= DONE;
                        end else begin
                            rx_data[bit_cnt] <= miso;
                            if (bit_cnt > 0) begin
                                bit_cnt <= bit_cnt - 1;
                                state   <= LEAD;
                            end else state <= DONE;
                        end
                    end
                end

                DONE: begin
                    if (!start) state <= IDLE;
                end
            endcase
        end
    end
endmodule