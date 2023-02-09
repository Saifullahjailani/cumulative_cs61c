.globl argmax

.text
# =================================================================
# FUNCTION: Given a int vector, return the index of the largest
#	element. If there are multiple, return the one
#	with the smallest index.
# Arguments:
# 	a0 (int*) is the pointer to the start of the vector
#	a1 (int)  is the # of elements in the vector
# Returns:
#	a0 (int)  is the first index of the largest element
# Exceptions:
# - If the length of the vector is less than 1,
#   this function terminates the program with error code 57
# =================================================================
argmax:
    li t1, 1
	blt a1, t1, exit
    # Prologue
    addi sp, sp, -4
    sw ra, 0(sp)
    
    # argmax index
    li t0, 0
    # value at index t0
    lw t2, 0(a0)

    # current index
    li t3, 0
    
    jal loop_start  # jump to loop_start and save position to ra
    

loop_start:
    beq t3, a1, loop_end
    lw t4, 0(a0)
    blt t4, t2,loop_continue
    mv t0, t3
    mv t2, t4  
    jal loop_continue  # jump to loop_continue and save position to ra
    
loop_continue:
	addi a0, a0, 4
	addi t3, t3, 1
	jal loop_start
	
loop_end:
    mv a0, t0
     # Epilogue
	lw ra, 0(sp)
	addi sp, sp, 4
    ret


exit:
	li a1, 57
	call exit2
	ret
