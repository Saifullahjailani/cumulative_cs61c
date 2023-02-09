addi t5, x0, -10
Loop:
    bge t5, x0, Done
    addi t5, t5, 1
    blt t5, x0, Loop

Done:
    addi t5, x0,100
