addi t0, zero, 0x100  # pa
lui t1, 0x1800 # pc
srli t1, t1, 12
addi t3, zero, 0 #y  cycle
addi s2, zero, 64 #M
addi s3, zero, 60 #N
addi s4, zero,  32 #K

addi t2, zero,  0 #x cycle
lui t4, 0x900 #pb
srli t4, t4, 12
addi t5, zero,  0 #s
addi t6, zero,  0 #k cycle

addi s5, zero, 0
add s5, t0, t6
lb a3, 0, s5 #pa[k]
add s8, t2, t2 #2x
add s9 , t4, s8 #адрес [x]
lh a4, 0, s9 #pb[x]
mul a2, a3, a4 # pa[k]*pb[x]
add t5, t5, a2 # s+=...
addi t4, t4, 120 # pb += N
addi t6, t6, 1 #k++
blt t6, s4, -40
addi a5, zero, 4
mul a6, t2, a5
add a5, t1, a6 # pc[x]
sw t5, 0, a5 # pc[x] = s;
addi t2, t2, 1 #x++
blt t2, s3, -80
addi t0, t0, 32 # pa += K;
addi t1, t1, 240 # pc += N;
addi t3, t3, 1 #y++
blt t3, s2, -100