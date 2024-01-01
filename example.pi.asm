section .data

SRef0 :
db "hello there!"
SRef0_L : equ $-SRef0

SRef1 :
db "this is a string..."
SRef1_L : equ $-SRef1

section .text

global _start
_start:

mov rax , 1
mov rdi , 1
mov rsi , SRef0
mov rdx , SRef0_L 
syscall 

mov rax , 1
mov rdi , 1
mov rsi , SRef1
mov rdx , SRef1_L 
syscall 

mov rax , 60
mov rdi , 7
syscall
