section .text

global _mod
_mod:
; rax % rbx = rcx (the output value would be automatically put into the rcx register)

push rax
push rbx
push rcx
push rdx



mov rdx , rbx

_mod_loop:
cmp rax , rbx
jl _mod_lessThanCase
mov rcx , rax
sub rcx , rbx
cmp rcx , rdx
jl _mod_exit
add rbx , rdx
jmp _mod_loop


_mod_exit:

mov rsi , rcx
pop rdx 
pop rcx
pop rbx
pop rax
mov rcx , rsi
ret

_mod_lessThanCase:
mov rcx , rax
jmp _mod_exit
