addi t0, x0, -1
bgeu t0, x0, done

dead_end:
	add t0, x0, x0
	beq t0, x0, go
done:
	addi t0, x0, 100
	addi t1, x0, -200
    bltu t0, t1, dead_end
yoho:
	blt x0, t0, hoho
come:
	addi t0, t0, -100
    beq t0, x0, end
go:
	addi t0, t0, 100
    bne t0, x0, yoho
hoho:
	bge t0, x0, come
end:
