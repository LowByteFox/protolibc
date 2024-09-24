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

