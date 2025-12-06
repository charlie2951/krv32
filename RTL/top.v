//*** Top module contain UART, LED gpio and Program Memory ***//
module top(
    input rst, clk, boot,
    input uart_rx,
    output uart_tx,
    output [5:0] leds
  );
  wire [31:0] mem_rdata,led_rdata,boot_rdata, mem_wdata, addr;
  wire rstrb;
  wire [3:0] wr_strobe;
  wire uart_status;
  wire [31:0] uart_data;
  wire [5:0] tmp_led;
//select device
  wire isMEM = (addr[31:28]==4'b0000);//0x0000_0000
  wire isLED = (addr[31:28]==4'b0001);//0x1000_0000
  wire isUART = (addr[31:28]==4'b0010);//0x2000_0000
 // wire isUART_RXDATA= (addr[19:16]==4'b0101);//0x0005_0000
 // wire isUART_TXSTATUS= (addr[19:16]==4'b0100);//0x0004_0000
 // wire isUART_RXSTATUS= (addr[19:16]==4'b0111);//0x0007_0000
//  wire isboot = (addr[19:16]==4'b1000);//0x0008_0000
//Selecting input data to CPU from memory or peripheral devices based on address
  wire [31:0] cpu_rdata = isMEM ? mem_rdata:
                        isLED ? led_rdata:
                        isUART ? uart_data:32'h0;
                        //isboot ? boot_rdata:
                        //(isUART_RXSTATUS|isUART_RXDATA) ? uart_rx_data: 
                        // isUART_TXSTATUS ? uart_status: 32'h0;
                         
//Instantiate sub modules
// Mapping CPU
  KRV32 cpu0(
        .rst(!rst), .clk(clk),
        .boot(boot),
        .mem_rdata(cpu_rdata),
        .mem_addr(addr),
        .cycle(),
        .mem_rstrb(rstrb),
        .mem_wdata(mem_wdata),
        .mem_wstrb(wr_strobe)
      );

//Mapping Program memory
  progmem mem0(
            .rst(rst), .clk(clk),
            .addr(addr),
            .data_in(mem_wdata),
            .rd_strobe(rstrb & isMEM),
            .wr_strobe(wr_strobe & {4{isMEM}}),
            .data_out(mem_rdata)
          );


// Mapping LED GPIO
  led_gpio led0(
                .rst(rst), .clk(clk),
                .addr(addr),
                .data_in(mem_wdata),
                .rd_strobe(rstrb & isLED),
                .wr_strobe(wr_strobe & {4{isLED}}),
                .data_out(led_rdata),
                .leds(tmp_led)
                );

assign leds = ~tmp_led;

/*
//Mappring UART TX GPIO
 uart_gpio uart_tx0(.rst(!rst), .clk(clk),
                .addr(addr),
                .data_in(mem_wdata),
                .rd_strobe(rstrb ),
                .wr_strobe(wr_strobe),
                .data_out(uart_status),
                .uart_tx(uart_tx)
                );


// Mapping UART Receiver GPIO
 uart_rx_gpio uart_rx0(.rst(!rst), .clk(clk),
                .addr(addr),
                .data_in(mem_wdata),
                .rd_strobe(rstrb ),
                .wr_strobe(wr_strobe),
                .data_out(uart_rx_data),
                .uart_rx(uart_rx)
                );


*/

//Mapping UART MODULE, BAUD RATE=9600, CLK FERQ=27MHz for Tang9K board
 UART_GPIO 
#(.CLK_FRE(27), .BAUD_RATE(9600))
                uart_0(.rst(rst), .clk(clk),
                .addr(addr),
                .data_in(mem_wdata),
                .rd_strobe(rstrb ),
                .wr_strobe(wr_strobe),
                .data_out(uart_data),
                .uart_tx(uart_tx),
                .uart_rx(uart_rx)
                );
  
endmodule

