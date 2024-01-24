%include "asm/printRAX.asm"
%include "asm/newLine.asm"
%include "asm/mod.asm"
%include "asm/div.asm"

section .text

global _printINTEGER
_printINTEGER: ; THE ARGUMENT SHOULD BE IN THE RAX REGISTER BEFORE MAKING THE CALL 

push rbx
push rcx
push rdx

mov rbx , 10 ; we will be using the 10
; we will use the rdx for the counter , as the rcx register is used for the return values 

mov rdx , 0 ; we are setting up the counter

; we do the first iteration of the loop manually , so 
; as to we could also print the number : 0
call _mod
push rcx
add rdx , 1
call _div
mov rax , rcx

_primaryLoop:
cmp rax , 0
je _loopCallRAX 
call _mod
; we get the digit in the rcx register
; so we push it onto the stack and increment the counter register
; which in our case is the rdx register
push rcx
add rdx , 1
call _div ; the floor divison of rax // rbx should now be in rcx
mov rax , rcx ; we update the rax with the new value
jmp _primaryLoop ; looping back 



_loopCallRAX:
cmp rdx , 0
je _exit
pop rax
call _printRAX
sub rdx , 1
jmp _loopCallRAX


_exit:

pop rdx
pop rcx
pop rbx

ret



