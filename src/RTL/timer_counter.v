module timer_counter(
    input rst_n,
    input clk,
    input start,
    input mode,//mode-0:timer, mode-1:counter
    input [31:0] timer_val,
    output reg done,
    output [31:0] count_value
);

//parameter MAX_COUNT = 32'hFFFFFFFF;//load this value as max count in counter
reg [1:0] state, next_state;
localparam IDLE=0, COUNT=1, STOP=2;

reg [31:0] free_count;//free running counter
reg [31:0] cycle_cnt;
wire [31:0] count_limit;
//assign count_limit = mode ? MAX_COUNT: timer_val;
assign count_limit = timer_val;
reg start_d;
//reg running=0;
//start logic to clear the internal reg
always @(posedge clk or negedge rst_n) begin
    if(!rst_n)
        start_d <= 0;
    else
        start_d <= start;
end

wire start_pulse = start & ~start_d;  // rising edge detect

//--------------------------------------------------
// State register
//--------------------------------------------------
always @(posedge clk or negedge rst_n) begin
    if(!rst_n)
        state <= IDLE;
    else
        state <= next_state;
end

//-------------------------------------------------- 
// Next state logic (combinational) 
//-------------------------------------------------- 
always @(*) begin 
case(state) 
IDLE: next_state = start ? COUNT : IDLE; 
/* COUNT: begin 
//next_state = (cycle_cnt == count_limit) ? STOP : COUNT; 
next_state = (cycle_cnt != count_limit) ? COUNT: (mode==1) ? COUNT : STOP; 
end */ 
COUNT: begin 
if(mode == 0) 
begin 
next_state = (cycle_cnt == count_limit) ? STOP : COUNT; 
end else begin next_state = COUNT; 
// free-running counter 
end 
end 
STOP: next_state = IDLE; 
default: next_state = IDLE; 
endcase 
end 
//-------------------------------------------------- 
// Counter logic 
//-------------------------------------------------- 
always @(posedge clk or negedge rst_n) 
begin 
if(!rst_n) 
cycle_cnt <= 0; 
else if(start_pulse) begin 
// clear when new start detected 
cycle_cnt <= 0; // running <= 1; 
end 
else if(state == COUNT) begin 
/* if(cycle_cnt == count_limit) //cycle_cnt <= 0; cycle_cnt <= (mode) ? cycle_cnt+1 : 0; else cycle_cnt <= cycle_cnt + 1; end else cycle_cnt <= 0; end */ 
if(mode == 0) begin 
// Timer mode 
if(cycle_cnt < count_limit) 
cycle_cnt <= cycle_cnt + 1; 
end 
//else begin 
// Counter mode (free running) 
//cycle_cnt <= cycle_cnt + 1; 
//end 
end 
end 
//-------------------------------------------------- 
// Done pulse (1 cycle) 
//-------------------------------------------------- 
always @(posedge clk or negedge rst_n) begin 
if(!rst_n) done <= 0; 
else 
done <= (state == STOP); 
end 
//assign internal count reg value to output port 
//assign count_value = cycle_cnt; 
//endmodule

//--------free running counter logic------------//
always @(posedge clk) begin
    if(!rst_n)
        free_count <=0;
    else if(mode)
        free_count <= free_count + 1;
    end //end of always
//assigning internal count value to output port
assign count_value = free_count;
endmodule
