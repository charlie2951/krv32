//*********** Testbench for CPU *** //
// * Note that this testbench is used to test a 6 bit counter where LEDS are connected at o/p *//
//** LED Ports are active low in nature (inverted logic) //
`include "cpu_v02.v"

module top_test;
  reg rst, clk;
  wire cpu_busy;
  wire [5:0] LEDS;
  integer i;
  //instantiate DUT
  cpu cpuv02 (rst, clk, cpu_busy,LEDS);
  initial
  begin
    $dumpfile("top.vcd");
    $dumpvars;
    rst=0;
    clk=0;
    #50;
    rst=1;
    #60000;
   
    //$monitor("LED status: %0b", LEDS);
    //Print register content
    //$display("*** Printing register content ***");
    //$display("X[0](zero) = %0d ",$signed(cpuv02.regfile[0]));
    //$display("X[2](sp) = %0d ",$signed(cpuv02.regfile[2]));
    //$display("X[14](a4) = %0d ",$signed(cpuv02.regfile[14]));
    //$display("X[15](a5) = %0d ",$signed(cpuv02.regfile[15]));
    //for(i=0; i<16; i=i+1)
      //$display("X[%0d] = %0d ",i,$signed(cpuv02.regfile[i]));
     // $display("Printing data memory");
     //$display("datamem(23)=%0d",cpuv02.datamem[23]);
    //$display("datamem(17)=%0d",cpuv02.datamem[17]);
    //$display("datamem(18)=%0d",cpuv02.datamem[18]);
   // $display("datamem(19)=%0d",cpuv02.datamem[19]);
    $finish;
  end

  always #2 clk=~clk;

  
  initial begin
    $monitor("Time=%0d, Addr=%0h, Data=%0h, zero=%0d, sp=%0d, a4=%0d, a5=%0d, LEDS=%0b",$time,cpuv02.addr, cpuv02.data,cpuv02.regfile[0],cpuv02.regfile[2],cpuv02.regfile[14],cpuv02.regfile[15], LEDS);
   
  end
endmodule

