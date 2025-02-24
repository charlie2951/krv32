//*********** Testbench for CPU *** //

`include "cpu_v02.v"
module top_test;
  reg rst, clk;
  wire cpu_busy;
  integer i;
  //instantiate DUT
  cpu cpuv02 (rst, clk, cpu_busy);
  initial
  begin
    $dumpfile("top.vcd");
    $dumpvars;
    rst=1;
    clk=0;
    #50;
    rst=0;
    #5000;
   
    //************Print register content**************//
    $display("*** Printing register content ***");
    for(i=0; i<16; i=i+1)
      $display("X[%0d] = %0d ",i,$signed(cpuv02.regfile[i]));
      $display("Printing data memory");
     $display("datamem(23)=%0d",cpuv02.datamem[23]);
    //$display("datamem(17)=%0d",cpuv02.datamem[17]);
    //$display("datamem(18)=%0d",cpuv02.datamem[18]);
   // $display("datamem(19)=%0d",cpuv02.datamem[19]);
    $finish;
  end

  always #5 clk=~clk;
endmodule
