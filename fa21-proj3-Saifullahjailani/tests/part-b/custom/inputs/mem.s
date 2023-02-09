addi a0, x0, 200
lui  t3, 86627   
addi t3, t3, 1864  # Try to maximize the value
sw   t3, 80(a0)
sw   t3, -20(a0)
lw   t0, 80(a0)
lh   t0, 80(a0)
lh   t1, 82(a0)
lb   t0, 80(a0)
lb   t1, 81(a0)
lb   t2, 82(a0)
lb   t0, 83(a0)
lw   t0, -204(a0)
lh   t0, -204(a0)
lh   t1, -202(a0)
lb   t0, -204(a0)
lb   t1, -203(a0)
lb   t2, -202(a0)
lb   t0, -201(a0)

