//UART GPIO unit to control UART transmitter
//address range: RXDATA: 0x2000_0010
//RXCTRL: 0x2000_0014
// RXSTATUS: 0x2000_0018
//
//UART GPIO unit to control UART transmitter
//address range: DATA: 0x2000_0000
//CTRL: 0x2000_0004
// STATUS: 0x2000_0008


module  UART_GPIO  #(parameter CLK_FRE = 27, BAUD_RATE = 9600)
(
    input [31:0] addr,
    input rst, clk,
    input [31:0] data_in,
    input rd_strobe,
    input [3:0] wr_strobe,
    output reg [31:0] data_out,
    input uart_rx, //serial data in uart receive
    output uart_tx //serial data out from uart for transmission
  );

  reg [31:0] uart_rx_data, uart_rx_status, uart_rx_control;
  reg [31:0] uart_tx_data, uart_tx_control;

  //flags
  wire isUART_RXDATA    = (addr==32'h20000010);//0x5
  wire isUART_RXCTRL    = (addr==32'h20000014);//0x6
  wire isUART_RXSTATUS  = (addr==32'h20000018);//0x7

  wire o_rx_ready;
  wire [7:0] rx_data;
  wire o_tx_ready;
//tx part
  //flags
  wire isUART_TXDATA    = (addr==32'h20000000);//0x0002_0000
  wire isUART_TXCTRL    = (addr==32'h20000004);//0x0003_0000
  wire isUART_TXSTATUS  = (addr==32'h20000008);//0x0004_0000
 

  initial
  begin
    uart_rx_control <= 0;
    uart_tx_data <= 0;
    uart_tx_control <= 0;
  end

  always @(posedge clk)
  begin
    if(!rst)
    begin
      uart_rx_control <= 0;
      uart_tx_data <= 0;
      uart_tx_control <= 0;
    end
// rx part   control logic
     if(rd_strobe && isUART_RXSTATUS)
      data_out <= o_rx_ready;
     else if(rd_strobe && isUART_RXDATA)
      data_out <= {24'h0, rx_data};
     else if(|wr_strobe && isUART_RXCTRL)
      uart_rx_control <= data_in;
//transmitter part control logic
     else if(rd_strobe && isUART_TXSTATUS)
      data_out <= {31'h0, o_tx_ready};
    else if(|wr_strobe && isUART_TXDATA)
      uart_tx_data <= data_in;
    else if(|wr_strobe && isUART_TXCTRL)
      uart_tx_control <= data_in;
  end


   //Instantiate UART receiver module here
  uart_rx #(.CLK_FRE(27), .BAUD_RATE(9600)) rx0(.clk(clk), .rst_n(rst),
              .rx_data(rx_data),
              .rx_data_valid(o_rx_ready),
              .rx_data_ready(uart_rx_control[0]),
              .rx_pin(uart_rx)
             );

 //Instantiate UART transmitter module here
  uart_tx #(.CLK_FRE(27), .BAUD_RATE(9600)) tx0(.clk(clk), .rst_n(rst),
              .tx_data(uart_tx_data[7:0]),
              .tx_data_valid(uart_tx_control[0]),
              .tx_data_ready(o_tx_ready),
              .tx_pin(uart_tx)
             );
  
endmodule

