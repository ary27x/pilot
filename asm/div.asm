section .text

global _div
_div:
; performs floor operation and then returns the result
; for the operation num1 // num2  = num3
; num1 is rax (input)
; num2 is rbx (input)
; num3 is rcx (output)

push rax
push rbx
push rcx
push rdx


mov rcx , 0
mov rdx , rbx

_div_loop:
cmp rbx , rax
jg _div_exit
add rcx , 1
add rbx , rdx
jmp _div_loop

_div_exit:

mov rsi , rcx
pop rdx 
pop rcx
pop rbx
pop rax
mov rcx , rsi

ret