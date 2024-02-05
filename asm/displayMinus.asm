section .data
minusBuffer:
db 45 
minusBuffer_L : equ $-minusBuffer

section .text

global _displayMinus
_displayMinus:

push rax
push rdi 
push rsi 
push rdx

mov rax , 1
mov rdi , 1
mov rsi , minusBuffer
mov rdx , minusBuffer_L
syscall

pop rdx
pop rsi
pop rdi
pop rax

ret
