%include "asm/printINTEGER.asm" 

section .data

SRef0 :
db "Welcome to the program!!" , 10 
SRef0_L : equ $-SRef0

SRef1 :
db "This is the values before the swap : " , 10 
SRef1_L : equ $-SRef1

SRef2 :
db "This is the values after the swap :" , 10 
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

sub rsp , 4
mov dword [rbp - 4] , 27
sub rsp , 4
mov dword [rbp - 8] , 141
mov rax , 1
mov rdi , 1
mov rsi , SRef1
mov rdx , SRef1_L 
syscall 

mov eax , dword [rbp - 4]
call _printINTEGER

mov eax , dword [rbp - 8]
call _printINTEGER

sub rsp , 4

mov eax , dword [rbp - 4]
mov dword [rbp - 12] , eax 


mov eax , dword [rbp - 8]
mov dword [rbp - 4] , eax 


mov eax , dword [rbp - 12]
mov dword [rbp - 8] , eax 

mov rax , 1
mov rdi , 1
mov rsi , SRef2
mov rdx , SRef2_L 
syscall 

mov eax , dword [rbp - 4]
call _printINTEGER

mov eax , dword [rbp - 8]
call _printINTEGER


add rsp , 12
mov rsp , rbp
pop rbp

mov rax , 60
mov rdi , 0
syscall