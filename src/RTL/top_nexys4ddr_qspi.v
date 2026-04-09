module top(
    input rst, clk, //control signals
    input boot_en, //boot enable pin
    input uart0_rx, //uart0-rx
    output uart0_tx, //uart0-tx
    input uart1_rx,//uart1-rx
    output uart1_tx, //uart1-tx
    inout [15:0] gpio, //16 bit gpio
    inout i2c0_sda,//pins for i2c0(SDA)
    inout i2c0_scl,//pins for i2c0(SCL)
    inout i2c1_sda,//pins for i2c1(SDA)
    inout i2c1_scl, //pins for i21(SCL)
    output [7:0] an,//7seg disp select
    output [7:0] seg, //segment select
    //SPI master ports
    output wire SS,
    output wire SCK,
    output wire MOSI,
    input  wire MISO,
    // QSPI Physical Pins (Excluding Clock, which is handled by STARTUPE2)
    output wire        qspi_cs_n,
    inout  wire [3:0]  qspi_io
    );

    // QSPI Internal Clock (Before the Primitive)
    wire qspi_clk_internal;

  //Bus interfac e control lines
  wire [31:0] mem_rdata, mem_wdata, addr,segment_data, SPI0_DATA, QSPI_DATA;
  wire rstrb;
  wire [3:0] wr_strobe;
  //peripheral data collect wires
  wire [31:0] timer0_rdata, timer1_rdata;
  wire [31:0] i2c0_rdata, i2c1_rdata;
  wire [31:0] uart0_data, uart1_data,crypto_data;
  wire [31:0] boot_rdata, gpio_rdata;
