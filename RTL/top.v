/*Top module of KRV-32 SOC V1.0
-Debug Log:
(2/5/2025): Tested with counter and working
(2/5/2025): UART TX and RX working
13/5/2025: PWM IP integrated(4 channel), 500Hz freq, duty cycle 0-255
14/5/2025: PWM IP working
*/
module top (
    input clk,
    input reset_n,
    output  [7:0] LEDS,
    output uart_tx,
    input uart_rx,
    output pwm0, pwm1, pwm2, pwm3
    
);
  
   wire divclk;
   wire mem_rstrb;
   wire mem_ready;
   wire [31:0] mem_addr;
   wire [31:0] mem_wdata;
   wire [3:0] mem_wstrb;
   wire [31:0] mem_rdata, rdata_gpio, rdata_uart_tx, rdata_uart_rx, pwm_data;

    wire ismem      = (mem_addr[31:28]==4'h0);
    wire isgpio     = (mem_addr[31:28]==4'h1);
    wire isuart_tx  = (mem_addr[31:28]==4'h2);
    wire isuart_rx  = (mem_addr[31:28]==4'h3);
    wire ispwm      = (mem_addr[31:28]==4'h4);

    wire [31:0] processor_rdata = ismem? mem_rdata: 
                                isgpio? rdata_gpio:
                                isuart_tx? rdata_uart_tx: 
                                isuart_rx? rdata_uart_rx:32'h0;
//Program memory
progmem mem0(
      .clk(divclk),.rst(reset_n),
      .addr(mem_addr),
      .data_out(mem_rdata),
      .rd_strobe(ismem & mem_rstrb),
      .data_in(mem_wdata),
      .wr_strobe({4{ismem}} & mem_wstrb)
  );

//KRV32 CPU
cpu KRV32(
.rst(reset_n),
.clk(divclk),
.mem_addr(mem_addr),
.mem_wdata(mem_wdata),
.mem_wstrb(mem_wstrb),
.mem_rdata(processor_rdata),
.mem_rstrb(mem_rstrb)

);


//LED gpio IP
   gpio_ip gpio_unit(
    // System
    .clk(divclk),
    .rst(!reset_n),
    // GPIO_0.DATA
    .csr_gpio_0_data_out(LEDS),

    // Local Bus
    .waddr({4'h0, mem_addr[27:0]}),
    .wdata(mem_wdata),
    .wen(isgpio & (|mem_wstrb)),
    .wstrb({4{isgpio}} & mem_wstrb),
    .wready(),
    .raddr({4'h0, mem_addr[27:0]}),
    .ren(isgpio & mem_rstrb),
    .rdata(rdata_gpio),
    .rvalid()
    );

//UART TX IP
   uart_ip uart_unit(

    // System
    .clk(divclk),
    .rst(!reset_n),
    // Local Bus
    .waddr({4'h0, mem_addr[27:0]}),
    .wdata(mem_wdata),
    .wen(isuart_tx & (|mem_wstrb)),
    .wstrb(mem_wstrb),
    .wready(),
    .raddr({4'h0, mem_addr[27:0]}),
    .ren(isuart_tx & mem_rstrb),
    .rdata(rdata_uart_tx),
    .rvalid(),
    // uart tx
    .o_uart_tx(uart_tx)
    );

//UART RX IP
uart_ip_rx uart_rx_unit(
    // System
    .clk(divclk),
    .rst(!reset_n),
    // Local Bus
    .waddr({4'h0, mem_addr[27:0]}),
    .wdata(mem_wdata),
    .wen(isuart_rx & (|mem_wstrb)),
    .wstrb(mem_wstrb),
    .wready(),
    .raddr({4'h0, mem_addr[27:0]}),
    .ren(isuart_rx & mem_rstrb),
    .rdata(rdata_uart_rx),
    .rvalid(),
    // uart rx
    .i_uart_rx(uart_rx)
    );


//PWM unit
    pwm_ip pwm_unit(
// System
    .clk(divclk),
    .rst(!reset_n),
    // Local Bus
    .waddr({4'h0, mem_addr[27:0]}),
    .wdata(mem_wdata),
    .wen(ispwm & (|mem_wstrb)),
    .wstrb(mem_wstrb),
    .wready(),
    .raddr({4'h0, mem_addr[27:0]}),
    .ren(ispwm & mem_rstrb),
    .rdata(pwm_data),
    .rvalid(),
    // pwm channel output
    .pwm_ch0(pwm0),
    .pwm_ch1(pwm1),
    .pwm_ch2(pwm2),
    .pwm_ch3(pwm3)
);

assign divclk = clk;
endmodule
