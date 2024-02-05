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

; we need to first step kinda manually 
; that is , if the first byte as the address has -
; then we would set the negative flag register (maybe rdx) to 1
; (initially the register should have the value 0)
; and then we would increment the pointer to the next value 
; and at the end , when we are returning from the function
; if the negative flag register is set , we would : 
; imul rax , -1
; and then return 
; and we have to add parsing support for negative number variables
; something like : number is -7

mov rdx , 0 ; rdx register acts as the negative number flag
movzx rbx , byte [rcx]
cmp rbx , 45
jne _readINTEGER_wrapper
mov rdx , 1

; i think the value of the rdx regsiter 
; is somehow getting corrupted
inc rcx

_readINTEGER_wrapper:
push rdx ; we are saving the value of the rdx register

_readINTEGER_loop:

movzx rbx , byte [rcx]
cmp rbx , 57
jg _readINTEGER_exit
cmp rbx , 48
jl _readINTEGER_exit


sub rbx , 48
mul rdi
add rax , rbx

inc rcx
jmp _readINTEGER_loop


_readINTEGER_exit:
; checking the status of the negative flag
;mov rdx , 1
pop rdx
cmp rdx , 1
jne _readINTEGER_EXIT_RESTORE
;imul rax , -1 
not rax
add eax , 1
;mov eax , 0
_readINTEGER_EXIT_RESTORE:
;restoring the register values
pop rbx
pop rcx
pop rdx
pop rsi 
pop rdi 

ret





