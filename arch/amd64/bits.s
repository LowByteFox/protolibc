.global htonl
.global ntohl
htonl:
ntohl:
	mov %edi, %eax
	xchg %al, %ah
	ror $16, %eax
	xchg %al, %ah
	ret

.global htons
.global ntohs
htons:
ntohs:
	mov	%edi, %eax
	ror	$8, %ax
	ret
