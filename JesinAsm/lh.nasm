; vim:ft=nasm
bits 64
global _start:function
%define SYS_execve (59)
%define SYS_exit (60)


section .text
_start:
	pop rdx	;old argc
	pop rax	;discard old argv[0]
	lea rdx, [rsp + 8*rdx]	;environ
	mov eax, argv2
	push rax
	sub al, argv2 - argv1
	push rax
	sub al, argv1 - argv0
	push rax
	sub al, argv0 - progPath
	xchg edi, eax
	mov rsi, rsp
	push SYS_execve
	pop rax
	syscall
	neg eax
	test al, al
	xchg edi, eax
	push SYS_exit
	pop rax
	cmovz edi, eax
	syscall

progPath: db '/usr/bin/'
argv0: db 'ls',0
argv1: db '--color=auto',0
argv2: db '-AFbhl',0
