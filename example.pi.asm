section .data

SRef0 :
db "Welcome to the program!"
SRef0_L : equ $-SRef0

SRef1 :
db "this is the end of the program !"
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

sub rsp , 4
mov dword [rbp - 4] , 25
sub rsp , 4
mov dword [rbp - 8] , 943
mov dword [rbp - 4] , 943
mov dword [rbp - 8] , 25
mov rax , 1
mov rdi , 1
mov rsi , SRef1
mov rdx , SRef1_L 
syscall 


add rsp , 8
mov rsp , rbp
pop rbp

mov rax , 60
mov rdi , 4
syscall
