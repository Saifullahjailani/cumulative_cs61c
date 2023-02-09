addi a0, x0, 200
lui  t1, 86627
addi t1, t1, 2047
sw   t1, 100(a0)
sw   t1, -20(a0)
lw   t0, 100(a0)
lh   t0, 100(a0)
lh   t1, 100(a0)
lb   t0, 100(a0)
lb   t1, 101(a0)
lb   t2, 102(a0)
lb   t0, 103(a0)