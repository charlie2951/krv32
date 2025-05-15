// Created with Corsair v1.0.4

module regs_pwm #(
    parameter ADDR_W = 32,
    parameter DATA_W = 32,
    parameter STRB_W = DATA_W / 8
)(
    // System
    input clk,
    input rst,
    // CHANNEL_SEL0.CH0
    output  csr_channel_sel0_ch0_out,

    // CHANNEL_SEL1.CH1
    output  csr_channel_sel1_ch1_out,

    // CHANNEL_SEL2.CH2
    output  csr_channel_sel2_ch2_out,

    // CHANNEL_SEL3.CH3
    output  csr_channel_sel3_ch3_out,

    // DUTY_CYCLE0.CH0
    output [7:0] csr_duty_cycle0_ch0_out,

    // DUTY_CYCLE1.CH1
    output [7:0] csr_duty_cycle1_ch1_out,

    // DUTY_CYCLE2.CH2
    output [7:0] csr_duty_cycle2_ch2_out,

    // DUTY_CYCLE3.CH3
    output [7:0] csr_duty_cycle3_ch3_out,

    // PWM_OUT0.PWM0
    input  csr_pwm_out0_pwm0_in,

    // PWM_OUT1.PWM1
    input  csr_pwm_out1_pwm1_in,

    // PWM_OUT2.PWM2
    input  csr_pwm_out2_pwm2_in,

    // PWM_OUT3.PWM3
    input  csr_pwm_out3_pwm3_in,

    // Local Bus
    input  [ADDR_W-1:0] waddr,
    input  [DATA_W-1:0] wdata,
    input               wen,
    input  [STRB_W-1:0] wstrb,
    output              wready,
    input  [ADDR_W-1:0] raddr,
    input               ren,
    output [DATA_W-1:0] rdata,
    output              rvalid
);
//------------------------------------------------------------------------------
// CSR:
// [0x0] - CHANNEL_SEL0 - PWM Channel select register-0
//------------------------------------------------------------------------------
wire [31:0] csr_channel_sel0_rdata;
assign csr_channel_sel0_rdata[31:1] = 31'h0;

