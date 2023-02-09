addi t0, x0, 4
addi t1, x0, 4
addi t3, x0, 0
beq t0, t1, foruth

first:
    addi t3, x0, 20
    beq t0, t1, last
second:
    beq t0, t1, first
third:
    beq t0, t1, second 
foruth:
    beq t0, t1, third
last:
	