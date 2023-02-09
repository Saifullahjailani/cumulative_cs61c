.globl write_matrix

.text
# ==============================================================================
# FUNCTION: Writes a matrix of integers into a binary file
# FILE FORMAT:
#   The first 8 bytes of the file will be two 4 byte ints representing the
#   numbers of rows and columns respectively. Every 4 bytes thereafter is an
#   element of the matrix in row-major order.
# Arguments:
#   a0 (char*) is the pointer to string representing the filename
#   a1 (int*)  is the pointer to the start of the matrix in memory
#   a2 (int)   is the number of rows in the matrix
#   a3 (int)   is the number of columns in the matrix
# Returns:
#   None
# Exceptions:
# - If you receive an fopen error or eof,
#   this function terminates the program with error code 89
# - If you receive an fclose error or eof,
#   this function terminates the program with error code 90
# - If you receive an fwrite error or eof,
#   this function terminates the program with error code 92
# ==============================================================================
write_matrix:

    # Prologue
    addi sp, sp, -32
    sw ra, 0(sp)
    sw s0, 4(sp)   # char* file name
    sw s1, 8(sp)   # int row
    sw s2, 12(sp)  # int col
    sw s3, 16(sp)  # int discriptor
    sw s4, 20(sp)  # int* to matrix in memory
    sw s5, 24(sp)  # size of Matrix 
    sw s6, 28(sp)  # size of word


    #initialization
    mv s0, a0
    mv s1, a2
    mv s2, a3
    #  s3 is descriptor will be defined letter 
    mv s4, a1
    mul s5, s1, s2  # all elements row * cols
    li s6, 4
    li t0, 1        # Permission write only 1
    
    #opening file
    mv a1, s0
    mv a2, t0
    jal fopen
    li t0, -1
    beq a0, t0, fopen_failed
    mv s3, a0 # descriptor initialized
    
    
    
    #writing rows and cols
    # putting in stack
    addi sp, sp, -8
    sw s1, 0(sp)
    sw s2, 4(sp)
    # writing to file
    mv a1, s3
    mv a2, sp
    li a3, 2
    mv a4, s6
    jal fwrite
    li t0, 2
    bne a0, t0, fwrite_failed
    addi sp, sp, 8

    #writing data
    mv a1, s3
    mv a2, s4
    mv a3, s5
    mv a4, s6
    jal fwrite
    bne a0, s5, fwrite_failed

    # close file
    mv a1, s3
    jal fclose
    bne a0, x0, fclose_failed

    # Epilogue
    mv a0, s4
    lw ra, 0(sp)
    lw s0, 4(sp)   
    lw s1, 8(sp)   
    lw s2, 12(sp)  
    lw s3, 16(sp)  
    lw s4, 20(sp)  
    lw s5, 24(sp)  
    lw s6, 28(sp)  
    addi sp, sp, 32
    ret




fopen_failed:
    li a1, 89
    call exit2


fwrite_failed:
    li a1, 92
    call exit2

fclose_failed:
    li a1, 90
    call exit2