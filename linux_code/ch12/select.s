	.file	"select.c"
	.text
	.section	.rodata
.LC0:
	.string	"select start"
.LC1:
	.string	"select() error!"
.LC2:
	.string	"Time-out!"
.LC3:
	.string	"message from console is %s \n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$352, %rsp
	movl	%edi, -340(%rbp)
	movq	%rsi, -352(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movl	$0, %eax
	movl	$16, %ecx
	leaq	-304(%rbp), %rdx
	movq	%rdx, %rdi
#APP
# 14 "select.c" 1
	cld; rep; stosq
# 0 "" 2
#NO_APP
	movl	%edi, %eax
	movl	%ecx, %edx
	movl	%edx, -336(%rbp)
	movl	%eax, -332(%rbp)
	movq	-304(%rbp), %rax
	orq	$1, %rax
	movq	%rax, -304(%rbp)
	movq	$5, -320(%rbp)
	movq	$0, -312(%rbp)
.L6:
	movq	-304(%rbp), %rax
	movq	-296(%rbp), %rdx
	movq	%rax, -176(%rbp)
	movq	%rdx, -168(%rbp)
	movq	-288(%rbp), %rax
	movq	-280(%rbp), %rdx
	movq	%rax, -160(%rbp)
	movq	%rdx, -152(%rbp)
	movq	-272(%rbp), %rax
	movq	-264(%rbp), %rdx
	movq	%rax, -144(%rbp)
	movq	%rdx, -136(%rbp)
	movq	-256(%rbp), %rax
	movq	-248(%rbp), %rdx
	movq	%rax, -128(%rbp)
	movq	%rdx, -120(%rbp)
	movq	-240(%rbp), %rax
	movq	-232(%rbp), %rdx
	movq	%rax, -112(%rbp)
	movq	%rdx, -104(%rbp)
	movq	-224(%rbp), %rax
	movq	-216(%rbp), %rdx
	movq	%rax, -96(%rbp)
	movq	%rdx, -88(%rbp)
	movq	-208(%rbp), %rax
	movq	-200(%rbp), %rdx
	movq	%rax, -80(%rbp)
	movq	%rdx, -72(%rbp)
	movq	-192(%rbp), %rax
	movq	-184(%rbp), %rdx
	movq	%rax, -64(%rbp)
	movq	%rdx, -56(%rbp)
	movl	$5, %edi
	call	sleep@PLT
	leaq	.LC0(%rip), %rdi
	call	puts@PLT
	leaq	-320(%rbp), %rdx
	leaq	-176(%rbp), %rax
	movq	%rdx, %r8
	movl	$0, %ecx
	movl	$0, %edx
	movq	%rax, %rsi
	movl	$1, %edi
	call	select@PLT
	movl	%eax, -328(%rbp)
	cmpl	$-1, -328(%rbp)
	jne	.L2
	leaq	.LC1(%rip), %rdi
	call	puts@PLT
	nop
	movl	$0, %eax
	movq	-8(%rbp), %rsi
	xorq	%fs:40, %rsi
	je	.L8
	jmp	.L9
.L2:
	cmpl	$0, -328(%rbp)
	jne	.L4
	leaq	.LC2(%rip), %rdi
	call	puts@PLT
	jmp	.L6
.L4:
	movq	-176(%rbp), %rax
	andl	$1, %eax
	testq	%rax, %rax
	je	.L6
	leaq	-48(%rbp), %rax
	movl	$30, %edx
	movq	%rax, %rsi
	movl	$0, %edi
	call	read@PLT
	movl	%eax, -324(%rbp)
	movl	-324(%rbp), %eax
	cltq
	movb	$0, -48(%rbp,%rax)
	leaq	-48(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC3(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L6
.L9:
	call	__stack_chk_fail@PLT
.L8:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 9.4.0-1ubuntu1~20.04.2) 9.4.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 8
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 8
4:
