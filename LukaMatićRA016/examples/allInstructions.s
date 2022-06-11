.globl main
.globl f1

.data
m1:		.word 17
m2:		.word 4
m3:		.word 1

.text
main:
	la		$t0, m3
	li		$t2, 0
	sw		$t2, 0($t0)
	li		$t2, 0
	la		$t0, m2
	lw		$t1, 0($t0)
	add		$t2, $t2, $t1
	addi	$t2, $t2, 7
	sub		$t2, $t2, $t1
	addi	$sp, $sp, -4
	sw		$ra, ($sp)
	jal		f1
	lw		$ra, ($sp)
	addiu	$sp, $sp, 4
	sub		$t2, $t2, $t1
	and		$t2, $t2, $t1
	jr		$ra
f1:
	and		$t2, $t2, $t1
	andi	$t2, $t2, 0
	b		lab
	la		$t0, m2
lab:
	la		$t0, m3
	li		$t2, 7
	sw		$t2, 0($t0)
	sub		$t2, $t2, $t1
	nop
	bltz	$t2, f1
	jr		$ra
