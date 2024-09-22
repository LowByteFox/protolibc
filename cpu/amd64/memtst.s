.global memtst_back
memtst_back:
	mov %rbp, %rax
bt_up:
    cmp $0, %rdi
	jle bt_out
	test %rax, %rax
	jz bt_out
	mov (%rax), %rax
	dec %rdi
	jmp bt_up
bt_out:
    test %rax, %rax
	jz bt_ret
	add	$8, %rax
	mov (%rax), %rax
bt_ret:
    ret