//************select peripheral device ****   //
  wire isMEM = (addr[31:16]==16'h0000); //program memory
  wire isGPIO = (addr[31:16]==16'h1000); //GPIO
  wire isTIMER0 = (addr[31:4]==28'hB000000); //timer0
  wire isTIMER1 = (addr[31:4]==28'hB000001); //timer1
  wire isI2C0 = (addr[31:8]==24'hC00000); //i2c master0
  wire isI2C1 = (addr[31:8]==24'hC00001); //i2c master1
  wire isBOOT =(addr[31:16]==16'hA000); //Bootloader
 //UART0 and UART-1 data read back by cpu
  wire isUART0 = (addr[31:16]==16'h2000); //UART0
  wire isUART1 = (addr[31:16]==16'h3000); //UART1
  //Crypto data out read back by cpu
  wire isenc_valid_out= (addr[31:24]==8'h05); //crypto
  wire isenc_data_out= (addr[31:24]==8'h06); //crypto
  wire isdec_valid_out= (addr[31:24]==8'h0b); //crypto
  wire isdec_data_out= (addr[31:24]==8'h0c); //crypto
  wire iscrypto = isenc_valid_out|isenc_data_out|isdec_valid_out|isdec_data_out;
 wire isSEG = (addr[31:16]==16'h1100);//seven seg disp
 wire isSPI0 = (addr[31:16]==16'hC100);//SPI-0 master
 wire isQSPI = (addr[31:16]==16'hC200);//QSPI master
//Selecting input data to CPU from memory or peripheral devices based on address
 wire [31:0] cpu_rdata = isMEM ? mem_rdata:
                         isBOOT ? boot_rdata:
                         isGPIO ? gpio_rdata:
                        isUART0 ? uart0_data:
                        isUART1 ? uart1_data:
			                  isTIMER0 ? timer0_rdata:
                        isTIMER1 ? timer1_rdata:
                        isI2C0 ? i2c0_rdata:
                        isI2C1 ? i2c1_rdata:
                        isSEG ? segment_data:
                        isSPI0 ? SPI0_DATA:
                        iscrypto?crypto_data:
                        isQSPI ? QSPI_DATA:32'h0;


//Instantiate sub modules
// Mapping CPU
  cpu cpu0(
        .rst(!rst), .clk(clk),
        .boot_en(boot_en),
        .mem_rdata(cpu_rdata),
        .mem_addr(addr),
        .cycle(),
        .mem_rstrb(rstrb),
        .mem_wdata(mem_wdata),
        .mem_wstrb(wr_strobe)
      );

//Mapping Program memory
  progmem mem0(
            .rst(!rst), .clk(clk),
            .addr(addr),
            .data_in(mem_wdata),
            .rd_strobe(rstrb & isMEM),
            .wr_strobe(wr_strobe & {4{isMEM}}),
            .data_out(mem_rdata)
          );

//mapping uart0 wrapper gpio regs
uart0_wrapper uart0_mem_map(.rst(!rst), .clk(clk),
                .addr(addr),
                .data_in(mem_wdata),
                .rd_strobe(rstrb ),
                .wr_strobe(wr_strobe),
                .data_out(uart0_data),
                .uart_tx(uart0_tx),
                .uart_rx(uart0_rx)
                );

//mapping uart1 wrapper gpio regs
uart1_wrapper uart1_mem_map(.rst(!rst), .clk(clk),
                .addr(addr),
                .data_in(mem_wdata),
                .rd_strobe(rstrb ),
                .wr_strobe(wr_strobe),
                .data_out(uart1_data),
                .uart_tx(uart1_tx),
                .uart_rx(uart1_rx)
                );
//Mapping with Crypto wrapper
CRYPTO_GPIO crypto_0(
.rst(rst), .clk(clk),
                .addr(addr),
                .data_in(mem_wdata),
                .rd_strobe(rstrb ),
                .wr_strobe(wr_strobe),
                .data_out(crypto_data)
                
);  

// Instantiate Boot loader memory
bootmem boot_0(
.clk(clk),
.rst(!rst),
.addr(addr),
.rd_strobe(rstrb & isBOOT),
.data_out(boot_rdata),
.data_in(mem_wdata),
.wr_strobe(wr_strobe & {4{isBOOT}})
);

//GPIO controller
gpio_controller gpio_0(
.clk(clk),
.rst_n(!rst),
.addr(addr),
.rd_strobe(rstrb & isGPIO),
.data_out(gpio_rdata),
.data_in(mem_wdata),
.wr_strobe(wr_strobe & {4{isGPIO}}),
.gpio_pins(gpio) //16 gpio
);

//timer0
timer0_mmio timer0(
.rst_n(rst), .clk(clk),
.addr(addr),
.data_in(mem_wdata),
.data_out(timer0_rdata),
.rd_strobe(rstrb),
.wr_strobe(wr_strobe)

);

//Timer1 mapping
//timer1
timer1_mmio timer1(
.rst_n(rst), .clk(clk),
.addr(addr),
.data_in(mem_wdata),
.data_out(timer1_rdata),
.rd_strobe(rstrb),
.wr_strobe(wr_strobe)

);

//I2C master-0 mmio wrapper
i2c_master0_mmio i2c0(
.clk(clk),
.rst_n(rst),
.addr(addr),
.din(mem_wdata),
.dout(i2c0_rdata),
.rd_strb(rstrb),
.wr_strb(wr_strobe),
.i2c_sda(i2c0_sda),
.i2c_scl(i2c0_scl)
);

//I2C master-1 mmio wrapper
i2c_master1_mmio i2c1(
.clk(clk),
.rst_n(rst),
.addr(addr),
.din(mem_wdata),
.dout(i2c1_rdata),
.rd_strb(rstrb),
.wr_strb(wr_strobe),
.i2c_sda(i2c1_sda),
.i2c_scl(i2c1_scl)
);
//7 seg display controller
seven_seg_mmio seg0 (
    .clk(clk),      
    .resetn(rst),   
    // MMIO Interface (Base: 0x11000000)
    .addr(addr),
    .data_in(mem_wdata),
    .data_out(segment_data),
    .rstrb(rstrb),
    .wstrb(wr_strobe),
    // Physical Pins
    .an(an[3:0]),       
    .seg(seg)       
);
//for nexys4ddr board there are 8 display
//disable 1st 4 display an[4] to an[7]
assign an[7:4]=4'b1111;

// SPI-0 Master w25q flash interface
riscv_spi_wrapper spi0(
.clk(clk),
.reset(!rst),
.addr(addr),
.data_in(mem_wdata),
.data_out(SPI0_DATA),
.rd_strobe(rstrb),
.wr_strobe(wr_strobe),
.spi_cs_n(SS),
.sclk(SCK),
.mosi(MOSI),
.miso(MISO)
);

//QSPI MASTER Wrapper instantiation
// Maps QSPI to 0xC2000000
    riscv_qspi_wrapper u_qspi_bus_interface (
        .clk           (clk),
        .rst_n         (rst),
        .cpu_addr      (addr),
        .cpu_data_in   (mem_wdata),
        .cpu_data_out  (QSPI_DATA),
        .cpu_rd_strobe (rstrb),
        .cpu_wr_strobe (wr_strobe),
              
        // Connection to Master Logic
        .qspi_cs_n     (qspi_cs_n),
        .qspi_clk      (qspi_clk_internal), // Internal wire
        .qspi_io       (qspi_io)
    );
//STARTUP2E primitive
STARTUPE2 #(
    .PROG_USR("FALSE"),   // Hardware: Always FALSE unless using specialized encryption
    .SIM_CCLK_FREQ(0.0)   // Hardware: Ignored (Set to 0.0)
) u_startup (
    .CFGCLK(),            // Leave Open
    .CFGMCLK(),           // Leave Open
    .EOS(),               // Leave Open
    .PREQ(),              // Leave Open
    .CLK(1'b0),           // Leave 0
    .GSR(1'b0),           // Global Set/Reset (Not needed for QSPI)
    .GTS(1'b0),           // Global Tri-state (Not needed for QSPI)
    .KEYCLEARB(1'b1),     // Internal logic tie-high
    .PACK(1'b1),          // Internal logic tie-high
    .USRCCLKO(qspi_clk_internal), // Input: Connect your QSPI Master Clock here
    .USRCCLKTS(1'b0),     // Input: 0 = Enable CCLK output, 1 = High-Z
    .USRDONEO(1'b1),      // Input: Drive high to maintain 'DONE' status
    .USRDONETS(1'b1)      // Input: 1 = Use internal DONE pull-up
);
endmodule