.globl read_matrix

.text
# ==============================================================================
# FUNCTION: Allocates memory and reads in a binary file as a matrix of integers
#
# FILE FORMAT:
#   The first 8 bytes are two 4 byte ints representing the # of rows and columns
#   in the matrix. Every 4 bytes afterwards is an element of the matrix in
#   row-major order.
# Arguments:
#   a0 (char*) is the pointer to string representing the filename
#   a1 (int*)  is a pointer to an integer, we will set it to the number of rows
#   a2 (int*)  is a pointer to an integer, we will set it to the number of columns
# Returns:
#   a0 (int*)  is the pointer to the matrix in memory
# Exceptions:
# - If malloc returns an error,
#   this function terminates the program with error code 88
# - If you receive an fopen error or eof,
#   this function terminates the program with error code 89
# - If you receive an fclose error or eof,
#   this function terminates the program with error code 90
# - If you receive an fread error or eof,
#   this function terminates the program with error code 91
# ==============================================================================
read_matrix:

    # Prologue
    addi sp, sp, -32
    sw ra, 0(sp)
    sw s0, 4(sp)   # char* file name
    sw s1, 8(sp)   # int* row
    sw s2, 12(sp)  # int* col
    sw s3, 16(sp)  # int discriptor
    sw s4, 20(sp)  # int* buffer
    sw s5, 24(sp)  # size of Matrix in bytes
    sw s6, 28(sp)  # size of word

    # initialization 
    mv s0, a0
    mv s1, a1
    mv s2, a2
    li s6, 4

    #opening 
    mv a1, s0
    mv a2, x0
    jal fopen
    li t0, -1
    beq a0, t0, fopen_failed
    mv s3, a0

    #reading rows
    mv a1, s3
    mv a2, s1
    mv a3, s6
    jal fread
    bne a0, s6, fread_failed


    #reading cols
    mv a1, s3
    mv a2, s2
    mv a3, s6
    jal fread
    bne a0, s6, fread_failed
    lw t0, 0(s1)    # t0 = number of rows
    lw t1, 0(s2)    # t1 = number of cols
    mul s5, t0, t1  #size in elements (row X col)
    mul s5, s5, s6  # size in bytes (row X col X 4)

    #allocate space
    mv a0, s5
    jal malloc
    beq a0, x0, malloc_failed
    mv s4, a0

    #reading matrix
    mv a1, s3
    mv a2, s4
    mv a3, s5
    jal fread
    bne a0, s5, fread_failed


    #closing the file
    mv a1, s3
    jal fclose
    bne a0, x0, fclose_failed
    

    # Epilogue
    mv a0, s4
    lw ra, 0(sp)
    lw s0, 4(sp)   # char* file name
    lw s1, 8(sp)   # int* row
    lw s2, 12(sp)  # int* col
    lw s3, 16(sp)  # int discriptor
    lw s4, 20(sp)  # int* buffer
    lw s5, 24(sp)  # size of Matrix in bytes
    lw s6, 28(sp)  # size of word
    addi sp, sp, 32
    ret


fopen_failed:
    li a1, 89
    call exit2


malloc_failed:
    li a1, 88
    call exit2

fread_failed:
    li a1, 91
    call exit2

fclose_failed:
    li a1, 90
    call exit2