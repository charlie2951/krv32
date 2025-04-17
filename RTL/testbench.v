`include "top.v"
module top_test;
  reg rst, clk;
  wire [31:0] cycle;
  wire [7:0] leds, uart_d, uart_c;
  integer i;
  //instantiate DUT
  top dut (rst, clk, cycle,leds, uart_d, uart_c);
  initial
  begin
    $dumpfile("test.vcd");
    $dumpvars;
    rst=1;
    clk=0;
    #50;
    rst=0;
    #10000;
    /*
    for(i=0;i<20;i=i+1)
    begin
      $display("data at loc %d is: %8h", i, dut.mem0.PROGMEM[i]);
    end
    */
    $finish;
  end

  initial
  begin
    // $monitor("Time=%0d, X[0]=%0d, X[1]=%0d, X[2]=%0d, X[3]=%0d, X[4]=%0d,  X[5]=%0d, Cycle=%0d",$time, dut.cpu0.regfile[0],dut.cpu0.regfile[1],dut.cpu0.regfile[2],dut.cpu0.regfile[3],dut.cpu0.regfile[4],dut.cpu0.regfile[5], cycle);
    $monitor("Time=%6d, Cycle=%d, LEDS = %d, uart_data=%d, uart_ctrl=%d", $time,cycle, leds, uart_d, uart_c);
  end

  always #5 clk=~clk;
endmodule
