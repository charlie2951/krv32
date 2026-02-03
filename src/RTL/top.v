module top(
    input rst, clk,
    input boot_en,
    input uart0_rx,
    output uart0_tx,
    input uart1_rx,
    output uart1_tx,
    output [7:0] leds
  );
  wire [31:0] mem_rdata, mem_wdata, addr;
  wire rstrb;
  wire [3:0] wr_strobe;
  wire [31:0] led_rdata,uart0_data, uart1_data,crypto_data;
  wire [31:0] boot_rdata;
//select device
  wire isMEM = (addr[31:16]==16'h0000);
  wire isLED = (addr[31:16]==16'h1000);
  wire isBOOT =(addr[31:16]==16'hA000);
 //UART0 and UART-1 data read back by cpu
  wire isUART0 = (addr[31:16]==16'h2000);
   wire isUART1 = (addr[31:16]==16'h3000);
  //Crypto data out read back by cpu
  wire isenc_valid_out= (addr[31:24]==8'h05);
  wire isenc_data_out= (addr[31:24]==8'h06);
  wire isdec_valid_out= (addr[31:24]==8'h0b);
  wire isdec_data_out= (addr[31:24]==8'h0c);
  wire iscrypto = isenc_valid_out|isenc_data_out|isdec_valid_out|isdec_data_out;

//Selecting input data to CPU from memory or peripheral devices based on address
 wire [31:0] cpu_rdata = isMEM ? mem_rdata:
                         isBOOT ? boot_rdata:
                        isLED ? led_rdata:
                        isUART0 ? uart0_data:
                        isUART1 ? uart1_data:
                        iscrypto?crypto_data:32'h0;


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

// Mapping LED GPIO
led_gpio led0(.rst(!rst), .clk(clk),
                .addr(addr),
                .data_in(mem_wdata),
                .rd_strobe(rstrb & isLED),
                .wr_strobe(wr_strobe & {4{isLED}}),
                .data_out(led_rdata),
                .leds(leds)
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


endmodule