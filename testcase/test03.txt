//multiplication of negative numbers
addi x1,x0,-10
addi x2,x0,5
addi x3, x0, 0
addi x4, x0,0
loop:
add x3, x3, x1
addi x4, x4,1
bne x4,x2,loop 

//hex code
ff600093
00500113
00000193
00000213
001181b3
00120213
fe221ce3

//result
X[0] = 0 
X[1] = -10 
X[2] = 5 
X[3] = -50 
X[4] = 5 

//comment
test passed