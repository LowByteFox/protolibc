.global __memcpylong
__memcpylong:
	mov	%rdi, %rax
	mov	%rdx, %rcx
	shr	$3, %ecx
	cld
	rep movsq
	ret

.global __memsetlong
__memsetlong:
	mov	%rdx, %rcx
	mov	%rsi, %rax
	mov	%rdi, %rdx
	shr	$3, %ecx
    cld
	rep stosq
	mov	%rdx, %rax
	ret
