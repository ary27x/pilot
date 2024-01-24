section .bss
input_buffer resb 10


section .text

global _readINTEGER
_readINTEGER:


; we need to preserve the register values

push rdi 
push rsi 
push rdx
push rcx 
push rbx


mov rax , 0 
mov rdi , 0
mov rsi , input_buffer
mov rdx , 10
syscall


mov rdi , 10
mov rax , 0
mov rdx , 0
mov rcx , input_buffer

_readINTERGER_loop:

movzx rbx , byte [rcx]
cmp rbx , 57
jg _readINTERGER_exit
cmp rbx , 48
jl _readINTERGER_exit


sub rbx , 48
mul rdi
add rax , rbx

inc rcx
jmp _readINTERGER_loop


_readINTERGER_exit:
;restoring the register values
pop rbx
pop rcx
pop rdx
pop rsi 
pop rdi 

ret





