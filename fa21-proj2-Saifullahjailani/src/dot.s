.globl dot

.text
# =======================================================
# FUNCTION: Dot product of 2 int vectors
# Arguments:
#   a0 (int*) is the pointer to the start of v0
#   a1 (int*) is the pointer to the start of v1
#   a2 (int)  is the length of the vectors
#   a3 (int)  is the stride of v0
#   a4 (int)  is the stride of v1
# Returns:
#   a0 (int)  is the dot product of v0 and v1
# Exceptions:
# - If the length of the vector is less than 1,
#   this function terminates the program with error code 57
# - If the stride of either vector is less than 1,
#   this function terminates the program with error code 58
# =======================================================
dot:

    li t0, 1
	blt a2, t0, exit
    blt a3, t0, exit_2
    blt a4, t0, exit_2
    # Prologue
    addi sp, sp, -4
    sw ra, 0(sp)
    jal loop_start
    


loop_start:
    addi a2, a2, -1 # to make counter hit at zero
    li t0, 4 # size of word 
    mul a3, a3, t0
    mul a4, a4, t0
    li t0, 0 # dot
    jal loop_continue

loop_continue:
    blt a2, x0, loop_end
    addi a2, a2, -1
    lw t1, 0(a0)
    lw t2, 0(a1)
    mul t3, t1, t2
    add t0, t0, t3
    add a0, a0, a3
    add a1, a1, a4
    jal loop_continue
loop_end:
    mv a0, t0
    lw ra, 0(sp)
    addi sp, sp, 4
    ret

exit:
	li a1, 57
	call exit2
	ret

exit_2:
    li a1 58
    call exit2
    ret
