%include "asm/readINTEGER.asm" 
%include "asm/printINTEGER.asm" 

section .data

SRef0 :
db "Enter your age : "
SRef0_L : equ $-SRef0

SRef1 :
db "You are eligible for a voter pass !"
SRef1_L : equ $-SRef1

SRef2 :
db "But you are not eligible for drinking..."
SRef2_L : equ $-SRef2

SRef3 :
db "And you are eligible for drinking !"
SRef3_L : equ $-SRef3

SRef4 :
db "You are eligible neither for drinking nor for a voter pass..."
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

mov eax , dword [rbp - 4]
mov ebx , 18
cmp eax , ebx 
jge _BRANCH_0_IF
jmp _BRANCH_0_ELSE

_BRANCH_0_IF:
mov rax , 1
mov rdi , 1
mov rsi , SRef1
mov rdx , SRef1_L 
syscall 

call _newLine

mov eax , dword [rbp - 4]
mov ebx , 21
cmp eax , ebx 
jl _BRANCH_1_IF
jmp _BRANCH_1_ELSE

_BRANCH_1_IF:
mov rax , 1
mov rdi , 1
mov rsi , SRef2
mov rdx , SRef2_L 
syscall 

call _newLine
jmp _BRANCH_1_EXIT
_BRANCH_1_ELSE:
mov rax , 1
mov rdi , 1
mov rsi , SRef3
mov rdx , SRef3_L 
syscall 

call _newLine
jmp _BRANCH_1_EXIT

_BRANCH_1_EXIT:
jmp _BRANCH_0_EXIT
_BRANCH_0_ELSE:
mov rax , 1
mov rdi , 1
mov rsi , SRef4
mov rdx , SRef4_L 
syscall 

call _newLine
jmp _BRANCH_0_EXIT

_BRANCH_0_EXIT:

add rsp , 4
mov rsp , rbp
pop rbp

mov rax , 60
mov rdi , 0
syscall
