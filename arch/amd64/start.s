.global _start
_start:
	xor %rbp, %rbp
	pop %rdi
	mov %rsp, %rsi
	push %rdi
    lea 8(%rsi,%rdi,8), %rdx
	mov	%rdx, environ
	and $-16, %rsp

	call __libc_start_main
	mov %rax, %rbx
	call __neatlibc_exit
	mov %rbx, %rdi
	mov $60, %eax
	syscall
