section .data
integerBuffer:
db 48 
integerBuffer_L : equ $-integerBuffer

section .text

global _printRAX
_printRAX:

push rax
push rdi 
push rsi 
push rdx
add [integerBuffer] , rax

mov rax , 1
mov rdi , 1
mov rsi , integerBuffer
mov rdx , integerBuffer_L
syscall

pop rdx
pop rsi
pop rdi
pop rax
sub [integerBuffer] , rax
ret
