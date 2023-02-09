start:
    addi t0, x0, 0
    jal shot
shot:
    addi t0, t0, 1
    jal testing_is_the_worst_part_of_projects

come:
    addi t0, x0, 1
    jal go
go:
    jal I_am_done

haha:
    addi t0, x0, 2
    jal come
you_can_get_them_all:
    jal haha

testing_is_the_worst_part_of_projects:
    addi t0, x0, 1

I_am_done:
