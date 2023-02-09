addi t0, x0, -1
addi t1, x0, 1

jump1:
    beq t1, x0, done

jump2:
    bne t0, t1, jump4
    

jump3:
    

jump4:
    bge t1, t0, jump7 
    

jump5:


jump6:
    bgeu t0, x0, jump9

jump7:
    blt t0, t1, jump6

jump8:
    

jump9:
    addi t1, t1, -1
    bgeu t0, t1, jump1
    

done:
