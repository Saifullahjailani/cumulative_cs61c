.globl matmul

.text
# =======================================================
# FUNCTION: Matrix Multiplication of 2 integer matrices
# 	d = matmul(m0, m1)
# Arguments:
# 	a0 (int*)  is the pointer to the start of m0
#	a1 (int)   is the # of rows (height) of m0
#	a2 (int)   is the # of columns (width) of m0
#	a3 (int*)  is the pointer to the start of m1
# 	a4 (int)   is the # of rows (height) of m1
#	a5 (int)   is the # of columns (width) of m1
#	a6 (int*)  is the pointer to the the start of d
# Returns:
#	None (void), sets d = matmul(m0, m1)
# Exceptions:
#   Make sure to check in top to bottom order!
#   - If the dimensions of m0 do not make sense,
#     this function terminates the program with exit code 59
#   - If the dimensions of m1 do not make sense,
#     this function terminates the program with exit code 59
#   - If the dimensions of m0 and m1 don't match,
#     this function terminates the program with exit code 59
# =======================================================
matmul:
    # Error checks
    li t0, 1
    blt a1, t0, exit
    blt a2, t0, exit
    blt a4, t0, exit
    blt a5, t0, exit
    bne a2, a4, exit 
    # Prologue
    addi sp, sp, -44
    sw ra, 0(sp)
    sw s0, 4(sp)
    sw s1, 8(sp)
    sw s2, 12(sp)
    sw s3, 16(sp)
    sw s4, 20(sp)
    sw s5, 24(sp)
    sw s6, 28(sp)
    sw s7, 32(sp)
    sw s8, 36(sp)
    sw s9, 40(sp)

    mv s0, a0 # Adress A
    mv s1, a3 # Adress B
    mv s2, a6 # Adress C

    mv s3, a2    # a2 = a4 width of A and height of B
    mv s4, a1    # height A
    mv s5, a5    # width B

    li t0, 4     # size of int
    mul s6, t0, s3 # jumpsize

    li s7, 1 # i counter
    jal outer_loop_start

outer_loop_start:
    bgt s7, s4, outer_loop_end
    li s8, 1 # j counter
    mv s9, s1 
    jal inner_loop_start

inner_loop_start:
    bgt s8, s5, inner_loop_end
    mv a0, s0
    mv a1, s9
    mv a2, s3
    li a3, 1
    mv a4, s5
    jal dot
    sw a0, 0(s2)
    addi s2, s2, 4
    addi s8, s8, 1
    addi s9, s9, 4
    jal inner_loop_start

inner_loop_end:
    add s0, s6, s0
    addi s7, s7, 1
    jal outer_loop_start

outer_loop_end:
    # Epilogue
        
    lw ra, 0(sp)
    lw s0, 4(sp)
    lw s1, 8(sp)
    lw s2, 12(sp)
    lw s3, 16(sp)
    lw s4, 20(sp)
    lw s5, 24(sp)
    lw s6, 28(sp)
    lw s7, 32(sp)
    lw s8, 36(sp)
    lw s9, 40(sp)
    addi sp, sp, 44
    ret

exit:
	li a1, 59
	call exit2
	ret



