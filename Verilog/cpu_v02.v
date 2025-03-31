/*
Copyright (c) 2024-2025 Subir Maity

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
/* CPU Version 3.0
Verified for synthesis in FPGA
FSM based design
R-type, I type and Branch instr included
Shift operator implemented (SLL/SLLI, SRL/SRLI)
STORE instructions implemented: SB, SW, SH
LOAD instruction: LH, LB, LW
Jump type: JAL and JALR implemented
LUI, AUIPC implememted
Known Bug: Arithmetic right shift SRA, SRAI not working
 
Adding  GPIO: addr[31:24] == 8'h1=LEDs
*/
module cpu(
    input rst, clk,
    output reg [9:0] cycle,
    output [5:0] LEDS //six on board led in FPGA board Tang 9k board 

  );
  parameter MEM_SIZE = 1024; //adjust mem size as per requirement
    reg [7:0] ram [0:MEM_SIZE-1]; //program memory space
  reg [31:0] regfile[0:31];//Register file with X0 to X31;
  reg [31:0] addr, data_rs1, data_rs2; //address bus
  reg [31:0] data; //data bus
  reg [3:0] state; //state register
  parameter RESET=0, FETCH=1, DECODE=2, EXECUTE=3, HLT=4,BYTE1=5, BYTE2=6, BYTE3=7, BYTE4=8, WAIT_LOADING=9; //Different states
  //********* Decoding of Instructions*******//
  wire [4:0] opcode = data[6:2];
  wire [4:0] rd = data[11:7];
  wire [2:0] funct3 = data[14:12];
  wire [6:0] funct7 = data[31:25];
  wire [31:0] I_data = {{21{data[31]}},data[30:20]}; //sign extended data
  wire [31:0] B_data = {{20{data[31]}},data[7],data[30:25],data[11:8],1'b0}; //sign extended branch data
  wire [31:0] S_data = {{21{data[31]}},data[30:25],data[11:7]};//sign extended imm data for S-type
  wire [31:0] J_data = {{12{data[31]}},data[19:12],data[20],data[30:21],1'b0};//sign extended jump data
  wire [31:0] U_data = {data[31],data[30:12],12'h000};//LUI, AUIPC , 12 bit shifted imm data
  // check whether opcode is for R type or I type  or B-type
  wire isRtype = (opcode == 5'b01100);
  wire isItype = (opcode == 5'b00100);
  wire isBtype = (opcode == 5'b11000);
  wire isSystype = (opcode == 5'b11100);
  wire isStype = (opcode == 5'b01000);
  wire isLtype = (opcode == 5'b00000);
  wire isJAL   = (opcode == 5'b11011);
  wire isJALR  = (opcode == 5'b11001);
  wire isLUI = (opcode == 5'b01101);
  wire isAUIPC = (opcode == 5'b00101);
  // flag to calculate mem location for load/store type
  reg [1:0] loc;
  reg [31:0] load_data_tmp=32'h0;
  wire [1:0] load_store_flag = (isStype|isLtype) ? loc:0;
  //  Design ALU using conditional operator
  wire [31:0] ADD = alu_in1 + alu_in2 + load_store_flag;
  wire [31:0] XOR = alu_in1 ^ alu_in2;
  wire [31:0] OR = alu_in1 | alu_in2;
  wire [31:0] AND = alu_in1 & alu_in2;
  wire [32:0] SUB = {1'b0,alu_in1} + {1'b1, ~alu_in2} + 1'b1; //2's comp additon=subtraction
  //shift operation(only SLL/SLLI and SRL/SRLI implemented)
  wire [31:0] shift_data_2 = isRtype ? alu_in2 : isItype ? {7'b0,alu_in2[4:0]}:0;
  wire [31:0] SLL = alu_in1 << shift_data_2;//left shift
  wire [31:0] SRL = alu_in1 >> shift_data_2;//right shift
  wire [31:0] SRA = alu_in1 >>> shift_data_2;//right shift arithmetic(keep sign) BUG

  //branching
  wire EQUAL =  (SUB[31:0] == 0); //if A and B are same then Sub result is 0
  wire NEQUAL = !EQUAL;
  wire LESS_THAN = (alu_in1[31] ^ alu_in2[31])? alu_in1[31]:SUB[32];
  wire LESS_THAN_U = SUB[32];
  wire GREATER_THAN = !LESS_THAN;
  wire GREATER_THAN_U = !LESS_THAN_U;
  wire TAKE_BRANCH = ((funct3==3'b000) & EQUAL)  |
       ((funct3==3'b111) & GREATER_THAN_U)       |
       ((funct3==3'b001) & NEQUAL)               |
       ((funct3==3'b100) & LESS_THAN)            |
       ((funct3==3'b101) & GREATER_THAN)         |
       ((funct3==3'b110) & LESS_THAN_U) ;

  // Note : for ADD and SUB, funct3 is same but funct7[5] is different

  wire [31:0] alu_result = (funct3==3'b000) & isRtype & ~funct7[5]? ADD: //ADD
       (funct3==3'b000) & isItype  ? ADD: //ADD
       (funct3==3'b000) & ~(isStype|isLtype) & funct7[5]? SUB[31:0]: //SUB
       (funct3==3'b100)? XOR: //XOR
       (funct3==3'b110)? OR: //OR
       (funct3==3'b010) & !(isStype|isLtype)? {31'b0, LESS_THAN}: //SLT chk
       (funct3==3'b011)? {31'b0, LESS_THAN_U}:
       (funct3==3'b001) & !(isStype|isLtype)? SLL: //SLL,SLLI chk
       (funct3==3'b101)? SRL: //SRL,SRLI
       (funct3==3'b101) & funct7[5]? SRA:
       (isStype | isLtype|isJALR) ? ADD:0; //S-type, L type, for mem location calc

  //source1 and source 2 data for ALU operation
  wire [31:0] alu_in1 = data_rs1; //source is always rs1 for both type
  wire [31:0] alu_in2 = (isRtype | isBtype)? data_rs2 : (isItype|isLtype|isJALR)? I_data:S_data;//ALU req for comparison in Btype
  wire [31:0] pcplus4 = addr + 4;
  wire [31:0] pcplusimm = addr + (isBtype ? B_data: isJAL ? J_data:isAUIPC ? U_data:0);
  integer i;
  // GPIO related signals
    reg [31:0] GPIO0; //only one GPIO -32 bit address space 0x10000000-0x10000003
  //bug here
  wire isGPIO = (isLtype | isStype) & (alu_result[31:28]==4'b0001);//0001 -LED, will be active only for load-store op
    assign LEDS = ~GPIO0[5:0];//assign last 6 bit to on board leds, board LEDS are having active low input

  initial
  begin
    for(i=0; i<MEM_SIZE; i=i+1)
      ram[i]=32'h0; //clear the whole mem before loading firmware
    $readmemh("firmware.txt", ram);//load firmware into ram
    for(i=0;i<32;i=i+1)
      regfile[i]=32'h0;
    state=0;
    addr = 0;
    cycle = 0;
    regfile[0] = 0;//X0 reg is always 0
    GPIO0 = 0;

  end

  //clock dependent operation

  always @(posedge clk)
  begin
    if(!rst)
    begin
      addr <= 0;
      state <= RESET;
      data <= 32'h0;
    end
    else
    case(state)
      RESET: //If reset is pressed
      begin
        if(!rst)
          state <= RESET;
        else
          state <= FETCH;
      end

      FETCH: //Fetch data from progmem RAM
      begin
        data <= {ram[addr], ram[addr+1], ram[addr+2],ram[addr+3]};
        loc <= 0;
        state <= isSystype ? HLT: DECODE;
      end

      DECODE: //Decoding of different instruction and generate signal
      begin
        data_rs1 <= regfile[data[19:15]];
        data_rs2 <= regfile[data[24:20]];
        //state <= (isItype | isRtype | isBtype)? ALU: isSystype ? HLT:EXECUTE;
        //state <= ~isSystype? EXECUTE:  HLT;
        state <= EXECUTE;
      end

      EXECUTE:
      begin

        addr <= (isBtype & TAKE_BRANCH)|isJAL ? pcplusimm : isJALR? alu_result: pcplus4;
        state <= !(isStype|isLtype|isJAL|isJALR) ? FETCH: BYTE1;
      end
      BYTE1:
      begin
        //$display("Rs1 content=%0d", data_rs1);
        if(isStype)
        begin
          if(!isGPIO)
            ram[alu_result]   <= funct3[1] ? data_rs2[31:24]:0; //store word
          else
            GPIO0[31:24] <= funct3[1] ? data_rs2[31:24]: 0;

        end

        if(isLtype)
        begin
          // $display("alu_res=%8h",alu_result);
          if(!isGPIO)
            load_data_tmp[31:24] <=  funct3[1] ? ram[alu_result]:0; //load word
          else
            load_data_tmp[31:24] <= funct3[1] ? GPIO0[31:24]:0;


        end
        loc <= 1;
        state <= BYTE2;
      end
      BYTE2:
      begin
        loc <= 2;
        if(isStype)
        begin
          if(!isGPIO)
            ram[alu_result] <= funct3[1]?data_rs2[23:16]:0; //store word
          else
            GPIO0[23:16] <= funct3[1] ? data_rs2[23:16]:0;
        end

        if(isLtype)
        begin
          if(!isGPIO)
            load_data_tmp[23:16] <= funct3[1]?ram[alu_result]:0; //load word
          else
            load_data_tmp[23:16] <= funct3[1] ? GPIO0[23:16]: 0 ;
        end
        // load_data_tmp[23:16] <= funct3[1]?ram[alu_result]:0; //load word
        state <= BYTE3;
      end
      BYTE3:
      begin
        loc <= 3;
        if(isStype)
        begin
          if(!isGPIO)
            ram[alu_result] <= (|funct3)?data_rs2[15:8]:0; //store half word
          else
            GPIO0[15:8] <= (|funct3) ? data_rs2[15:8]:0;
        end

        if(isLtype)
        begin
          if(!isGPIO)
            load_data_tmp[15:8] <= (|funct3)? ram[alu_result]:0;//load half word
          else
            load_data_tmp[15:8] <= (|funct3) ? GPIO0[15:8]:0;
        end
        // load_data_tmp[15:8] <= (|funct3)? ram[alu_result]:0;//load half word
        state <= BYTE4;
      end
      BYTE4:
      begin

        if(isStype)
        begin
          if(!isGPIO) //mem segment
            ram[alu_result] <= data_rs2[7:0];//Store byte
          else //led segment
            GPIO0[7:0] <= data_rs2[7:0];
        end

        if(isLtype)
        begin
          if(!isGPIO)
            load_data_tmp[7:0] <= ram[alu_result];//load byte
          else
            load_data_tmp[7:0] <= GPIO0[7:0];
        end
        //load_data_tmp[7:0] <= ram[alu_result];//load byte
        state <= isStype ? FETCH : WAIT_LOADING;

      end
      WAIT_LOADING:
        state <= FETCH;
      /*
      HLT:
      begin
      $display("Execution completed!");
      end
      */
    endcase
  end
  //*** clock cycle counter **//
  always @(posedge clk)
  begin
    if(!rst)
      cycle <= 0;
    else
    begin
      if(state != HLT)
        cycle <= cycle + 1;
    end
  end
  // ** Register file write back data **//
  // wire write_reg_en = ~(isBtype|isStype) &   (state==EXECUTE);
  wire write_reg_en = ((isItype|isRtype|isJAL|isJALR|isLUI|isAUIPC) &(state==EXECUTE))|(isLtype & (state==WAIT_LOADING));
  wire [31:0] write_reg_data = (isItype |isRtype) ? alu_result :
       isLtype ? load_data_tmp:
       (isJAL|isJALR)?pcplus4:
       isLUI?U_data:
       isAUIPC?pcplusimm:0;

  always @(posedge clk)
  begin
    if (write_reg_en)
      if (rd != 0)
        regfile[rd] <= write_reg_data;
  end

endmodule

