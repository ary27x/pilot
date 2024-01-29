%include "asm/readINTEGER.asm" 
%include "asm/printINTEGER.asm" 

section .data

SRef0 :
db "Enter the lower and the upper limit of the range : "
SRef0_L : equ $-SRef0

SRef1 :
db "This following is a prime number : "
SRef1_L : equ $-SRef1

section .text

global _start
_start:

push rbp
mov rbp , rsp

mov rax , 1
mov rdi , 1
mov rsi , SRef0
mov rdx , SRef0_L 
syscall 

call _newLine
sub rsp , 4
call _readINTEGER
mov dword [rbp - 4 ] , eax
sub rsp , 4
call _readINTEGER
mov dword [rbp - 8 ] , eax
sub rsp , 4
jmp _LOOP_0_ENTRY 
_LOOP_0_ENTRY: 
mov eax , dword [ rbp - 4 ]
mov dword [rbp - 12] , eax
mov eax , dword [ rbp - 4 ] 
mov ebx , dword [ rbp - 8 ] 
cmp rax , rbx
jg _LOOP_0_EXIT 
sub rsp , 4
mov dword [rbp - 16] , 0
sub rsp , 4
sub rsp , 4
mov dword [ rbp - 24 ] , 1
jmp _LOOP_1_ENTRY 
_LOOP_1_ENTRY: 
mov eax , dword [ rbp - 24 ]
mov dword [rbp - 20] , eax
mov eax , dword [ rbp - 24 ] 
mov ebx , dword [ rbp - 12 ] 
cmp rax , rbx
jg _LOOP_1_EXIT 

mov eax , dword [rbp - 12]
push rax
mov eax , dword [rbp - 20]
push rax
pop rbx 
pop rax 
call _mod 
 push rcx
pop rax
push rax
push 0
pop rbx
pop rax
cmp eax , ebx 
je _BRANCH_0_IF
jmp _BRANCH_0_ELSE

_BRANCH_0_IF:
mov eax , dword [rbp - 16]
push rax
push 1
pop rbx 
pop rax 
add rax , rbx 
push rax
pop rax
mov dword [rbp - 16] , eax 
jmp _BRANCH_0_EXIT
_BRANCH_0_ELSE:
jmp _BRANCH_0_EXIT

_BRANCH_0_EXIT:
add dword [ rbp - 24 ] , 1
jmp _LOOP_1_ENTRY 
_LOOP_1_EXIT: 

mov eax , dword [rbp - 16]
push rax
push 2
pop rbx
pop rax
cmp eax , ebx 
je _BRANCH_1_IF
jmp _BRANCH_1_ELSE

_BRANCH_1_IF:
mov rax , 1
mov rdi , 1
mov rsi , SRef1
mov rdx , SRef1_L 
syscall 

mov eax , dword [rbp - 12]
call _printINTEGER

call _newLine
jmp _BRANCH_1_EXIT
_BRANCH_1_ELSE:
jmp _BRANCH_1_EXIT

_BRANCH_1_EXIT:
add dword [ rbp - 4 ] , 1
jmp _LOOP_0_ENTRY 
_LOOP_0_EXIT: 

add rsp , 24
mov rsp , rbp
pop rbp

mov rax , 60
mov rdi , 0
syscall
