//UART GPIO unit to control UART transmitter
//address range: DATA: 0x2000-0000
//CTRL: 0x2000-0004
// STATUS: 0x2000-00008
//`include "uart_tx.v"
module  uart1_tx_gpio(
    input [31:0] addr,
    input rst, clk,
    input [31:0] data_in,
    input rd_strobe,
    input [3:0] wr_strobe,
    output reg [31:0] data_out,
    output uart_tx
  );

  reg [31:0] uart_data, uart_control;

  //flags
  wire isUART_DATA= (addr==32'h30000000);
  wire isUART_CTRL= (addr==32'h30000004);
  wire isUART_STATUS= (addr==32'h30000008);

  wire o_ready;

  initial
  begin
    uart_data <= 0;
    uart_control <= 0;
  end

  always @(posedge clk)
  begin
    if(rst)
    begin
      uart_data <= 0;
      uart_control <= 0;
    end
    else if(rd_strobe && isUART_STATUS)
      data_out <= {31'h0, o_ready};
    else if(|wr_strobe && isUART_DATA)
      uart_data <= data_in;
    else if(|wr_strobe && isUART_CTRL)
      uart_control <= data_in;
     // uart_control <= 0; //only valid for 1 cycle 
  end

  //Instantiate UART transmitter module here
  uart1_tx tx1(.clk(clk), .rst_n(!rst),
              .tx_data(uart_data[7:0]),
              .tx_data_valid(uart_control[0]),
              .tx_data_ready(o_ready),
              .tx_pin(uart_tx)
             );

endmodule