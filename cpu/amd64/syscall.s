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

.global _exit
_exit:
	mov	$60, %rax
	jmp	__syscall

.global fork
fork:
	mov	$57, %eax
	jmp	__syscall

.global read
read:
	mov	$0, %eax
	jmp	__syscall

.global write
write:
	mov	$1, %eax
	jmp	__syscall

.global open
open:
	mov	$2, %eax
	jmp	__syscall

.global close
close:
	mov	$3, %eax
	jmp	__syscall

.global waitpid
waitpid:
	xor	%rcx, %rcx
	mov	$61, %eax
	jmp	__syscall

.global creat
creat:
	mov	$85, %eax
	jmp	__syscall

.global link
link:
	mov	$86, %eax
	jmp	__syscall

.global unlink
unlink:
	mov	$87, %eax
	jmp	__syscall

.global execve
execve:
	mov	$59, %eax
	jmp	__syscall

.global chdir
chdir:
	mov	$80, %eax
	jmp	__syscall

.global time
time:
	mov	$201, %eax
	jmp	__syscall

.global mknod
mknod:
	mov	$133, %eax
	jmp	__syscall

.global chmod
chmod:
	mov	$90, %eax
	jmp	__syscall

.global lseek
lseek:
	mov	$8, %eax
	jmp	__syscall

.global getpid
getpid:
	mov	$39, %eax
	jmp	__syscall

.global mount
mount:
	mov	$165, %eax
	jmp	__syscall

.global umount
umount:
	xor	%rsi, %rsi
	mov	$166, %eax
	jmp	__syscall

.global setuid
setuid:
	mov	$105, %eax
	jmp	__syscall

.global getuid
getuid:
	mov	$102, %eax
	jmp	__syscall

.global utime
utime:
	mov	$132, %eax
	jmp	__syscall

.global access
access:
	mov	$21, %eax
	jmp	__syscall

.global sync
sync:
	mov	$162, %eax
	jmp	__syscall

.global kill
kill:
	mov	$62, %eax
	jmp	__syscall

.global mkdir
mkdir:
	mov	$83, %eax
	jmp	__syscall

.global rmdir
rmdir:
	mov	$84, %eax
	jmp	__syscall

.global __dup
.global dup
__dup:
dup:
	mov	$32, %eax
	jmp	__syscall

.global pipe
pipe:
	mov	$22, %eax
	jmp	__syscall

.global brk
brk:
	mov	$12, %eax
	jmp	__syscall

.global setgid
setgid:
	mov	$106, %eax
	jmp	__syscall

.global getgid
getgid:
	mov	$104, %eax
	jmp	__syscall

.global geteuid
geteuid:
	mov	$107, %eax
	jmp	__syscall

.global getegid
getegid:
	mov	$108, %eax
	jmp	__syscall

.global ioctl
ioctl:
	mov	$16, %eax
	jmp	__syscall

.global fcntl
fcntl:
	mov	$72, %eax
	jmp	__syscall

.global dup2
dup2:
	mov	$33, %eax
	jmp	__syscall

.global getppid
getppid:
	mov	$110, %eax
	jmp	__syscall

.global setsid
setsid:
	mov	$124, %eax
	jmp	__syscall

.global gettimeofday
gettimeofday:
	mov	$96, %eax
	jmp	__syscall

.global settimeofday
settimeofday:
	mov	$164, %eax
	jmp	__syscall

.global mmap
mmap:
	mov	$9, %eax
	jmp	__syscall

.global munmap
munmap:
	mov	$11, %eax
	jmp	__syscall

.global stat
stat:
	mov	$4, %eax
	jmp	__syscall

.global lstat
lstat:
	mov	$6, %eax
	jmp	__syscall

.global fstat
fstat:
	mov	$5, %eax
	jmp	__syscall

.global clone
clone:
	mov	$56, %eax
	jmp	__syscall

.global uname
uname:
	mov	$63, %eax
	jmp	__syscall

.global fchdir
fchdir:
	mov	$81, %eax
	jmp	__syscall

.global rename
rename:
	mov	$82, %eax
	jmp	__syscall

.global getdents
getdents:
	mov	$78, %eax
	jmp	__syscall

.global nanosleep
nanosleep:
	mov	$35, %eax
	jmp	__syscall

.global poll
poll:
	mov	$7, %eax
	jmp	__syscall

.global chown
chown:
	mov	$92, %eax
	jmp	__syscall

.global getcwd
getcwd:
	mov	$79, %eax
	jmp	__syscall

.global sigaction
sigaction:
	mov	$13, %eax
	jmp	__syscall

.global sigreturn
sigreturn:
	mov	$15, %eax
	jmp	__syscall

.global fsync
fsync:
	mov	$74, %eax
	jmp	__syscall

.global fdatasync
fdatasync:
	mov	$75, %eax
	jmp	__syscall

.global truncate
truncate:
	mov	$76, %eax
	jmp	__syscall

.global ftruncate
ftruncate:
	mov	$77, %eax
	jmp	__syscall
