__syscall:
    mov	%rcx, %r10
	syscall

	cmp	$0, %rax
	jge done
	cmp	$-4095, %rax
	jb done
	neg	%rax
	mov	%eax, (errno)
	mov	$-1, %rax
done:
	ret

# INFO: Pledge doesn't affect this one 
.global _exit
_exit:
	mov	$60, %rax
	jmp	__syscall

# INFO: Not sure what to do with this
.global brk
brk:
	mov	$12, %eax
	jmp	__syscall

# INFO: This syscall cannot be generated
.global mmap
mmap:
    push %r9
    push %r8
    push %r10
    push %rdx
    push %rsi
    push %rdi
    push %rcx
    mov $9, %eax
    mov %rax, %rsi
    mov $1, %eax
    mov %rax, %rdi
    call __pledge_check
    pop %rcx
    pop %rdi
    pop %rsi
    pop %rdx
    pop %r10
    pop %r8
    pop %r9
    mov $9, %eax
    jmp __syscall
