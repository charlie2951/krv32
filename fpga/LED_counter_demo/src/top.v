//We are going to map 6 on board LED to CPU
//LED memory space starts from 0x10000000

module top(
    input rst, clk,
    //output [31:0] cycle,
    output [5:0] LEDS
  );
  wire [31:0]   addr;
  wire [31:0] mem_wdata, mem_rdata,io_wdata, wdata, rdata;
  wire rstrb;
  wire [3:0] wstrb;
  reg [7:0] io_rdata;
  //*** GPIO Port mapping and  registers **//
  reg [7:0] led_gpio, uart_data, uart_ctrl;
  assign LEDS = ~led_gpio[5:0];//00-03
  //assign uart_d = uart_data;//04-07
  //assign uart_c = uart_ctrl;//08-0b
  //Instantiate sub modules
  cpu cpu0(
        .rst(rst), .clk(clk),
        .mem_rdata(rdata),
        .mem_wdata(wdata),
        .mem_addr(addr),
        .cycle(),
        .mem_rstrb(rstrb),
        .mem_wstrb(wstrb)
      );

  progmem mem0(
            .rst(rst), .clk(clk),
            .addr(addr),
            .data_in(mem_wdata),
            .rd_strobe(rstrb),
            .wr_strobe(wstrb),
            .data_out(mem_rdata)
          );
  //Select program memory or GPIO depending upon address space
  wire isGPIO = (addr[31:28]==4'b0001);
  assign rdata =  isGPIO ? {24'h0,io_rdata} : mem_rdata;
  assign mem_wdata = wdata;

  always @(posedge clk)
  begin
    if(!rst)
    begin
      led_gpio <= 0;
      uart_ctrl <= 0;
      uart_data <= 0;
    end
    else if(rstrb)
    case(addr[3:2])
      2'b00:
        io_rdata <= led_gpio; //0x10000000-03
      2'b01:
        io_rdata <= uart_data;//0x10000004-07
      2'b10:
        io_rdata <= uart_ctrl;//0x10000008-0b
    endcase
    else if(wstrb[0] & isGPIO)
    begin
      case(addr[3:2])
        2'b00:
          led_gpio <= wdata[7:0];
        2'b01:
          uart_data <= wdata[7:0];
        2'b10:
          uart_ctrl <= wdata[7:0];
      endcase

    end

  end
endmodule

