%include "asm/readINTEGER.asm" 
%include "asm/printINTEGER.asm" 

section .data

SRef0 :
db "Enter the first and the second number : "
SRef0_L : equ $-SRef0

SRef1 :
db "The first number is greater than the second number"
SRef1_L : equ $-SRef1

SRef2 :
db "The second number is greater than the first number"
SRef2_L : equ $-SRef2

SRef3 :
db "Both the numbers are equal !"
SRef3_L : equ $-SRef3

SRef4 :
db "Try entering two different numbers"
SRef4_L : equ $-SRef4

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

mov eax , dword [rbp - 4]
mov ebx , dword [rbp - 8]
cmp eax , ebx 
jg _BRANCH_0_IF
jmp _BRANCH_0_EXIT

_BRANCH_0_IF:
mov rax , 1
mov rdi , 1
mov rsi , SRef1
mov rdx , SRef1_L 
syscall 

call _newLine
jmp _BRANCH_0_EXIT
_BRANCH_0_EXIT:


mov eax , dword [rbp - 8]
mov ebx , dword [rbp - 4]
cmp eax , ebx 
jg _BRANCH_1_IF
jmp _BRANCH_1_EXIT

_BRANCH_1_IF:
mov rax , 1
mov rdi , 1
mov rsi , SRef2
mov rdx , SRef2_L 
syscall 

call _newLine
jmp _BRANCH_1_EXIT
_BRANCH_1_EXIT:


mov eax , dword [rbp - 4]
mov ebx , dword [rbp - 8]
cmp eax , ebx 
je _BRANCH_2_IF
jmp _BRANCH_2_EXIT

_BRANCH_2_IF:
mov rax , 1
mov rdi , 1
mov rsi , SRef3
mov rdx , SRef3_L 
syscall 

call _newLine
mov rax , 1
mov rdi , 1
mov rsi , SRef4
mov rdx , SRef4_L 
syscall 

call _newLine
jmp _BRANCH_2_EXIT
_BRANCH_2_EXIT:


add rsp , 8
mov rsp , rbp
pop rbp

mov rax , 60
mov rdi , 0
syscall
