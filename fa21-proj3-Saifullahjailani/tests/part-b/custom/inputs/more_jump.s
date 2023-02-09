addi t0, x0, 100
sub t0, x0, x0
slti t0, t0, 1
jalr x0, x0,28
labe2:
	addi t0, x0, 0
    addi s0, x0, -100
    jal end
label2:
	jalr x0, x0, 16
    
end:
	addi t0, x0, 100
    


	
