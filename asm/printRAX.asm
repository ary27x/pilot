section .data
integerBuffer:
db 48 
integerBuffer_L : equ $-integerBuffer

section .text

global _printRAX
_printRAX:



push rax
push rdx
add [integerBuffer] , rax

mov rax , 1
mov rdi , 1
mov rsi , integerBuffer
mov rdx , integerBuffer_L
syscall

pop rdx
pop rax
sub [integerBuffer] , rax
ret