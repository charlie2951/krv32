//*********** Testbench for CPU *** //
// * Note that this testbench is used to test a 6 bit counter where LEDS are connected at o/p *//
//** LED Ports are active low in nature (inverted logic) //
`include "cpu_v02.v"

module top_test;
  reg rst, clk;
  wire [9:0] cycle;
  wire [5:0] leds;
  integer i;
  parameter data_loc = 32; //if you want to see mem data for load/store
  //instantiate DUT
  cpu cpuv02 (rst, clk, cycle, leds);
  initial
  begin
    $dumpfile("top.vcd");
    $dumpvars;
    rst=0;
    clk=0;
    #50;
    rst=1;
    #90000;
/*
    //Print register content
    $display("*** Printing register content ***");
    for(i=0; i<16; i=i+1)
      $display("X[%0d] = %0d ",i,$signed(cpuv02.regfile[i]));
    $display("Clock cycle=%0d", cycle);
    //$display("Data at loc %0d : %0d ",data_loc,$signed({cpuv02.ram[data_loc],cpuv02.ram[data_loc+1],cpuv02.ram[data_loc+2],cpuv02.ram[data_loc+3]}));
      $display("LED value is: %6b", leds);
    */
    $finish;
  end
  
  initial begin
    $monitor("LED value is: %6b", leds);
   end
  //*/
  always #2 clk=~clk;
endmodule


