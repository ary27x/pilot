section .data
nLBuffer:
db 10 
nLBuffer_L : equ $-nLBuffer

section .text

global _newLine
_newLine:


push rax
push rdx


mov rax , 1
mov rdi , 1
mov rsi , nLBuffer
mov rdx , nLBuffer_L
syscall

pop rdx
pop rax

ret