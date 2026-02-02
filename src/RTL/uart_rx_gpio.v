//UART GPIO unit to control UART transmitter
//address range: RXDATA: 0x5000-0000
//RXCTRL: 0x6000-0000
// RXSTATUS: 0x7000-0000

module  uart_rx_gpio(
    input [31:0] addr,
    input rst, clk,
    input [31:0] data_in,
    input rd_strobe,
    input [3:0] wr_strobe,
    output reg [31:0] data_out,
    input uart_rx //serial data in uart receive
  );

  reg [31:0] uart_data, uart_status, uart_control;

  //flags
  wire isUART_RXDATA= (addr[31:28]==4'b0101);//0x5
  wire isUART_RXCTRL= (addr[31:28]==4'b0110);//0x6
  wire isUART_RXSTATUS= (addr[31:28]==4'b0111);//0x7

  wire o_ready;
  wire [7:0] rx_data;
  initial
  begin
    //uart_data <= 0;
    //uart_status <= 0;
    uart_control <= 0;
  end

  always @(posedge clk)
  begin
    if(rst)
    begin
      //uart_data <= 0;
     // uart_status <= 0;
      uart_control <= 0;
    end
     if(rd_strobe && isUART_RXSTATUS)
      data_out <= o_ready;
     if(rd_strobe && isUART_RXDATA)
      data_out <= {24'h0, rx_data};
     if(|wr_strobe && isUART_RXCTRL)
      uart_control <= data_in;
      //uart_control <= 0; //only valid for 1 cycle 
  end

  //Instantiate UART transmitter module here
  uart_rx rx0(.clk(clk), .rst_n(!rst),
              .rx_data(rx_data),
              .rx_data_valid(o_ready),
              .rx_data_ready(uart_control[0]),
              .rx_pin(uart_rx)
             );


  
endmodule
