module riscv_spi_wrapper (
    input  wire        clk,
    input  wire        reset,
    
    // CPU Bus Interface
    input  wire [31:0] addr,      // 32-bit Address
    input  wire [31:0] data_in,   // Data from CPU to Peripheral
    output reg  [31:0] data_out,  // Data from Peripheral to CPU
    input  wire        rd_strobe, // Read Enable
    input  wire [3:0]  wr_strobe, // Write Enable (byte-wise)
    
    // SPI Physical Pins
    output reg         spi_cs_n,  // Active Low Chip Select
    output wire        sclk,
    output wire        mosi,
    input  wire        miso
);

    // Internal Signals
    wire [7:0] rx_byte;
    reg  [7:0] tx_byte;
    reg        spi_start;
    wire       spi_busy;

    // Address Decoding
    wire base_sel    = (addr[31:4] == 28'hC100000); // Match 0xC100000X
    wire ctrl_reg_sel = base_sel && (addr[3:0] == 4'h0); // 0xC1000000
    wire data_reg_sel = base_sel && (addr[3:0] == 4'h4); // 0xC1000004

    // --- Bus Write Logic ---
    always @(posedge clk ) begin
        if (reset) begin
            spi_start <= 0;
            spi_cs_n  <= 1; // Default idle (High)
            tx_byte   <= 8'h00;
        end else begin
            spi_start <= 0; // Pulse logic: only high for one cycle
            
            if (|wr_strobe) begin
                if (ctrl_reg_sel) begin
                    spi_start <= data_in[0]; // Write bit 0 to start
                    spi_cs_n  <= data_in[2]; // Write bit 2 to control CS
                end
                if (data_reg_sel) begin
                    tx_byte   <= data_in[7:0];
                end
            end
        end
    end

    // --- Bus Read Logic ---
    always @(posedge clk) begin
        if (rd_strobe) begin
            if (ctrl_reg_sel)
                data_out = {29'b0, spi_cs_n, spi_busy, 1'b0};
            else if (data_reg_sel)
                data_out = {24'b0, rx_byte};
            else
                data_out = 32'h0;
        end else begin
            data_out = 32'h0;
        end
    end

    // --- Instantiate the SPI Controller ---
    spi_master_divided #(.CLK_DIV(50)) spi_core (
        .clk(clk),
        .reset(reset),
        .start(spi_start),
        .tx_data(tx_byte),
        .rx_data(rx_byte),
        .busy(spi_busy),
        .sclk(sclk),
        .mosi(mosi),
        .miso(miso)
    );

endmodule