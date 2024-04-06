%include "asm/colors.asm"

section .text  

global _reset 
_reset : 
mov rax , 1
mov rdi , 1
mov rsi , __reset
mov rdx , __reset_L
syscall
ret  

global _black 
_black : 
mov rax , 1
mov rdi , 1
mov rsi , __black
mov rdx , __black_L
syscall
ret 

global _red 
_red : 
mov rax , 1
mov rdi , 1
mov rsi , __red
mov rdx , __red_L
syscall
ret  

global _green
_green :
mov rax , 1
mov rdi , 1
mov esi , __green
mov rdx , __green_L
syscall
ret


global _yellow
_yellow :
mov rax , 1
mov rdi , 1
mov esi , __yellow
mov rdx , __yellow_L
syscall
ret

global _blue
_blue :
mov rax , 1
mov rdi , 1
mov esi , __blue
mov rdx , __blue_L
syscall
ret

global _purple
_purple:
mov rax , 1
mov rdi , 1
mov esi , __purple
mov rdx , __purple_L
syscall
ret

global _cyan
_cyan:
mov rax , 1
mov rdi , 1
mov esi , __cyan
mov rdx , __cyan_L
syscall
ret

global _white
_white:
mov rax , 1
mov rdi , 1
mov esi , __white
mov rdx , __white_L
syscall
ret


; wrapper function calls for background colors

global _background_black
_background_black:
mov rax , 1
mov rdi , 1
mov esi , __background_black
mov rdx , __background_black_L
syscall
ret

global _background_red
_background_red:
mov rax , 1
mov rdi , 1
mov esi , __background_red
mov rdx , __background_red_L
syscall
ret

global _background_green
_background_green:
mov rax , 1
mov rdi , 1
mov esi , __background_green
mov rdx , __background_green_L
syscall
ret

global _background_yellow
_background_yellow:
mov rax , 1
mov rdi , 1
mov esi , __background_yellow
mov rdx , __background_yellow_L
syscall
ret


global _background_blue
_background_blue:
mov rax , 1
mov rdi , 1
mov esi , __background_blue
mov rdx , __background_blue_L
syscall
ret


global _background_purple
_background_purple:
mov rax , 1
mov rdi , 1
mov esi , __background_purple
mov rdx , __background_purple_L
syscall
ret

global _background_cyan
_background_cyan:
mov rax , 1
mov rdi , 1
mov esi , __background_cyan
mov rdx , __background_cyan_L
syscall
ret

global _background_white
_background_white:
mov rax , 1
mov rdi , 1
mov esi , __background_white
mov rdx , __background_white_L
syscall
ret

