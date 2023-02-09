.globl classify

.text
classify:
    # =====================================
    # COMMAND LINE ARGUMENTS
    # =====================================
    # Args:
    #   a0 (int)    argc
    #   a1 (char**) argv
    #   a2 (int)    print_classification, if this is zero,
    #               you should print the classification. Otherwise,
    #               this function should not print ANYTHING.
    # Returns:
    #   a0 (int)    Classification
    # Exceptions:
    # - If there are an incorrect number of command line args,
    #   this function terminates the program with exit code 72
    # - If malloc fails, this function terminates the program with exit code 88
    #
    # Usage:
    #   main.s <M0_PATH> <M1_PATH> <INPUT_PATH> <OUTPUT_PATH>
    addi sp, sp, -52
    sw ra, 0(sp)
    sw s0, 4(sp)    # char ** to command arguments 
    sw s1, 8(sp)    # classification command, if zero print   
    sw s2, 12(sp)   # Ptr to memory for M0, int*  
    sw s3, 16(sp)   # ptr to memory for M1, int*
    sw s4, 20(sp)   # ptr to memory for input, int*
    sw s5, 24(sp)   # ptr to memory for m0 * input
    sw s6, 28(sp)   # rows h
    sw s7, 32(sp)   # cols h
    sw s8, 36(sp)   # ptr to memory Output = m1 * h
    sw s9, 40(sp)   # rows Output
    sw s10, 44(sp)  # cols Output
    sw s11, 48(sp)  # tmp

    #======================================
    # Initialization
    #======================================
    li t0, 5
    bne t0, a0, wrong_number_of_commands
    mv s0, a1
    mv s1, a2
    
	# =====================================
    # LOAD MATRICES
    # =====================================
    addi sp, sp, -24 
    # sp = [row_m0, col_m0, row_m1, col_m1, row_output, col_output]
    # s0 = ["main.s", "M0_PATH", "M1_PATH", "INPUT_PATH", "OUTPUT_PATH"]
    
    # Load pretrained m0
    addi t0, sp, 0
    addi t1, sp, 4
    lw  a0, 4(s0)
    mv a1, t0
    mv a2, t1
    jal read_matrix
    mv s2, a0

    # Load pretrained m1
    addi t0, sp, 8
    addi t1, sp, 12
    lw  a0, 8(s0)
    mv a1, t0
    mv a2, t1
    jal read_matrix
    mv s3, a0

    # Load input matrix
    addi t0, sp, 16
    addi t1, sp, 20
    lw  a0, 12(s0)
    mv a1, t0
    mv a2, t1
    jal read_matrix
    mv s4, a0


    # sp = [row_m0, col_m0, row_m1, col_m1, row_output, input_col]
    # =====================================
    # RUN LAYERS
    # =====================================
    # 1. LINEAR LAYER:    m0 * input
    # m0(mo_row, m0_col) * input(input_row, input_col) = result(m0_row, input_col)
    lw s6, 0(sp)
    lw s7, 20(sp)
    li t0, 4
    mul s11, s6, s7
    mul a0, s11, t0
    jal malloc
    beq a0, zero, malloc_failed
    mv s5, a0

    mv a0, s2 # a0 = ptr to M0
    lw a1, 0(sp)
    lw a2, 4(sp)
    mv a3, s4
    lw a4, 16(sp)
    lw a5, 20(sp)
    mv a6, s5
    jal matmul


    # 2. NONLINEAR LAYER: ReLU(m0 * input)
    mv a0, s5
    mv a1, s11
    jal relu
    # 3. LINEAR LAYER:    m1 * ReLU(m0 * input)
    # m1(m1_r, m1_col) * h(s6, s7) = output(m1_r, s7)
    lw s9, 8(sp)
    mv s10, s7
    mul s11, s9, s10
    li t0, 4
    mul a0, s11, t0
    jal malloc
    beq a0, zero, malloc_failed
    mv s8, a0

    mv a0, s3
    lw a1, 8(sp)
    lw a2, 12(sp)
    mv a3, s5
    mv a4, s6
    mv a5, s7
    mv a6, s8
    jal matmul


    # =====================================
    # WRITE OUTPUT
    # =====================================
    # Write output matrix
    lw a0, 16(s0)
    mv a1, s8
    mv a2, s9
    mv a3, s10
    jal write_matrix

    # =====================================
    # CALCULATE CLASSIFICATION/LABEL
    # =====================================
    # Call argmax
    mv a0, s8
    mul a1, s9, s10
    jal argmax
    mv s11, a0
    bne s1, x0, Done
    # Print classification
    
    mv a1, s11
    jal print_int
    # Print newline afterwards for clarity
    li a1, 10
    jal print_char

    jal Done
wrong_number_of_commands:
    addi a1, x0, 72
    call exit2

malloc_failed:
    addi a1, x0, 88
    call exit2

Done:
    mv a0, s2
    jal free

    mv a0, s3
    jal free

    mv a0, s4
    jal free

    mv a0, s5
    jal free

    mv a0, s8
    jal free

    mv a0, s11
    addi sp, sp, 24
    lw ra, 0(sp)
    lw s0, 4(sp)    # char ** to command arguments 
    lw s1, 8(sp)    # classification command, if zero print   
    lw s2, 12(sp)   # Ptr to memory for M0, int*  
    lw s3, 16(sp)   # ptr to memory for M1, int*
    lw s4, 20(sp)   # ptr to memory for input, int*
    lw s5, 24(sp)   # ptr to memory for m0 * input
    lw s6, 28(sp)   # rows h
    lw s7, 32(sp)   # cols h
    lw s8, 36(sp)   # ptr to memory Output = m1 * h
    lw s9, 40(sp)   # rows Output
    lw s10, 44(sp)  # cols Output
    lw s11, 48(sp)  # tmp
    addi sp, sp, 52
    jalr ra





  
    
    
