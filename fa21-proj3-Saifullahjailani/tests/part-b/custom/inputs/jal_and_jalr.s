addi a0, x0, 10
addi t0, x0, 0
addi t1, x0, 1
jal fib

fib:
    beq a0, x0, done
    add t3, x0, t1
    add t1, t1, t0
    add t0, t3, x0
    addi a0, a0, -1
    jal fib
done:
    add a0, x0, t1
    jalr ra, ra, 0