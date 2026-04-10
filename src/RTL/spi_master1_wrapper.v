module riscv_spi1_wrapper (
    input  wire        clk,
    input  wire        reset,
    
    // CPU Bus Interface
    input  wire [31:0] addr,      
    input  wire [31:0] data_in,   
    output reg  [31:0] data_out,  
    input  wire        rd_strobe, 
    input  wire [3:0]  wr_strobe, 
    
    // SPI Physical Pins
    output reg         spi_cs_n,  
    output wire        sclk,
    output wire        mosi,
    input  wire        miso
);

    // --- Internal Registers ---
    reg [7:0]  tx_byte;
    wire [7:0] rx_byte;
    reg        spi_start;
    wire       spi_busy;
    
    // Configuration Registers (New)
    reg [15:0] r_clk_div;
    reg        r_cpol;
    reg        r_cpha;

    // --- Address Decoding ---
    // Base: 0xC100000X
    wire base_sel     = (addr[31:4] == 28'hC300000); 
    wire ctrl_reg_sel = base_sel && (addr[3:0] == 4'h0); // Control/Status
    wire data_reg_sel = base_sel && (addr[3:0] == 4'h4); // TX/RX Data
    wire cfg_reg_sel  = base_sel && (addr[3:0] == 4'h8); // Mode & Divider (New)

    // --- Bus Write Logic ---
    always @(posedge clk) begin
        if (reset) begin
            spi_start <= 0;
            spi_cs_n  <= 1;
            tx_byte   <= 8'h00;
            r_clk_div <= 16'd50; // Default fallback
            r_cpol    <= 1'b0;
            r_cpha    <= 1'b0;
        end else begin
            spi_start <= 0; // Auto-clear start pulse
            
            if (|wr_strobe) begin
                if (ctrl_reg_sel) begin
                    spi_start <= data_in[0]; 
                    spi_cs_n  <= data_in[2]; 
                end
                if (data_reg_sel) begin
                    tx_byte   <= data_in[7:0];
                end
                if (cfg_reg_sel) begin
                    r_clk_div <= data_in[15:0];  // Lower 16 bits: Divider
                    r_cpol    <= data_in[16];    // Bit 16: CPOL
                    r_cpha    <= data_in[17];    // Bit 17: CPHA
                end
            end
        end
    end

    // --- Bus Read Logic ---
    always @(posedge clk) begin
        if (rd_strobe) begin
            case (1'b1)
                ctrl_reg_sel: data_out = {29'b0, spi_cs_n, spi_busy, 1'b0};
                data_reg_sel: data_out = {24'b0, rx_byte};
                cfg_reg_sel:  data_out = {14'b0, r_cpha, r_cpol, r_clk_div};
                default:      data_out = 32'h0;
            endcase
        end else begin
            data_out = 32'h0;
        end
    end

    // --- Instantiate Universal SPI Master ---
    spi_master_universal spi_core (
        .clk(clk),
        .reset(reset),
        .start(spi_start),
        .tx_data(tx_byte),
        .rx_data(rx_byte),
        .busy(spi_busy),
        .clk_div(r_clk_div),
        .cpol(r_cpol),
        .cpha(r_cpha),
        .sclk(sclk),
        .mosi(mosi),
        .miso(miso)
    );

endmodule