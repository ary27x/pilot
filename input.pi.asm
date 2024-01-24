%include "asm/readINTEGER.asm" 
%include "asm/printINTEGER.asm" 

section .data

SRef0 :
db "Enter any two numbers : "
SRef0_L : equ $-SRef0

SRef1 :
db "This is the first number : "
SRef1_L : equ $-SRef1

SRef2 :
db " and this is the second number : "
SRef2_L : equ $-SRef2

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

add rsp , 8
mov rsp , rbp
pop rbp

mov rax , 60
mov rdi , 0
syscall
