.globl abs

.text
# =================================================================
# FUNCTION: Given an int return its absolute value.
# Arguments:
# 	a0 (int) is input integer
# Returns:
#	a0 (int) the absolute value of the input
# =================================================================
abs:
	bgt x0, a0, ret_abs
    ret
    
ret_abs:
	sub a0, x0, a0
    ret