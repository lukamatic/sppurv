.globl main
.globl f1
.globl f2

.data
m1:		.word 3
m2:		.word 4

.text
main:
	la		$t0, m1
	lw		$t0, 0($t0)
	addi	$t0, $t0, 4
	addi	$sp, $sp, -4
	sw		$ra, ($sp)
	jal		f2
	lw		$ra, ($sp)
	addiu	$sp, $sp, 4
	add		$t0, $t0, $t0
	jr		$ra
f1:
	la		$t0, m1
	lw		$t0, 4($t0)
	addi	$t0, $t0, 4
	jr		$ra
f2:
	la		$t0, m1
	lw		$t0, 0($t0)
	addi	$t0, $t0, 4
	addi	$sp, $sp, -4
	sw		$ra, ($sp)
	jal		f1
	lw		$ra, ($sp)
	addiu	$sp, $sp, 4
	addi	$t0, $t0, 7
	jr		$ra
