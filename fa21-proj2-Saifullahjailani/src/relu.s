.globl relu

.text
# ==============================================================================
# FUNCTION: Performs an inplace element-wise ReLU on an array of ints
# Arguments:
# 	a0 (int*) is the pointer to the array
#	a1 (int)  is the # of elements in the array
# Returns:
#	None
# Exceptions:
# - If the length of the vector is less than 1,
#   this function terminates the program with error code 57
# ==============================================================================
relu:
    # Prologue
	addi sp, sp, -4
	sw ra, 0(sp)
	jal loop_start

loop_start:
	li t1, 1
	blt a1, t1, exit
	jal loop_continue 
    
loop_continue:
	blt a1, t1, loop_end
	lw t0, 0(a0)
	addi a1, a1, -1
	addi a0, a0, 4
	bge t0, x0, loop_continue
	addi a0, a0, -4
	sw x0, 0(a0)
	addi a0, a0, 4
	jal loop_continue  # jump to loop_continue and save position to ra
	

loop_end:
    # Epilogue
	lw ra, 0(sp)
	addi sp, sp, 4
	ret

exit:
	lw ra, 0(sp)
	addi sp, sp, 4
	li a1, 57
	call exit2
	ret