wire csr_channel_sel0_wen;
assign csr_channel_sel0_wen = wen && (waddr == 32'h0);

//---------------------
// Bit field:
// CHANNEL_SEL0[0] - CH0 - PWM Channel-0 select
// access: wo, hardware: o
//---------------------
reg  csr_channel_sel0_ch0_ff;

assign csr_channel_sel0_rdata[0] = 1'b0;

assign csr_channel_sel0_ch0_out = csr_channel_sel0_ch0_ff;

always @(posedge clk) begin
    if (rst) begin
        csr_channel_sel0_ch0_ff <= 1'b0;
    end else  begin
     if (csr_channel_sel0_wen) begin
            if (wstrb[0]) begin
                csr_channel_sel0_ch0_ff <= wdata[0];
            end
        end else begin
            csr_channel_sel0_ch0_ff <= csr_channel_sel0_ch0_ff;
        end
    end
end


//------------------------------------------------------------------------------
// CSR:
// [0x4] - CHANNEL_SEL1 - PWM Channel select register-1
//------------------------------------------------------------------------------
wire [31:0] csr_channel_sel1_rdata;
assign csr_channel_sel1_rdata[31:1] = 31'h0;

wire csr_channel_sel1_wen;
assign csr_channel_sel1_wen = wen && (waddr == 32'h4);

//---------------------
// Bit field:
// CHANNEL_SEL1[0] - CH1 - PWM Channel-1 select
// access: wo, hardware: o
//---------------------
reg  csr_channel_sel1_ch1_ff;

assign csr_channel_sel1_rdata[0] = 1'b0;

assign csr_channel_sel1_ch1_out = csr_channel_sel1_ch1_ff;

always @(posedge clk) begin
    if (rst) begin
        csr_channel_sel1_ch1_ff <= 1'b0;
    end else  begin
     if (csr_channel_sel1_wen) begin
            if (wstrb[0]) begin
                csr_channel_sel1_ch1_ff <= wdata[0];
            end
        end else begin
            csr_channel_sel1_ch1_ff <= csr_channel_sel1_ch1_ff;
        end
    end
end


//------------------------------------------------------------------------------
// CSR:
// [0x8] - CHANNEL_SEL2 - PWM Channel select register-2
//------------------------------------------------------------------------------
wire [31:0] csr_channel_sel2_rdata;
assign csr_channel_sel2_rdata[31:1] = 31'h0;

wire csr_channel_sel2_wen;
assign csr_channel_sel2_wen = wen && (waddr == 32'h8);

//---------------------
// Bit field:
// CHANNEL_SEL2[0] - CH2 - PWM Channel-2 select
// access: wo, hardware: o
//---------------------
reg  csr_channel_sel2_ch2_ff;

assign csr_channel_sel2_rdata[0] = 1'b0;

assign csr_channel_sel2_ch2_out = csr_channel_sel2_ch2_ff;

always @(posedge clk) begin
    if (rst) begin
        csr_channel_sel2_ch2_ff <= 1'b0;
    end else  begin
     if (csr_channel_sel2_wen) begin
            if (wstrb[0]) begin
                csr_channel_sel2_ch2_ff <= wdata[0];
            end
        end else begin
            csr_channel_sel2_ch2_ff <= csr_channel_sel2_ch2_ff;
        end
    end
end


//------------------------------------------------------------------------------
// CSR:
// [0xc] - CHANNEL_SEL3 - PWM Channel select register-3
//------------------------------------------------------------------------------
wire [31:0] csr_channel_sel3_rdata;
assign csr_channel_sel3_rdata[31:1] = 31'h0;

wire csr_channel_sel3_wen;
assign csr_channel_sel3_wen = wen && (waddr == 32'hc);

//---------------------
// Bit field:
// CHANNEL_SEL3[0] - CH3 - PWM Channel-3 select
// access: wo, hardware: o
//---------------------
reg  csr_channel_sel3_ch3_ff;

assign csr_channel_sel3_rdata[0] = 1'b0;

assign csr_channel_sel3_ch3_out = csr_channel_sel3_ch3_ff;

always @(posedge clk) begin
    if (rst) begin
        csr_channel_sel3_ch3_ff <= 1'b0;
    end else  begin
     if (csr_channel_sel3_wen) begin
            if (wstrb[0]) begin
                csr_channel_sel3_ch3_ff <= wdata[0];
            end
        end else begin
            csr_channel_sel3_ch3_ff <= csr_channel_sel3_ch3_ff;
        end
    end
end


//------------------------------------------------------------------------------
// CSR:
// [0x10] - DUTY_CYCLE0 - Duty Cycle select Register-0
//------------------------------------------------------------------------------
wire [31:0] csr_duty_cycle0_rdata;
assign csr_duty_cycle0_rdata[31:8] = 24'h0;

wire csr_duty_cycle0_wen;
assign csr_duty_cycle0_wen = wen && (waddr == 32'h10);

//---------------------
// Bit field:
// DUTY_CYCLE0[7:0] - CH0 - CH0 Duty cycle 8 bit value 0-255
// access: wo, hardware: o
//---------------------
reg [7:0] csr_duty_cycle0_ch0_ff;

assign csr_duty_cycle0_rdata[7:0] = 8'h0;

assign csr_duty_cycle0_ch0_out = csr_duty_cycle0_ch0_ff;

always @(posedge clk) begin
    if (rst) begin
        csr_duty_cycle0_ch0_ff <= 8'h0;
    end else  begin
     if (csr_duty_cycle0_wen) begin
            if (wstrb[0]) begin
                csr_duty_cycle0_ch0_ff[7:0] <= wdata[7:0];
            end
        end else begin
            csr_duty_cycle0_ch0_ff <= csr_duty_cycle0_ch0_ff;
        end
    end
end


//------------------------------------------------------------------------------
// CSR:
// [0x14] - DUTY_CYCLE1 - Duty Cycle select Register-1
//------------------------------------------------------------------------------
wire [31:0] csr_duty_cycle1_rdata;
assign csr_duty_cycle1_rdata[31:8] = 24'h0;

wire csr_duty_cycle1_wen;
assign csr_duty_cycle1_wen = wen && (waddr == 32'h14);

//---------------------
// Bit field:
// DUTY_CYCLE1[7:0] - CH1 - CH1 Duty cycle 8 bit value 0-255
// access: wo, hardware: o
//---------------------
reg [7:0] csr_duty_cycle1_ch1_ff;

assign csr_duty_cycle1_rdata[7:0] = 8'h0;

assign csr_duty_cycle1_ch1_out = csr_duty_cycle1_ch1_ff;

always @(posedge clk) begin
    if (rst) begin
        csr_duty_cycle1_ch1_ff <= 8'h0;
    end else  begin
     if (csr_duty_cycle1_wen) begin
            if (wstrb[0]) begin
                csr_duty_cycle1_ch1_ff[7:0] <= wdata[7:0];
            end
        end else begin
            csr_duty_cycle1_ch1_ff <= csr_duty_cycle1_ch1_ff;
        end
    end
end


//------------------------------------------------------------------------------
// CSR:
// [0x18] - DUTY_CYCLE2 - Duty Cycle select Register-2
//------------------------------------------------------------------------------
wire [31:0] csr_duty_cycle2_rdata;
assign csr_duty_cycle2_rdata[31:8] = 24'h0;

wire csr_duty_cycle2_wen;
assign csr_duty_cycle2_wen = wen && (waddr == 32'h18);

//---------------------
// Bit field:
// DUTY_CYCLE2[7:0] - CH2 - CH2 Duty cycle 8 bit value 0-255
// access: wo, hardware: o
//---------------------
reg [7:0] csr_duty_cycle2_ch2_ff;

assign csr_duty_cycle2_rdata[7:0] = 8'h0;

assign csr_duty_cycle2_ch2_out = csr_duty_cycle2_ch2_ff;

always @(posedge clk) begin
    if (rst) begin
        csr_duty_cycle2_ch2_ff <= 8'h0;
    end else  begin
     if (csr_duty_cycle2_wen) begin
            if (wstrb[0]) begin
                csr_duty_cycle2_ch2_ff[7:0] <= wdata[7:0];
            end
        end else begin
            csr_duty_cycle2_ch2_ff <= csr_duty_cycle2_ch2_ff;
        end
    end
end


//------------------------------------------------------------------------------
// CSR:
// [0x1c] - DUTY_CYCLE3 - Duty Cycle select Register-3
//------------------------------------------------------------------------------
wire [31:0] csr_duty_cycle3_rdata;
assign csr_duty_cycle3_rdata[31:8] = 24'h0;

wire csr_duty_cycle3_wen;
assign csr_duty_cycle3_wen = wen && (waddr == 32'h1c);

//---------------------
// Bit field:
// DUTY_CYCLE3[7:0] - CH3 - CH3 Duty cycle 8 bit value 0-255
// access: wo, hardware: o
//---------------------
reg [7:0] csr_duty_cycle3_ch3_ff;

assign csr_duty_cycle3_rdata[7:0] = 8'h0;

assign csr_duty_cycle3_ch3_out = csr_duty_cycle3_ch3_ff;

always @(posedge clk) begin
    if (rst) begin
        csr_duty_cycle3_ch3_ff <= 8'h0;
    end else  begin
     if (csr_duty_cycle3_wen) begin
            if (wstrb[0]) begin
                csr_duty_cycle3_ch3_ff[7:0] <= wdata[7:0];
            end
        end else begin
            csr_duty_cycle3_ch3_ff <= csr_duty_cycle3_ch3_ff;
        end
    end
end


//------------------------------------------------------------------------------
// CSR:
// [0x20] - PWM_OUT0 - PWM Output register-0
//------------------------------------------------------------------------------
wire [31:0] csr_pwm_out0_rdata;
assign csr_pwm_out0_rdata[31:1] = 31'h0;


wire csr_pwm_out0_ren;
assign csr_pwm_out0_ren = ren && (raddr == 32'h20);
reg csr_pwm_out0_ren_ff;
always @(posedge clk) begin
    if (rst) begin
        csr_pwm_out0_ren_ff <= 1'b0;
    end else begin
        csr_pwm_out0_ren_ff <= csr_pwm_out0_ren;
    end
end
//---------------------
// Bit field:
// PWM_OUT0[0] - PWM0 - Channel-0 PWM output
// access: ro, hardware: i
//---------------------
reg  csr_pwm_out0_pwm0_ff;

assign csr_pwm_out0_rdata[0] = csr_pwm_out0_pwm0_ff;


always @(posedge clk) begin
    if (rst) begin
        csr_pwm_out0_pwm0_ff <= 1'b0;
    end else  begin
              begin            csr_pwm_out0_pwm0_ff <= csr_pwm_out0_pwm0_in;
        end
    end
end


//------------------------------------------------------------------------------
// CSR:
// [0x24] - PWM_OUT1 - PWM Output register-1
//------------------------------------------------------------------------------
wire [31:0] csr_pwm_out1_rdata;
assign csr_pwm_out1_rdata[31:1] = 31'h0;


wire csr_pwm_out1_ren;
assign csr_pwm_out1_ren = ren && (raddr == 32'h24);
reg csr_pwm_out1_ren_ff;
always @(posedge clk) begin
    if (rst) begin
        csr_pwm_out1_ren_ff <= 1'b0;
    end else begin
        csr_pwm_out1_ren_ff <= csr_pwm_out1_ren;
    end
end
//---------------------
// Bit field:
// PWM_OUT1[0] - PWM1 - Channel-1 PWM output
// access: ro, hardware: i
//---------------------
reg  csr_pwm_out1_pwm1_ff;

assign csr_pwm_out1_rdata[0] = csr_pwm_out1_pwm1_ff;


always @(posedge clk) begin
    if (rst) begin
        csr_pwm_out1_pwm1_ff <= 1'b0;
    end else  begin
              begin            csr_pwm_out1_pwm1_ff <= csr_pwm_out1_pwm1_in;
        end
    end
end


//------------------------------------------------------------------------------
// CSR:
// [0x28] - PWM_OUT2 - PWM Output register-2
//------------------------------------------------------------------------------
wire [31:0] csr_pwm_out2_rdata;
assign csr_pwm_out2_rdata[31:1] = 31'h0;


wire csr_pwm_out2_ren;
assign csr_pwm_out2_ren = ren && (raddr == 32'h28);
reg csr_pwm_out2_ren_ff;
always @(posedge clk) begin
    if (rst) begin
        csr_pwm_out2_ren_ff <= 1'b0;
    end else begin
        csr_pwm_out2_ren_ff <= csr_pwm_out2_ren;
    end
end
//---------------------
// Bit field:
// PWM_OUT2[0] - PWM2 - Channel-2 PWM output
// access: ro, hardware: i
//---------------------
reg  csr_pwm_out2_pwm2_ff;

assign csr_pwm_out2_rdata[0] = csr_pwm_out2_pwm2_ff;


always @(posedge clk) begin
    if (rst) begin
        csr_pwm_out2_pwm2_ff <= 1'b0;
    end else  begin
              begin            csr_pwm_out2_pwm2_ff <= csr_pwm_out2_pwm2_in;
        end
    end
end


//------------------------------------------------------------------------------
// CSR:
// [0x2c] - PWM_OUT3 - PWM Output register-3
//------------------------------------------------------------------------------
wire [31:0] csr_pwm_out3_rdata;
assign csr_pwm_out3_rdata[31:1] = 31'h0;


wire csr_pwm_out3_ren;
assign csr_pwm_out3_ren = ren && (raddr == 32'h2c);
reg csr_pwm_out3_ren_ff;
always @(posedge clk) begin
    if (rst) begin
        csr_pwm_out3_ren_ff <= 1'b0;
    end else begin
        csr_pwm_out3_ren_ff <= csr_pwm_out3_ren;
    end
end
//---------------------
// Bit field:
// PWM_OUT3[0] - PWM3 - Channel-3 PWM output
// access: ro, hardware: i
//---------------------
reg  csr_pwm_out3_pwm3_ff;

assign csr_pwm_out3_rdata[0] = csr_pwm_out3_pwm3_ff;


always @(posedge clk) begin
    if (rst) begin
        csr_pwm_out3_pwm3_ff <= 1'b0;
    end else  begin
              begin            csr_pwm_out3_pwm3_ff <= csr_pwm_out3_pwm3_in;
        end
    end
end


//------------------------------------------------------------------------------
// Write ready
//------------------------------------------------------------------------------
assign wready = 1'b1;

//------------------------------------------------------------------------------
// Read address decoder
//------------------------------------------------------------------------------
reg [31:0] rdata_ff;
always @(posedge clk) begin
    if (rst) begin
        rdata_ff <= 32'h0;
    end else if (ren) begin
        case (raddr)
            32'h0: rdata_ff <= csr_channel_sel0_rdata;
            32'h4: rdata_ff <= csr_channel_sel1_rdata;
            32'h8: rdata_ff <= csr_channel_sel2_rdata;
            32'hc: rdata_ff <= csr_channel_sel3_rdata;
            32'h10: rdata_ff <= csr_duty_cycle0_rdata;
            32'h14: rdata_ff <= csr_duty_cycle1_rdata;
            32'h18: rdata_ff <= csr_duty_cycle2_rdata;
            32'h1c: rdata_ff <= csr_duty_cycle3_rdata;
            32'h20: rdata_ff <= csr_pwm_out0_rdata;
            32'h24: rdata_ff <= csr_pwm_out1_rdata;
            32'h28: rdata_ff <= csr_pwm_out2_rdata;
            32'h2c: rdata_ff <= csr_pwm_out3_rdata;
            default: rdata_ff <= 32'h0;
        endcase
    end else begin
        rdata_ff <= 32'h0;
    end
end
assign rdata = rdata_ff;

//------------------------------------------------------------------------------
// Read data valid
//------------------------------------------------------------------------------
reg rvalid_ff;
always @(posedge clk) begin
    if (rst) begin
        rvalid_ff <= 1'b0;
    end else if (ren && rvalid) begin
        rvalid_ff <= 1'b0;
    end else if (ren) begin
        rvalid_ff <= 1'b1;
    end
end

assign rvalid = rvalid_ff;

endmodule