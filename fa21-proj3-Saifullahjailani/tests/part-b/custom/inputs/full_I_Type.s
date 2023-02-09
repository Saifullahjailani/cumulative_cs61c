addi t0, x0, 0
addi t1, x0, 1
slli t3, t0, 3
slti t4, t0, 4
xori s0, t0, 1
srli s1, t1, 3
srai t3, t3, 3
ori t0, t1, 29
andi t0, t0, 0
addi a0, x0, 400
sw t0, 0(a0)
sh t1, 4(a0)
sb t1, 8(a0)

lw t0, 0(a0)
lh t1, 4(a0)
lb t1, 8(a0)
