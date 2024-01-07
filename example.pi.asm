%include "asm/printINTEGER.asm" 

section .data

SRef0 :
db "these are the values before the swap :"
SRef0_L : equ $-SRef0

SRef1 :
db "First : "
SRef1_L : equ $-SRef1

SRef2 :
db " Second : "
SRef2_L : equ $-SRef2

SRef3 :
db "these are the values after the swap :"
SRef3_L : equ $-SRef3

section .text

global _start
_start:

push rbp
mov rbp , rsp

sub rsp , 4
mov dword [rbp - 4] , 7
sub rsp , 4
mov dword [rbp - 8] , 14
mov rax , 1
mov rdi , 1
mov rsi , SRef0
mov rdx , SRef0_L 
syscall 

call _newLine
mov rax , 1
mov rdi , 1
mov rsi , SRef1
mov rdx , SRef1_L 
syscall 

mov eax , dword [rbp - 4]
call _printINTEGER

mov rax , 1
mov rdi , 1
mov rsi , SRef2
mov rdx , SRef2_L 
syscall 

mov eax , dword [rbp - 8]
call _printINTEGER

call _newLine
sub rsp , 4

mov eax , dword [rbp - 4]
mov dword [rbp - 12] , eax 


mov eax , dword [rbp - 8]
mov dword [rbp - 4] , eax 


mov eax , dword [rbp - 12]
mov dword [rbp - 8] , eax 

mov rax , 1
mov rdi , 1
mov rsi , SRef3
mov rdx , SRef3_L 
syscall 

call _newLine
mov rax , 1
mov rdi , 1
mov rsi , SRef1
mov rdx , SRef1_L 
syscall 

mov eax , dword [rbp - 4]
call _printINTEGER

mov rax , 1
mov rdi , 1
mov rsi , SRef2
mov rdx , SRef2_L 
syscall 

mov eax , dword [rbp - 8]
call _printINTEGER

call _newLine

add rsp , 12
mov rsp , rbp
pop rbp

mov rax , 60
mov rdi , 0
syscall
