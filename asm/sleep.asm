section .text

global _sleep
_sleep: 

; parameters : eax register -> sleep time (in seconds)

; saving the register values
push rdi 
push rsi 
push rdx
push rcx 
push rbx

mov rcx , rax ; we put the sleep time in the rcx register
call _getCURRENT_TIME ; we get the starting time
mov rbx , rax 

_time_loop: 
call _getCURRENT_TIME
sub rax , rbx
cmp rax , rcx
jge _time_loop_exit
jmp _time_loop

_time_loop_exit:

;restoring the register values
pop rbx
pop rcx
pop rdx
pop rsi 
pop rdi 

ret

_getCURRENT_TIME: ; this would put the current time in the rax register
push rcx
mov rax , 201
xor rdi , rdi
syscall
pop rcx
ret



