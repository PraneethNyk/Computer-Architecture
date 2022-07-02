bneq $s5, $zero, 2   # branch k!=$zero
add $s0, $s3, $s4  # k==$zero so f=i+j
j 11          # end of case so Exit
add $s0, $s1, $s2  # k==1 so f=g+h
addi $t0, $s5, -1 # $t0 = k-1
bneq $t0, $zero, 2   # branch k != 1
j 7          # end of case so Exit
addi $t0, $s5, -2 # $t0=k-2
bneq $t0, $zero, 2   # branch k != 2
sub $s0, $s1, $s2  # k==2 so f=g-h
j 3          # end of case so Exit
addi $t0, $s5, -3 # $t0 = k-3
bneq $t0, $zero, 1 # branch k != 3
sub $s0, $s3, $s4  # k==3 so f=i-j