//PWM IP that includes PWM generator along with configuration registers
// Mapping done by Corsair
// 4 channel PWM is used

module pwm_ip#(
    parameter ADDR_W = 32,
    parameter DATA_W = 32,
    parameter STRB_W = DATA_W / 8
)(
		input clk,rst,
       		// Local Bus
    		input  [ADDR_W-1:0] waddr,
    		input  [DATA_W-1:0] wdata,
    		input               wen,
    		input  [STRB_W-1:0] wstrb,
    		output              wready,
    		input  [ADDR_W-1:0] raddr,
    		input               ren,
    		output [DATA_W-1:0] rdata,
    		output              rvalid,
            output pwm_ch0, pwm_ch1, pwm_ch2, pwm_ch3
);


// local wires

wire [3:0] enable;
wire [7:0] dc0, dc1, dc2, dc3;
//wire pwm_ch0, pwm_ch1, pwm_ch2, pwm_ch3;


//Instantiate sub modules
regs_pwm pwm_config_reg(
.clk(clk),
 .rst(rst),
    // CHANNEL_SEL.CH0
 .csr_channel_sel0_ch0_out(enable[0]),
    // CHANNEL_SEL.CH1
  .csr_channel_sel1_ch1_out(enable[1]),
    // CHANNEL_SEL.CH2
   .csr_channel_sel2_ch2_out(enable[2]),
    // CHANNEL_SEL.CH3
   .csr_channel_sel3_ch3_out(enable[3]),

    // DUTY_CYCLE.CH0
    .csr_duty_cycle0_ch0_out(dc0),
    // DUTY_CYCLE.CH1
    .csr_duty_cycle1_ch1_out(dc1),
    // DUTY_CYCLE.CH2
    .csr_duty_cycle2_ch2_out(dc2),
    // DUTY_CYCLE.CH3
    .csr_duty_cycle3_ch3_out(dc3),

    // PWM_OUT.PWM0
    .csr_pwm_out0_pwm0_in(pwm_ch0),
    // PWM_OUT.PWM1
    .csr_pwm_out1_pwm1_in(pwm_ch1),
    // PWM_OUT.PWM2
    .csr_pwm_out2_pwm2_in(pwm_ch2),
    // PWM_OUT.PWM3
    .csr_pwm_out3_pwm3_in(pwm_ch3),

     // Local Bus
    .waddr(waddr),
        .wdata(wdata),
        .wen(wen),
        .wstrb(wstrb),
        .wready(wready),
        .raddr(raddr),
        .ren(ren),
        .rdata(rdata),
        .rvalid(rvalid)


);

    //PWM hardware
    pwm_4_channel pwm0(
    .clk(clk), .rst_n(!rst),
    .enable(enable),
    .duty_cycle_ch1(dc0),
    .duty_cycle_ch2(dc1),
    .duty_cycle_ch3(dc2),
    .duty_cycle_ch4(dc3),
    .pwm_out_ch1(pwm_ch0),
    .pwm_out_ch2(pwm_ch1),
    .pwm_out_ch3(pwm_ch2),
    .pwm_out_ch4(pwm_ch3)

    );
endmodule