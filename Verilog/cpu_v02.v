`include "addressing_mode.v"

//*** Version-3 of KRV-32 ***//
// ** Attempt to make the code compatible with GCC RISC-V compiler ***//
//** Program memory space 0-127
// Data memory and IO space  starts from Addr 2000.
// LED port connected to ADDR = 2000-2003

module cpu(
    input rst, clk,
    output reg cpu_busy,
    output [5:0] LEDS
  );
  parameter SIZE = 128; //program memory size
  parameter DATA_OFFSET = 2000; //data mem location starts in address space 2000
  reg divclk;
  reg [7:0] ram [0:SIZE-1]; //program memory 
  reg [7:0] datamem [0:31];//data and IO memory 
  reg [31:0] regfile[0:15];//Register file with X0 to X15;
  reg [31:0] addr; //address bus
  reg [31:0] data; //data bus
  reg en, alu_en; //ALU control signal
  reg [3:0] state; //state register
  parameter RESET=0, FETCH=1, DECODE=2, REG_RD=3, ALU=4, PC_UPDATE=5, REG_WR=6, BRANCHING=7, JUMPING=8, LUI=9, AUIPC=10; //Different states
  wire [3:0] mode; //addressing mode finder
  reg [4:0] rd,rs1, rs2; //reg to find source and dest reg number
  reg [2:0] funct3; //funct3 (see Risc-V manual)
  reg [6:0] funct7, alu_code; //funct7 and ALU operation code
  reg [31:0] data_rs1, data_rs2; //source reg1 and 2 data
  //note: data_rs2 is also used for processing immediate data
  //this techniques is used to use less hardware on FPGA
  initial
  begin
    ram[0]=32'h0;
    $readmemh("code1.txt", ram);
    state=0;
    en=0;
    divclk=0;
    addr = 0;
    regfile[0]=32'h0;//X0 reg is always set to hardware 0
    regfile[1]=32'h0;
    regfile[2]=32'h0;
    regfile[3]=32'h0;
    regfile[4]=32'h0;
    regfile[5]=32'h0;
    regfile[6]=32'h0;
    regfile[7]=32'h0;
    regfile[8]=32'h0;
    regfile[9]=32'h0;
    regfile[10]=32'h0;
    regfile[11]=32'h0;
    regfile[12]=32'h0;
    regfile[13]=32'h0;
    regfile[14]=32'h0;
    regfile[15]=32'h0;
  end
  //Instantiate sub modules
  addressing_mode mode_finder(data[6:0],mode);
  //program memory

  //always @(addr)
  //  data = (!rst) ? 32'h0 : {ram[addr],ram[addr+1],ram[addr+2],ram[addr+3]};

    assign LEDS = ~(datamem[3][5:0]); //LEDS are connected to datamem[3]

// ---------------------------------------------------------------------//
// *** Implementation of clock divider to get a 10MHz clock from 25MHz *** //
//Approx divided by two
/*
always @(posedge clk) begin
divclk <= ~divclk;
end
*/

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
      RESET:
      begin
        if(!rst)
          state <= RESET;
        else
          state <= FETCH;
      end

      FETCH: //Fetch data from progmem RAM
      begin
        data[31:24] <= ram[addr];
        data[23:16] <= ram[addr+1];
        data[15:8] <= ram[addr+2];
        data[7:0] <= ram[addr+3];
        state <= DECODE;
      end
      DECODE: //Decoding of different instruction and generate signal
      begin
        cpu_busy <= 1;
        case(mode)
          1: //R type instruction
          begin
            rd <= data[11:7];
            funct3 <= data[14:12];
            rs1 <= data[19:15];
            rs2 <= data[24:20];
            funct7 <= data[31:25];
            state <= REG_RD;
          end
          2: //Immediate type instruction
          begin
            data_rs2 <=data[31]?{20'hfffff, data[31:20]}:{20'h0, data[31:20]};
            rd <= data[11:7];
            rs1 <= data[19:15];
            funct3 <= data[14:12];
            state <= REG_RD;

          end
          3: //Load type instruction
          begin
            //$display("Not implemented");
            rs1 <= data[19:15];
            rd <= data[11:7];
            data_rs2 <=data[31]?{20'hfffff, data[31:20]} - DATA_OFFSET :{20'h0, data[31:20]}-DATA_OFFSET;
            funct3 <= data[14:12];
            state <= REG_WR;
          end
          4: //store type instruction
          begin
            // $display("Not implemented");
            rs1 <= data[19:15];
            rs2 <= data[24:20];
            funct3 <= data[14:12];
            //data_rs1 <= regfile[rs1];
            data_rs2 <=data[31]?{20'hfffff, data[31:25],data[11:7]}-DATA_OFFSET:{20'h0, data[31:25],data[11:7]}-DATA_OFFSET;
            state <= REG_WR;

          end

          5: //Branch type instruction
          begin
            rs1 <= data[19:15];
            rs2 <= data[24:20];
            funct3 <= data[14:12];
            //immediate data i.e. offset for branch
            data_rs2 <= (data[31])?{19'h7ffff,data[31],data[7],data[30:25],data[11:8],1'b0}:{19'h00000,data[31],data[7],data[30:25],data[11:8],1'b0};
            state <= BRANCHING;
          end

          6: //for jump
          begin
            data_rs2 <= (data[31])?{11'h7ff,data[31],data[19:12],data[20],data[30:21],1'b0}:{11'h000,data[31],data[19:12],data[20],data[30:21],1'b0};
            funct3 <= data[14:12];
            rs1 <= data[19:15];
            rd <= data[11:7];
            state <= JUMPING;
          end

          7: //LUI
          begin
            rd <= data[11:7];
            data_rs2[31:20] <= data[31:20] ;
            data_rs2[19:0] <= 3'h0;
            state <= LUI;
          end
          8: //AUIPC
          begin
            rd <= data[11:7];
            data_rs2[31:20] <= data[31:20] ;
            data_rs2[19:0] <= 3'h0;
            state <= AUIPC;
          end
          default:
            state <= PC_UPDATE;
        endcase
      end
      REG_RD: //state for reading register for source data
      begin
        data_rs1 <= regfile[rs1];
        data_rs2 <= (mode==1)? regfile[rs2]:data_rs2;
        alu_en <= 1;
        alu_code <= (mode==1)?({4'b0000,funct3} | funct7):((mode==2)?funct3:0);
        state <= ALU;
      end

      ALU: //perform ALU operation follwed by store result into regfile
      begin
        if(alu_en)
        begin
          case(alu_code)
            7'b0000000:
              regfile[rd] <= data_rs1 + data_rs2; //addition ADDI
            7'b0100000:
              regfile[rd] <= data_rs1 - data_rs2; // subtract
            7'b0000111:
              regfile[rd] <= data_rs1 & data_rs2; //logical AND 
            7'b0000110:
              regfile[rd] <= data_rs1 | data_rs2; //logical OR
            7'b0000100:
              regfile[rd] <= data_rs1 ^ data_rs2; //logical xor
            7'b0000001:
              regfile[rd] <= data_rs1 << data_rs2; //shift left,SLLI
            7'b0000101:
              regfile[rd] <= data_rs1 >> data_rs2; //shift right
            7'b0000010:
              regfile[rd] <= (data_rs1 < data_rs2)? 1:0; //set less than
            7'b0000011:
              regfile[rd] <= (data_rs1 < {20'h0,data_rs2[31:20]})? 1:0; //SLTIU
          endcase
          // $display(regfile[rd]);
        end
        else
          regfile[rd] <= 32'bx;

        state <= PC_UPDATE;
      end
      PC_UPDATE: //update the program counter and decide to pickup next instruction
      begin
        addr <= (mode!=9)?(addr + 4):0;
        state <= FETCH;
        cpu_busy <= 0;
      end

      REG_WR: //register or mem write for special op such as load
      begin

        if(mode==3)
        begin //for Load type instruction
          case(funct3)
            3'b000://LB
              regfile[rd] <= {6'h0, datamem[regfile[rs1] + data_rs2]};

            3'b001://LH
              regfile[rd] <= {4'h0, datamem[regfile[rs1] + data_rs2],datamem[regfile[rs1] + data_rs2+1]};

            3'b010://LW
              regfile[rd] <= {datamem[regfile[rs1] + data_rs2],datamem[regfile[rs1] + data_rs2+1],datamem[regfile[rs1] + data_rs2+2],datamem[regfile[rs1] + data_rs2+3]};

           // 3'b100://LBU

            //  $display("Not implemented");
           // 3'b101://LHU

            //  $display("Not implemented");
           // default:
            //  $display("Not implemented");
          endcase
        end
        if(mode==4)
        begin //store type instruction

          case(funct3)
            3'b000: //store byte
            begin
              datamem[regfile[rs1] + data_rs2 + 3] <= regfile[rs2][7:0];
              datamem[regfile[rs1] + data_rs2 + 2] <= 8'h0;
              datamem[regfile[rs1] + data_rs2 + 1] <= 8'h0;
              datamem[regfile[rs1] + data_rs2 ] <= 8'h0;
            end
            3'b001: //store half
            begin
              datamem[regfile[rs1] + data_rs2 + 3] <= regfile[rs2][7:0];
              datamem[regfile[rs1] + data_rs2 + 2] <= regfile[rs2][15:8];
              datamem[regfile[rs1] + data_rs2 + 1] <= 8'h0;
              datamem[regfile[rs1] + data_rs2 ] <= 8'h0;
            end
            3'b010: //store word
            begin
              datamem[regfile[rs1] + data_rs2 + 3] <= regfile[rs2][7:0];
              datamem[regfile[rs1] + data_rs2 + 2] <= regfile[rs2][15:8];
              datamem[regfile[rs1] + data_rs2 + 1] <= regfile[rs2][23:16];
              datamem[regfile[rs1] + data_rs2 ] <= regfile[rs2][31:24];
              //$display(datamem[16]);
            end

          endcase
        end
        state <= PC_UPDATE;

      end


      BRANCHING: //for branch instructions
      begin
        case(funct3)
          3'h0:
            addr <= (regfile[rs1]==regfile[rs2]) ?  (addr+data_rs2): (addr+4);//BEQ
          3'h1:
            addr <= (regfile[rs1]!=regfile[rs2]) ?  (addr+data_rs2): (addr+4);//BNE
          3'h4:
            addr <= (regfile[rs1]<regfile[rs2]) ?  (addr+data_rs2): (addr+4);//BLT
          3'h5:
            addr <= (regfile[rs1]>=regfile[rs2]) ?  (addr+data_rs2): (addr+4);//BGE
          // 3'h6: addr <= (regfile[rs1]<regfile[rs2]) ?  (addr+data_rs2): (addr+4);//BLTU
          // 3'h7: addr <= (regfile[rs1]>=regfile[rs2]) ?  (addr+data_rs2): (addr+4);//BGEU
        endcase
        state <= FETCH;
      end

      JUMPING: //for Jump instructions
      begin
        regfile[rd] <= (rd==0)?0:(addr + 4); //rd=PC+4; //change for x0 to include j instruction in compiler
        addr <= (mode==6)? (addr + data_rs2):((mode==10)?(regfile[rs1]+data_rs2):addr); //PC=PC+imm for jal, PC=rs1+imm for jalr

        state <=FETCH;
      end

      LUI:
      begin
        regfile[rd] <= data_rs2;
        state <= PC_UPDATE;
      end

      AUIPC:
      begin
        regfile[rd] <= addr + data_rs2;
        state <= PC_UPDATE;
      end
    endcase
  end



endmodule
