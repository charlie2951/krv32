`timescale 1ns / 1ps
//wrapper for I2c1
module i2c_board_mmio
(
    input  wire        clk,
    input  wire        rst_n,

    // ==============================
    // RISC-V MMIO Interface
    // ==============================
    input  wire [31:0] addr,
    input  wire [31:0] din,
    output reg  [31:0] dout,
    input  wire        rd_strb,
    input  wire [3:0]  wr_strb,

    // ==============================
    // I2C Lines
    // ==============================
    inout  wire        i2c_sda,
    inout  wire        i2c_scl
);

////////////////////////////////////////////////////////////
// Base Address
////////////////////////////////////////////////////////////

localparam BASE_ADDR = 32'hC0000200;

wire sel = (addr[31:8] == BASE_ADDR[31:8]);

wire [7:0] reg_offset = addr[7:0];

////////////////////////////////////////////////////////////
// Registers
////////////////////////////////////////////////////////////

reg        enable_reg;
reg        read_write_reg;
reg [6:0]  device_addr_reg;
reg [7:0]  register_addr_reg;
reg [7:0]  mosi_data_reg;
reg [15:0] divider_reg;

wire [7:0] miso_data_wire;
wire       busy_wire;

////////////////////////////////////////////////////////////
// Enable pulse generation
////////////////////////////////////////////////////////////

reg enable_pulse;

always @(posedge clk or negedge rst_n) begin
    if (!rst_n)
        enable_pulse <= 0;
    else
        enable_pulse <= (enable_reg);   // 1-cycle pulse
end

////////////////////////////////////////////////////////////
// Write Logic
////////////////////////////////////////////////////////////

always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        enable_reg        <= 0;
        read_write_reg    <= 0;
        device_addr_reg   <= 0;
        register_addr_reg <= 0;
        mosi_data_reg     <= 0;
        divider_reg       <= 100;
    end
    else begin
        enable_reg <= 0; // self clear

        if (sel && |wr_strb) begin
            case (reg_offset)

                8'h0: begin
                    if (wr_strb[0]) begin
                        enable_reg     <= din[0];
                        read_write_reg <= din[1];
                    end
                end

                8'h4: begin
                    if (wr_strb[0])
                        device_addr_reg <= din[6:0];
                end

                8'h8: begin
                    if (wr_strb[0])
                        register_addr_reg <= din[7:0];
                end

                8'hC: begin
                    if (wr_strb[0])
                        mosi_data_reg <= din[7:0];
                end

                8'h14: begin
                    if (wr_strb[1:0])
                        divider_reg <= din[15:0];
                end

            endcase
        end
    end
end

////////////////////////////////////////////////////////////
// Read Logic
////////////////////////////////////////////////////////////

always @(posedge clk) begin
    dout = 32'h0;

    if (sel && rd_strb) begin
        case (reg_offset)

            8'h0:  dout = {30'd0, read_write_reg, enable_reg};
            8'h4:  dout = {25'd0, device_addr_reg};
            8'h8:  dout = {24'd0, register_addr_reg};
            8'hC:  dout = {24'd0, mosi_data_reg};
            8'h10: dout = {24'd0, miso_data_wire};
            8'h14: dout = {16'd0, divider_reg};
            8'h18: dout = {31'd0, busy_wire};

        endcase
    end
end

////////////////////////////////////////////////////////////
// I2C Master Instance
////////////////////////////////////////////////////////////

i2c_master #(
    .NUMBER_OF_DATA_BYTES(1),
    .NUMBER_OF_REGISTER_BYTES(1),
    .ADDRESS_WIDTH(7),
    .CHECK_FOR_CLOCK_STRETCHING(1),
    .CLOCK_STRETCHING_MAX_COUNT('hFF)
)
i2c_master_inst
(
    .clock(clk),
    .reset_n(rst_n),
    .enable(enable_pulse),
    .read_write(read_write_reg),
    .mosi_data(mosi_data_reg),
    .register_address(register_addr_reg),
    .device_address(device_addr_reg),
    .divider(divider_reg),

    .miso_data(miso_data_wire),
    .busy(busy_wire),

    .external_serial_data(i2c_sda),
    .external_serial_clock(i2c_scl)
);

endmodule