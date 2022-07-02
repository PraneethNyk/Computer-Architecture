addi $sp, $sp, -8    
sw $ra, 4($sp)
sw $a0, 0($sp)
slti $t0, $a0, 1
beq $t0, $zero, 3    
addi $v0, $zero, 1
addi $sp, $sp, 8
jr $ra
addi $a0, $a0, -1
jal 30               
lw $a0, 0($sp)
lw $t0, 4($sp)
addi $sp, $sp, 8
mult $v0, $a0, $v0
jr $ra