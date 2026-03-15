module spi_riscv_wrapper (
    input  wire        clk,
    input  wire        reset,
    input  wire [31:0] addr,
    input  wire [31:0] data_in,
    output reg  [31:0] data_out,
    input  wire        rd_strobe,
    input  wire [3:0]  wr_strobe,
    output wire        sclk,
    output wire        mosi,
    input  wire        miso,
    output reg         cs_pin  // Manually controlled CS
);

    wire base_hit = (addr & 32'hFFFFFFF0) == 32'hC1000000;
    reg  start_reg;
    wire [7:0] rx_data_wire;
    wire busy_wire;
    wire internal_cs; // We ignore the core's auto-CS now

    spi_master_full spi_core (
        .clk(clk), .reset(reset),
        .tx_data(data_in[7:0]), .start(start_reg),
        .rx_data(rx_data_wire), .sclk(sclk),
        .mosi(mosi), .miso(miso),
        .cs(internal_cs), .busy(busy_wire)
    );

    always @(posedge clk) begin
        if (reset) begin
            start_reg <= 0;
            cs_pin    <= 1; // Idle High
        end else begin
            start_reg <= 0;
            if (base_hit && wr_strobe != 0) begin
                case (addr[3:0])
                    4'h4: if (data_in[0]) start_reg <= 1;
                    4'h8: cs_pin <= data_in[0]; // Manually set CS
                endcase
            end
        end
    end

    always @(posedge clk) begin
       if(reset) 
	data_out <= 32'b0;
else begin
        if (base_hit && rd_strobe) begin
            case (addr[3:0])
                4'h0: data_out <= {24'b0, rx_data_wire};
                4'h4: data_out <= {31'b0, busy_wire};
                4'h8: data_out <= {31'b0, cs_pin};
                default: data_out <= 32'b0;
            endcase
        end
    end
end
endmodule