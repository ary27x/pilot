%include "asm/readINTEGER.asm" 
%include "asm/printINTEGER.asm" 

%include "asm/colorWrapper.asm" 

%include "asm/sleep.asm" 

section .data

@pilot_clear :
db `\033[2J\033[H`
@pilot_clear_L : equ $-@pilot_clear

@Sref0 :
db " "
@Sref0_L : equ $-@Sref0

@Sref1 :
db ""
@Sref1_L : equ $-@Sref1

@Sref2 :
db "  ^  "
@Sref2_L : equ $-@Sref2

@Sref3 :
db "[RWH]"
@Sref3_L : equ $-@Sref3

@Sref4 :
db "-------------------------"
@Sref4_L : equ $-@Sref4

@Sref5 :
db "[*] Case 0 : "
@Sref5_L : equ $-@Sref5

@Sref6 :
db "| Write | Shift |  Row  |"
@Sref6_L : equ $-@Sref6

@Sref7 :
db "[*] Case 1 : "
@Sref7_L : equ $-@Sref7

@Sref8 :
db "    ---->    "
@Sref8_L : equ $-@Sref8

@Sref9 :
db "|"
@Sref9_L : equ $-@Sref9

@Sref10 :
db "   "
@Sref10_L : equ $-@Sref10

@Sref11 :
db "   |"
@Sref11_L : equ $-@Sref11

@Sref12 :
db "-"
@Sref12_L : equ $-@Sref12

@Sref13 :
db " |"
@Sref13_L : equ $-@Sref13

@Sref14 :
db "[~] Turing Machine Simulator "
@Sref14_L : equ $-@Sref14

@Sref15 :
db "[*] Tape Input : "
@Sref15_L : equ $-@Sref15

@Sref16 :
db "[*] Total Operations : "
@Sref16_L : equ $-@Sref16

@Sref17 :
db "[!] Current RW_Head Offset : "
@Sref17_L : equ $-@Sref17

@Sref18 :
db "[!] Current Tape State : "
@Sref18_L : equ $-@Sref18

@Sref19 :
db "[!] Machine Halted At Offset Number : "
@Sref19_L : equ $-@Sref19

@Sref20 :
db "[*] Tape Output -> "
@Sref20_L : equ $-@Sref20

section .bss 

tape : resd 10

INPUT_tape : resd 10

state_dictionary : resd 60

section .text

global _start
_start:

push rbp
mov rbp , rsp

call _white

mov ebx , tape
mov eax , 0
mov [ebx] , eax
add ebx , 4
mov ebx , INPUT_tape
mov eax , 0
mov [ebx] , eax
add ebx , 4
mov ebx , state_dictionary
mov eax , 0
mov [ebx] , eax
add ebx , 4
sub rsp , 4
mov dword [rbp - 4] , 0
sub rsp , 4
mov dword [rbp - 8] , 5
sub rsp , 4
mov dword [rbp - 12] , 3
sub rsp , 4
mov dword [rbp - 16] , 1
sub rsp , 4
mov dword [rbp - 20] , 0
sub rsp , 4
mov dword [rbp - 24] , 0
sub rsp , 4

mov eax , dword [rbp - 16]
mov dword [rbp - 28] , eax 

call initializeTape
call initializeStateDictionary
_TILL_0_ENTRY:

mov eax , dword [rbp - 28]
push rax
mov eax , dword [rbp - 16]
push rax
pop rbx
pop rax
cmp eax , ebx 
je _TILL_0_MAIN
jmp _TILL_0_EXIT

_TILL_0_MAIN:
mov rax , 1
mov rdi , 1
mov rsi , @pilot_clear
mov rdx , @pilot_clear_L
syscall 

mov eax , dword [rbp - 12]
mov rdi , rax
mov eax , dword [rbp - 8]
mov rsi , rax
mov eax , dword [rbp - 4]
mov rdx , rax
mov eax , dword [rbp - 24]
mov rcx , rax
call render

mov eax , dword [rbp - 12]
push rax
push 9
pop rbx
pop rax
cmp eax , ebx 
je _BRANCH_32_IF
jmp _BRANCH_32_ELSE

_BRANCH_32_IF:

mov eax , dword [rbp - 20]
mov dword [rbp - 28] , eax 

jmp _BRANCH_32_EXIT
_BRANCH_32_ELSE:
sub rsp , 4
mov ecx , 0
mov eax , dword [rbp - 4]
mov ebx , 30
imul rax , rbx 
add ecx , eax 
mov eax , dword [rbp - 8]
mov ebx , 3
imul rax , rbx 
add ecx , eax 
mov eax , 2
mov ebx , 1
imul rax , rbx 
add ecx , eax 
mov eax , ecx 
push rax 
pop rax 
mov rbx , rax
mov eax , [state_dictionary + ebx * 4] 
mov dword [rbp - 32] , eax 

sub rsp , 4
mov ecx , 0
mov eax , dword [rbp - 12]
mov ebx , 1
imul rax , rbx 
add ecx , eax 
mov eax , ecx 
push rax 
pop rax 
mov rbx , rax
mov eax , [tape + ebx * 4] 
mov dword [rbp - 36] , eax 

mov ecx , 0
mov eax , dword [rbp - 12]
mov ebx , 1
imul rax , rbx 
add ecx , eax 
mov eax , ecx 
push rax 
mov ecx , 0
mov eax , dword [rbp - 4]
mov ebx , 30
imul rax , rbx 
add ecx , eax 
mov eax , dword [rbp - 8]
mov ebx , 3
imul rax , rbx 
add ecx , eax 
mov eax , 0
mov ebx , 1
imul rax , rbx 
add ecx , eax 
mov eax , ecx 
push rax 
pop rax 
mov rbx , rax
pop rax 
mov rcx , rax
mov eax , [state_dictionary + ebx * 4] 
mov [tape + ecx * 4] , eax 

mov ecx , 0
mov eax , dword [rbp - 4]
mov ebx , 30
imul rax , rbx 
add ecx , eax 
mov eax , dword [rbp - 8]
mov ebx , 3
imul rax , rbx 
add ecx , eax 
mov eax , 1
mov ebx , 1
imul rax , rbx 
add ecx , eax 
mov eax , ecx 
push rax 
pop rax 
mov rcx , rax 
mov eax , [state_dictionary + ecx * 4]
push rax
push 0
pop rbx
pop rax
cmp eax , ebx 
je _BRANCH_33_IF
jmp _BRANCH_33_ELSE

_BRANCH_33_IF:
mov eax , dword [rbp - 12]
push rax
push 1
pop rbx 
pop rax 
sub rax , rbx 
push rax
pop rax
mov dword [rbp - 12] , eax 
jmp _BRANCH_33_EXIT
_BRANCH_33_ELSE:
mov eax , dword [rbp - 12]
push rax
push 1
pop rbx 
pop rax 
add rax , rbx 
push rax
pop rax
mov dword [rbp - 12] , eax 
jmp _BRANCH_33_EXIT

_BRANCH_33_EXIT:

mov eax , dword [rbp - 36]
mov dword [rbp - 4] , eax 


mov eax , dword [rbp - 32]
mov dword [rbp - 8] , eax 

mov eax , dword [rbp - 24]
push rax
push 1
pop rbx 
pop rax 
add rax , rbx 
push rax
pop rax
mov dword [rbp - 24] , eax 
mov rax , 1
call _sleep
jmp _BRANCH_32_EXIT

_BRANCH_32_EXIT:
jmp _TILL_0_ENTRY
_TILL_0_EXIT:
mov rax , 2
mov rdi , rax
call newLine
call  _green
mov rax , 1
mov rdi , 1
mov rsi , @Sref19
mov rdx , @Sref19_L 
syscall 

mov eax , dword [rbp - 12]
call _printINTEGER

call  _reset
call _newLine
call  _green
mov rax , 1
mov rdi , 1
mov rsi , @Sref20
mov rdx , @Sref20_L 
syscall 

call  _reset
call renderOutput

add rsp , 36
mov rsp , rbp
pop rbp

mov rax , 60
mov rdi , 0
syscall


initializeTape:
push rbp
mov rbp , rsp
sub rsp , 4
sub rsp , 4
mov dword [ rbp - 8 ] , 0
sub rsp , 4
mov dword [ rbp - 12 ] , 9
jmp _LOOP_0_ENTRY 
_LOOP_0_ENTRY: 
mov eax , dword [ rbp - 8 ]
mov dword [rbp - 4] , eax
mov eax , dword [ rbp - 8 ] 
mov ebx , dword [ rbp - 12 ] 
cmp eax , ebx
jg _LOOP_0_EXIT 
sub rsp , 4
mov eax , dword [rbp - 4]
push rax
push 3
pop rbx 
pop rax 
call _mod 
 push rcx
pop rax
mov dword [rbp - 16] , eax 
sub rsp , 4
mov eax , dword [rbp - 4]
push rax
push 5
pop rbx 
pop rax 
call _mod 
 push rcx
pop rax
mov dword [rbp - 20] , eax 

mov eax , dword [rbp - 16]
push rax
push 0
pop rbx
pop rax
cmp eax , ebx 
je _BRANCH_0_IF
jmp _BRANCH_0_ELSE

_BRANCH_0_IF:
mov ecx , 0
mov eax , dword [rbp - 4]
mov ebx , 1
imul rax , rbx 
add ecx , eax 
mov eax , ecx 
push rax 
pop rax 
mov rcx , rax
mov eax , 1
mov [tape + ecx * 4] , eax
jmp _BRANCH_0_EXIT
_BRANCH_0_ELSE:
jmp _BRANCH_0_EXIT

_BRANCH_0_EXIT:

mov eax , dword [rbp - 20]
push rax
push 0
pop rbx
pop rax
cmp eax , ebx 
je _BRANCH_1_IF
jmp _BRANCH_1_ELSE

_BRANCH_1_IF:
mov ecx , 0
mov eax , dword [rbp - 4]
mov ebx , 1
imul rax , rbx 
add ecx , eax 
mov eax , ecx 
push rax 
pop rax 
mov rcx , rax
mov eax , 1
mov [tape + ecx * 4] , eax
jmp _BRANCH_1_EXIT
_BRANCH_1_ELSE:
jmp _BRANCH_1_EXIT

_BRANCH_1_EXIT:
add dword [ rbp - 8 ] , 1
jmp _LOOP_0_ENTRY 
_LOOP_0_EXIT: 
sub rsp , 4
mov dword [ rbp - 24 ] , 0
sub rsp , 4
mov dword [ rbp - 28 ] , 9
jmp _LOOP_1_ENTRY 
_LOOP_1_ENTRY: 
mov eax , dword [ rbp - 24 ]
mov dword [rbp - 4] , eax
mov eax , dword [ rbp - 24 ] 
mov ebx , dword [ rbp - 28 ] 
cmp eax , ebx
jg _LOOP_1_EXIT 
mov ecx , 0
mov eax , dword [rbp - 4]
mov ebx , 1
imul rax , rbx 
add ecx , eax 
mov eax , ecx 
push rax 
mov ecx , 0
mov eax , dword [rbp - 4]
mov ebx , 1
imul rax , rbx 
add ecx , eax 
mov eax , ecx 
push rax 
pop rax 
mov rbx , rax
pop rax 
mov rcx , rax
mov eax , [tape + ebx * 4] 
mov [INPUT_tape + ecx * 4] , eax 
add dword [ rbp - 24 ] , 1
jmp _LOOP_1_ENTRY 
_LOOP_1_EXIT: 
add rsp , 0
mov rsp , rbp
pop rbp
ret 


initializeStateDictionary_0:
push rbp
mov rbp , rsp
sub rsp , 4
mov dword [rbp - 4] , 0
sub rsp , 4
mov dword [rbp - 8] , 1
sub rsp , 4
mov dword [rbp - 12] , 2
sub rsp , 4
sub rsp , 4
mov dword [ rbp - 20 ] , 0
sub rsp , 4
mov dword [ rbp - 24 ] , 9
jmp _LOOP_2_ENTRY 
_LOOP_2_ENTRY: 
mov eax , dword [ rbp - 20 ]
mov dword [rbp - 16] , eax
mov eax , dword [ rbp - 20 ] 
mov ebx , dword [ rbp - 24 ] 
cmp eax , ebx
jg _LOOP_2_EXIT 
sub rsp , 4
mov eax , dword [rbp - 16]
push rax
push 2
pop rbx 
pop rax 
call _mod 
 push rcx
pop rax
mov dword [rbp - 28] , eax 
sub rsp , 4
mov eax , dword [rbp - 16]
push rax
push 3
pop rbx 
pop rax 
call _mod 
 push rcx
pop rax
mov dword [rbp - 32] , eax 
sub rsp , 4
mov eax , dword [rbp - 16]
push rax
push 4
pop rbx 
pop rax 
call _mod 
 push rcx
pop rax
mov dword [rbp - 36] , eax 
sub rsp , 4
mov eax , dword [rbp - 16]
push rax
push 5
pop rbx 
pop rax 
call _mod 
 push rcx
pop rax
mov dword [rbp - 40] , eax 

mov eax , dword [rbp - 28]
push rax
push 0
pop rbx
pop rax
cmp eax , ebx 
je _BRANCH_2_IF
jmp _BRANCH_2_ELSE

_BRANCH_2_IF:
mov ecx , 0
mov eax , 0
mov ebx , 30
imul rax , rbx 
add ecx , eax 
mov eax , dword [rbp - 16]
mov ebx , 3
imul rax , rbx 
add ecx , eax 
mov eax , dword [rbp - 4]
mov ebx , 1
imul rax , rbx 
add ecx , eax 
mov eax , ecx 
push rax 
pop rax 
mov rcx , rax
mov eax , 1
mov [state_dictionary + ecx * 4] , eax
jmp _BRANCH_2_EXIT
_BRANCH_2_ELSE:
jmp _BRANCH_2_EXIT

_BRANCH_2_EXIT:

mov eax , dword [rbp - 32]
push rax
push 0
pop rbx
pop rax
cmp eax , ebx 
je _BRANCH_3_IF
jmp _BRANCH_3_ELSE

_BRANCH_3_IF:
mov ecx , 0
mov eax , 0
mov ebx , 30
imul rax , rbx 
add ecx , eax 
mov eax , dword [rbp - 16]
mov ebx , 3
imul rax , rbx 
add ecx , eax 
mov eax , dword [rbp - 8]
mov ebx , 1
imul rax , rbx 
add ecx , eax 
mov eax , ecx 
push rax 
pop rax 
mov rcx , rax
mov eax , 1
mov [state_dictionary + ecx * 4] , eax
jmp _BRANCH_3_EXIT
_BRANCH_3_ELSE:
jmp _BRANCH_3_EXIT

_BRANCH_3_EXIT:

mov eax , dword [rbp - 36]
push rax
push 0
pop rbx
pop rax
cmp eax , ebx 
je _BRANCH_4_IF
jmp _BRANCH_4_ELSE

_BRANCH_4_IF:
mov ecx , 0
mov eax , 0
mov ebx , 30
imul rax , rbx 
add ecx , eax 
mov eax , dword [rbp - 16]
mov ebx , 3
imul rax , rbx 
add ecx , eax 
mov eax , dword [rbp - 8]
mov ebx , 1
imul rax , rbx 
add ecx , eax 
mov eax , ecx 
push rax 
pop rax 
mov rcx , rax
mov eax , 1
mov [state_dictionary + ecx * 4] , eax
jmp _BRANCH_4_EXIT
_BRANCH_4_ELSE:
jmp _BRANCH_4_EXIT

_BRANCH_4_EXIT:

mov eax , dword [rbp - 40]
push rax
push 0
pop rbx
pop rax
cmp eax , ebx 
je _BRANCH_5_IF
jmp _BRANCH_5_ELSE

_BRANCH_5_IF:
mov ecx , 0
mov eax , 0
mov ebx , 30
imul rax , rbx 
add ecx , eax 
mov eax , dword [rbp - 16]
mov ebx , 3
imul rax , rbx 
add ecx , eax 
mov eax , dword [rbp - 12]
mov ebx , 1
imul rax , rbx 
add ecx , eax 
mov eax , ecx 
push rax 
mov eax , dword [rbp - 16]
push rax
push 1
pop rbx 
pop rax 
sub rax , rbx 
push rax
pop rax
mov rbx , rax 
pop rax 
mov rcx , rax
mov [state_dictionary + ecx * 4] , ebx
jmp _BRANCH_5_EXIT
_BRANCH_5_ELSE:
jmp _BRANCH_5_EXIT

_BRANCH_5_EXIT:

mov eax , dword [rbp - 28]
push rax
push 0
pop rbx
pop rax
cmp eax , ebx 
je _BRANCH_6_IF
jmp _BRANCH_6_ELSE

_BRANCH_6_IF:
mov ecx , 0
mov eax , 0
mov ebx , 30
imul rax , rbx 
add ecx , eax 
mov eax , dword [rbp - 16]
mov ebx , 3
imul rax , rbx 
add ecx , eax 
mov eax , dword [rbp - 12]
mov ebx , 1
imul rax , rbx 
add ecx , eax 
mov eax , ecx 
push rax 
mov eax , dword [rbp - 16]
push rax
push 1
pop rbx 
pop rax 
add rax , rbx 
push rax
pop rax
mov rbx , rax 
pop rax 
mov rcx , rax
mov [state_dictionary + ecx * 4] , ebx
jmp _BRANCH_6_EXIT
_BRANCH_6_ELSE:
jmp _BRANCH_6_EXIT

_BRANCH_6_EXIT:

mov eax , dword [rbp - 32]
push rax
push 0
pop rbx
pop rax
cmp eax , ebx 
je _BRANCH_7_IF
jmp _BRANCH_7_ELSE

_BRANCH_7_IF:
mov ecx , 0
mov eax , 0
mov ebx , 30
imul rax , rbx 
add ecx , eax 
mov eax , dword [rbp - 16]
mov ebx , 3
imul rax , rbx 
add ecx , eax 
mov eax , dword [rbp - 12]
mov ebx , 1
imul rax , rbx 
add ecx , eax 
mov eax , ecx 
push rax 
mov eax , dword [rbp - 16]
push rax
push 1
pop rbx 
pop rax 
sub rax , rbx 
push rax
pop rax
mov rbx , rax 
pop rax 
mov rcx , rax
mov [state_dictionary + ecx * 4] , ebx
jmp _BRANCH_7_EXIT
_BRANCH_7_ELSE:
jmp _BRANCH_7_EXIT

_BRANCH_7_EXIT:

mov eax , dword [rbp - 16]
push rax
push 0
pop rbx
pop rax
cmp eax , ebx 
je _BRANCH_8_IF
jmp _BRANCH_8_ELSE

_BRANCH_8_IF:
mov ecx , 0
mov eax , 0
mov ebx , 30
imul rax , rbx 
add ecx , eax 
mov eax , dword [rbp - 16]
mov ebx , 3
imul rax , rbx 
add ecx , eax 
mov eax , dword [rbp - 12]
mov ebx , 1
imul rax , rbx 
add ecx , eax 
mov eax , ecx 
push rax 
pop rax 
mov rcx , rax
mov eax , 7
mov [state_dictionary + ecx * 4] , eax
jmp _BRANCH_8_EXIT
_BRANCH_8_ELSE:
jmp _BRANCH_8_EXIT

_BRANCH_8_EXIT:

mov eax , dword [rbp - 16]
push rax
push 1
pop rbx
pop rax
cmp eax , ebx 
je _BRANCH_9_IF
jmp _BRANCH_9_ELSE

_BRANCH_9_IF:
mov ecx , 0
mov eax , 0
mov ebx , 30
imul rax , rbx 
add ecx , eax 
mov eax , dword [rbp - 16]
mov ebx , 3
imul rax , rbx 
add ecx , eax 
mov eax , dword [rbp - 12]
mov ebx , 1
imul rax , rbx 
add ecx , eax 
mov eax , ecx 
push rax 
pop rax 
mov rcx , rax
mov eax , 0
mov [state_dictionary + ecx * 4] , eax
jmp _BRANCH_9_EXIT
_BRANCH_9_ELSE:
jmp _BRANCH_9_EXIT

_BRANCH_9_EXIT:

mov eax , dword [rbp - 16]
push rax
push 7
pop rbx
pop rax
cmp eax , ebx 
je _BRANCH_10_IF
jmp _BRANCH_10_ELSE

_BRANCH_10_IF:
mov ecx , 0
mov eax , 0
mov ebx , 30
imul rax , rbx 
add ecx , eax 
mov eax , dword [rbp - 16]
mov ebx , 3
imul rax , rbx 
add ecx , eax 
mov eax , dword [rbp - 12]
mov ebx , 1
imul rax , rbx 
add ecx , eax 
mov eax , ecx 
push rax 
pop rax 
mov rcx , rax
mov eax , 1
mov [state_dictionary + ecx * 4] , eax
jmp _BRANCH_10_EXIT
_BRANCH_10_ELSE:
jmp _BRANCH_10_EXIT

_BRANCH_10_EXIT:

mov eax , dword [rbp - 16]
push rax
push 4
pop rbx
pop rax
cmp eax , ebx 
je _BRANCH_11_IF
jmp _BRANCH_11_ELSE

_BRANCH_11_IF:
mov ecx , 0
mov eax , 0
mov ebx , 30
imul rax , rbx 
add ecx , eax 
mov eax , dword [rbp - 16]
mov ebx , 3
imul rax , rbx 
add ecx , eax 
mov eax , dword [rbp - 12]
mov ebx , 1
imul rax , rbx 
add ecx , eax 
mov eax , ecx 
push rax 
pop rax 
mov rcx , rax
mov eax , 6
mov [state_dictionary + ecx * 4] , eax
jmp _BRANCH_11_EXIT
_BRANCH_11_ELSE:
jmp _BRANCH_11_EXIT

_BRANCH_11_EXIT:

mov eax , dword [rbp - 16]
push rax
push 4
pop rbx
pop rax
cmp eax , ebx 
je _BRANCH_12_IF
jmp _BRANCH_12_ELSE

_BRANCH_12_IF:
mov ecx , 0
mov eax , 0
mov ebx , 30
imul rax , rbx 
add ecx , eax 
mov eax , dword [rbp - 16]
mov ebx , 3
imul rax , rbx 
add ecx , eax 
mov eax , dword [rbp - 8]
mov ebx , 1
imul rax , rbx 
add ecx , eax 
mov eax , ecx 
push rax 
pop rax 
mov rcx , rax
mov eax , 0
mov [state_dictionary + ecx * 4] , eax
jmp _BRANCH_12_EXIT
_BRANCH_12_ELSE:
jmp _BRANCH_12_EXIT

_BRANCH_12_EXIT:
add dword [ rbp - 20 ] , 1
jmp _LOOP_2_ENTRY 
_LOOP_2_EXIT: 
add rsp , 12
mov rsp , rbp
pop rbp
ret 


initializeStateDictionary_1:
push rbp
mov rbp , rsp
sub rsp , 4
mov dword [rbp - 4] , 0
sub rsp , 4
mov dword [rbp - 8] , 1
sub rsp , 4
mov dword [rbp - 12] , 2
sub rsp , 4
sub rsp , 4
mov dword [ rbp - 20 ] , 0
sub rsp , 4
mov dword [ rbp - 24 ] , 9
jmp _LOOP_3_ENTRY 
_LOOP_3_ENTRY: 
mov eax , dword [ rbp - 20 ]
mov dword [rbp - 16] , eax
mov eax , dword [ rbp - 20 ] 
mov ebx , dword [ rbp - 24 ] 
cmp eax , ebx
jg _LOOP_3_EXIT 
sub rsp , 4
mov eax , dword [rbp - 16]
push rax
push 2
pop rbx 
pop rax 
call _mod 
 push rcx
pop rax
mov dword [rbp - 28] , eax 
sub rsp , 4
mov eax , dword [rbp - 16]
push rax
push 3
pop rbx 
pop rax 
call _mod 
 push rcx
pop rax
mov dword [rbp - 32] , eax 
sub rsp , 4
mov eax , dword [rbp - 16]
push rax
push 4
pop rbx 
pop rax 
call _mod 
 push rcx
pop rax
mov dword [rbp - 36] , eax 
sub rsp , 4
mov eax , dword [rbp - 16]
push rax
push 5
pop rbx 
pop rax 
call _mod 
 push rcx
pop rax
mov dword [rbp - 40] , eax 

mov eax , dword [rbp - 32]
push rax
push 0
pop rbx
pop rax
cmp eax , ebx 
je _BRANCH_13_IF
jmp _BRANCH_13_ELSE

_BRANCH_13_IF:
mov ecx , 0
mov eax , 1
mov ebx , 30
imul rax , rbx 
add ecx , eax 
mov eax , dword [rbp - 16]
mov ebx , 3
imul rax , rbx 
add ecx , eax 
mov eax , dword [rbp - 4]
mov ebx , 1
imul rax , rbx 
add ecx , eax 
mov eax , ecx 
push rax 
pop rax 
mov rcx , rax
mov eax , 1
mov [state_dictionary + ecx * 4] , eax
jmp _BRANCH_13_EXIT
_BRANCH_13_ELSE:
jmp _BRANCH_13_EXIT

_BRANCH_13_EXIT:

mov eax , dword [rbp - 28]
push rax
push 0
pop rbx
pop rax
cmp eax , ebx 
je _BRANCH_14_IF
jmp _BRANCH_14_ELSE

_BRANCH_14_IF:
mov ecx , 0
mov eax , 1
mov ebx , 30
imul rax , rbx 
add ecx , eax 
mov eax , dword [rbp - 16]
mov ebx , 3
imul rax , rbx 
add ecx , eax 
mov eax , dword [rbp - 8]
mov ebx , 1
imul rax , rbx 
add ecx , eax 
mov eax , ecx 
push rax 
pop rax 
mov rcx , rax
mov eax , 1
mov [state_dictionary + ecx * 4] , eax
jmp _BRANCH_14_EXIT
_BRANCH_14_ELSE:
jmp _BRANCH_14_EXIT

_BRANCH_14_EXIT:

mov eax , dword [rbp - 40]
push rax
push 0
pop rbx
pop rax
cmp eax , ebx 
je _BRANCH_15_IF
jmp _BRANCH_15_ELSE

_BRANCH_15_IF:
mov ecx , 0
mov eax , 1
mov ebx , 30
imul rax , rbx 
add ecx , eax 
mov eax , dword [rbp - 16]
mov ebx , 3
imul rax , rbx 
add ecx , eax 
mov eax , dword [rbp - 8]
mov ebx , 1
imul rax , rbx 
add ecx , eax 
mov eax , ecx 
push rax 
pop rax 
mov rcx , rax
mov eax , 1
mov [state_dictionary + ecx * 4] , eax
jmp _BRANCH_15_EXIT
_BRANCH_15_ELSE:
jmp _BRANCH_15_EXIT

_BRANCH_15_EXIT:

mov eax , dword [rbp - 28]
push rax
push 0
pop rbx
pop rax
cmp eax , ebx 
je _BRANCH_16_IF
jmp _BRANCH_16_ELSE

_BRANCH_16_IF:
mov ecx , 0
mov eax , 1
mov ebx , 30
imul rax , rbx 
add ecx , eax 
mov eax , dword [rbp - 16]
mov ebx , 3
imul rax , rbx 
add ecx , eax 
mov eax , dword [rbp - 12]
mov ebx , 1
imul rax , rbx 
add ecx , eax 
mov eax , ecx 
push rax 
mov eax , dword [rbp - 16]
push rax
push 1
pop rbx 
pop rax 
sub rax , rbx 
push rax
pop rax
mov rbx , rax 
pop rax 
mov rcx , rax
mov [state_dictionary + ecx * 4] , ebx
jmp _BRANCH_16_EXIT
_BRANCH_16_ELSE:
jmp _BRANCH_16_EXIT

_BRANCH_16_EXIT:

mov eax , dword [rbp - 32]
push rax
push 0
pop rbx
pop rax
cmp eax , ebx 
je _BRANCH_17_IF
jmp _BRANCH_17_ELSE

_BRANCH_17_IF:
mov ecx , 0
mov eax , 1
mov ebx , 30
imul rax , rbx 
add ecx , eax 
mov eax , dword [rbp - 16]
mov ebx , 3
imul rax , rbx 
add ecx , eax 
mov eax , dword [rbp - 12]
mov ebx , 1
imul rax , rbx 
add ecx , eax 
mov eax , ecx 
push rax 
mov eax , dword [rbp - 16]
push rax
push 1
pop rbx 
pop rax 
add rax , rbx 
push rax
pop rax
mov rbx , rax 
pop rax 
mov rcx , rax
mov [state_dictionary + ecx * 4] , ebx
jmp _BRANCH_17_EXIT
_BRANCH_17_ELSE:
jmp _BRANCH_17_EXIT

_BRANCH_17_EXIT:

mov eax , dword [rbp - 40]
push rax
push 0
pop rbx
pop rax
cmp eax , ebx 
je _BRANCH_18_IF
jmp _BRANCH_18_ELSE

_BRANCH_18_IF:
mov ecx , 0
mov eax , 1
mov ebx , 30
imul rax , rbx 
add ecx , eax 
mov eax , dword [rbp - 16]
mov ebx , 3
imul rax , rbx 
add ecx , eax 
mov eax , dword [rbp - 12]
mov ebx , 1
imul rax , rbx 
add ecx , eax 
mov eax , ecx 
push rax 
mov eax , dword [rbp - 16]
push rax
push 1
pop rbx 
pop rax 
sub rax , rbx 
push rax
pop rax
mov rbx , rax 
pop rax 
mov rcx , rax
mov [state_dictionary + ecx * 4] , ebx
jmp _BRANCH_18_EXIT
_BRANCH_18_ELSE:
jmp _BRANCH_18_EXIT

_BRANCH_18_EXIT:

mov eax , dword [rbp - 16]
push rax
push 7
pop rbx
pop rax
cmp eax , ebx 
je _BRANCH_19_IF
jmp _BRANCH_19_ELSE

_BRANCH_19_IF:
mov ecx , 0
mov eax , 1
mov ebx , 30
imul rax , rbx 
add ecx , eax 
mov eax , dword [rbp - 16]
mov ebx , 3
imul rax , rbx 
add ecx , eax 
mov eax , dword [rbp - 12]
mov ebx , 1
imul rax , rbx 
add ecx , eax 
mov eax , ecx 
push rax 
pop rax 
mov rcx , rax
mov eax , 0
mov [state_dictionary + ecx * 4] , eax
jmp _BRANCH_19_EXIT
_BRANCH_19_ELSE:
jmp _BRANCH_19_EXIT

_BRANCH_19_EXIT:

mov eax , dword [rbp - 16]
push rax
push 0
pop rbx
pop rax
cmp eax , ebx 
je _BRANCH_20_IF
jmp _BRANCH_20_ELSE

_BRANCH_20_IF:
mov ecx , 0
mov eax , 1
mov ebx , 30
imul rax , rbx 
add ecx , eax 
mov eax , dword [rbp - 16]
mov ebx , 3
imul rax , rbx 
add ecx , eax 
mov eax , dword [rbp - 12]
mov ebx , 1
imul rax , rbx 
add ecx , eax 
mov eax , ecx 
push rax 
pop rax 
mov rcx , rax
mov eax , 9
mov [state_dictionary + ecx * 4] , eax
jmp _BRANCH_20_EXIT
_BRANCH_20_ELSE:
jmp _BRANCH_20_EXIT

_BRANCH_20_EXIT:

mov eax , dword [rbp - 16]
push rax
push 1
pop rbx
pop rax
cmp eax , ebx 
je _BRANCH_21_IF
jmp _BRANCH_21_ELSE

_BRANCH_21_IF:
mov ecx , 0
mov eax , 1
mov ebx , 30
imul rax , rbx 
add ecx , eax 
mov eax , dword [rbp - 16]
mov ebx , 3
imul rax , rbx 
add ecx , eax 
mov eax , dword [rbp - 12]
mov ebx , 1
imul rax , rbx 
add ecx , eax 
mov eax , ecx 
push rax 
pop rax 
mov rcx , rax
mov eax , 7
mov [state_dictionary + ecx * 4] , eax
jmp _BRANCH_21_EXIT
_BRANCH_21_ELSE:
jmp _BRANCH_21_EXIT

_BRANCH_21_EXIT:

mov eax , dword [rbp - 16]
push rax
push 6
pop rbx
pop rax
cmp eax , ebx 
je _BRANCH_22_IF
jmp _BRANCH_22_ELSE

_BRANCH_22_IF:
mov ecx , 0
mov eax , 1
mov ebx , 30
imul rax , rbx 
add ecx , eax 
mov eax , dword [rbp - 16]
mov ebx , 3
imul rax , rbx 
add ecx , eax 
mov eax , dword [rbp - 12]
mov ebx , 1
imul rax , rbx 
add ecx , eax 
mov eax , ecx 
push rax 
pop rax 
mov rcx , rax
mov eax , 2
mov [state_dictionary + ecx * 4] , eax
jmp _BRANCH_22_EXIT
_BRANCH_22_ELSE:
jmp _BRANCH_22_EXIT

_BRANCH_22_EXIT:

mov eax , dword [rbp - 16]
push rax
push 9
pop rbx
pop rax
cmp eax , ebx 
je _BRANCH_23_IF
jmp _BRANCH_23_ELSE

_BRANCH_23_IF:
mov ecx , 0
mov eax , 1
mov ebx , 30
imul rax , rbx 
add ecx , eax 
mov eax , dword [rbp - 16]
mov ebx , 3
imul rax , rbx 
add ecx , eax 
mov eax , dword [rbp - 12]
mov ebx , 1
imul rax , rbx 
add ecx , eax 
mov eax , ecx 
push rax 
pop rax 
mov rcx , rax
mov eax , 9
mov [state_dictionary + ecx * 4] , eax
jmp _BRANCH_23_EXIT
_BRANCH_23_ELSE:
jmp _BRANCH_23_EXIT

_BRANCH_23_EXIT:
add dword [ rbp - 20 ] , 1
jmp _LOOP_3_ENTRY 
_LOOP_3_EXIT: 
add rsp , 12
mov rsp , rbp
pop rbp
ret 


initializeStateDictionary:
push rbp
mov rbp , rsp
call initializeStateDictionary_0
call initializeStateDictionary_1
add rsp , -28
mov rsp , rbp
pop rbp
ret 


renderInput:
push rbp
mov rbp , rsp
sub rsp , 4
sub rsp , 4
mov dword [ rbp - 8 ] , 0
sub rsp , 4
mov dword [ rbp - 12 ] , 9
jmp _LOOP_4_ENTRY 
_LOOP_4_ENTRY: 
mov eax , dword [ rbp - 8 ]
mov dword [rbp - 4] , eax
mov eax , dword [ rbp - 8 ] 
mov ebx , dword [ rbp - 12 ] 
cmp eax , ebx
jg _LOOP_4_EXIT 
call  _green
mov ecx , 0
mov eax , dword [rbp - 4]
mov ebx , 1
imul rax , rbx 
add ecx , eax 
mov eax , ecx 
push rax 
pop rax 
mov rcx , rax
mov eax , [INPUT_tape + ecx * 4]
call _printINTEGER
call  _reset
add dword [ rbp - 8 ] , 1
jmp _LOOP_4_ENTRY 
_LOOP_4_EXIT: 
add rsp , -16
mov rsp , rbp
pop rbp
ret 


renderOutput:
push rbp
mov rbp , rsp
sub rsp , 4
sub rsp , 4
mov dword [ rbp - 8 ] , 0
sub rsp , 4
mov dword [ rbp - 12 ] , 9
jmp _LOOP_5_ENTRY 
_LOOP_5_ENTRY: 
mov eax , dword [ rbp - 8 ]
mov dword [rbp - 4] , eax
mov eax , dword [ rbp - 8 ] 
mov ebx , dword [ rbp - 12 ] 
cmp eax , ebx
jg _LOOP_5_EXIT 
call  _green
mov ecx , 0
mov eax , dword [rbp - 4]
mov ebx , 1
imul rax , rbx 
add ecx , eax 
mov eax , ecx 
push rax 
pop rax 
mov rcx , rax
mov eax , [tape + ecx * 4]
call _printINTEGER
call  _reset
add dword [ rbp - 8 ] , 1
jmp _LOOP_5_ENTRY 
_LOOP_5_EXIT: 
add rsp , -16
mov rsp , rbp
pop rbp
ret 


leftPadding:
push rbp
mov rbp , rsp
mov rax , rdi
sub rsp , 4
mov dword [rbp - 4] , eax 
sub rsp , 4
mov dword [ rbp - 8 ] , 1
_DISPLAY_LOOP_0:
mov eax , dword [ rbp - 8 ] 
mov ebx , dword [ rbp - 4 ] 
cmp eax , ebx
jg _DISPLAY_LOOP_0_EXIT 
mov rax , 1
mov rdi , 1
mov rsi , @Sref0
mov rdx , @Sref0_L 
syscall 

add dword [ rbp - 8 ] , 1
jmp _DISPLAY_LOOP_0
_DISPLAY_LOOP_0_EXIT:

add rsp , -20
mov rsp , rbp
pop rbp
ret 


newLine:
push rbp
mov rbp , rsp
mov rax , rdi
sub rsp , 4
mov dword [rbp - 4] , eax 
sub rsp , 4
mov dword [ rbp - 8 ] , 1
jmp _LOOP_6_ENTRY 
_LOOP_6_ENTRY: 
mov eax , dword [ rbp - 8 ] 
mov ebx , dword [ rbp - 4 ] 
cmp eax , ebx
jg _LOOP_6_EXIT 
mov rax , 1
mov rdi , 1
mov rsi , @Sref1
mov rdx , @Sref1_L 
syscall 

call _newLine
add dword [ rbp - 8 ] , 1
jmp _LOOP_6_ENTRY 
_LOOP_6_EXIT: 
add rsp , -20
mov rsp , rbp
pop rbp
ret 


renderHead:
push rbp
mov rbp , rsp
mov rax , rdi
sub rsp , 4
mov dword [rbp - 4] , eax 
sub rsp , 4
mov eax , dword [rbp - 4]
push rax
push 4
pop rbx 
pop rax 
imul rax , rbx 
push rax
push 30
pop rbx 
pop rax 
add rax , rbx 
push rax
pop rax
mov dword [rbp - 8] , eax 
mov rax , 1
mov rdi , rax
call newLine
mov eax , dword [rbp - 8]
mov rdi , rax
call leftPadding
call  _purple
mov rax , 1
mov rdi , 1
mov rsi , @Sref2
mov rdx , @Sref2_L 
syscall 

call  _reset
call _newLine
mov eax , dword [rbp - 8]
mov rdi , rax
call leftPadding
call  _purple
mov rax , 1
mov rdi , 1
mov rsi , @Sref3
mov rdx , @Sref3_L 
syscall 

call  _reset
call _newLine
add rsp , -20
mov rsp , rbp
pop rbp
ret 


renderStateDictionary:
push rbp
mov rbp , rsp
mov rax , rdi
sub rsp , 4
mov dword [rbp - 4] , eax 
mov rax , rsi
sub rsp , 4
mov dword [rbp - 8] , eax 
sub rsp , 4
mov dword [rbp - 12] , 10
mov rax , 1
mov rdi , rax
call newLine
mov rax , 23
mov rdi , rax
call leftPadding
mov rax , 1
mov rdi , 1
mov rsi , @Sref4
mov rdx , @Sref4_L 
syscall 

mov rax , 18
mov rdi , rax
call leftPadding
mov rax , 1
mov rdi , 1
mov rsi , @Sref4
mov rdx , @Sref4_L 
syscall 

call _newLine
mov rax , 10
mov rdi , rax
call leftPadding
call  _red
mov rax , 1
mov rdi , 1
mov rsi , @Sref5
mov rdx , @Sref5_L 
syscall 

call  _reset
call  _green
mov rax , 1
mov rdi , 1
mov rsi , @Sref6
mov rdx , @Sref6_L 
syscall 

call  _reset
mov rax , 5
mov rdi , rax
call leftPadding
call  _red
mov rax , 1
mov rdi , 1
mov rsi , @Sref7
mov rdx , @Sref7_L 
syscall 

call  _reset
call  _green
mov rax , 1
mov rdi , 1
mov rsi , @Sref6
mov rdx , @Sref6_L 
syscall 

call  _reset
mov rax , 1
mov rdi , rax
call newLine
mov rax , 23
mov rdi , rax
call leftPadding
mov rax , 1
mov rdi , 1
mov rsi , @Sref4
mov rdx , @Sref4_L 
syscall 

mov rax , 18
mov rdi , rax
call leftPadding
mov rax , 1
mov rdi , 1
mov rsi , @Sref4
mov rdx , @Sref4_L 
syscall 

call _newLine
sub rsp , 4
sub rsp , 4
mov dword [ rbp - 20 ] , 0
sub rsp , 4
mov dword [ rbp - 24 ] , 9
jmp _LOOP_7_ENTRY 
_LOOP_7_ENTRY: 
mov eax , dword [ rbp - 20 ]
mov dword [rbp - 16] , eax
mov eax , dword [ rbp - 20 ] 
mov ebx , dword [ rbp - 24 ] 
cmp eax , ebx
jg _LOOP_7_EXIT 
mov eax , dword [rbp - 12]
mov rdi , rax
call leftPadding
sub rsp , 4
sub rsp , 4
mov dword [ rbp - 32 ] , 0
sub rsp , 4
mov dword [ rbp - 36 ] , 1
jmp _LOOP_8_ENTRY 
_LOOP_8_ENTRY: 
mov eax , dword [ rbp - 32 ]
mov dword [rbp - 28] , eax
mov eax , dword [ rbp - 32 ] 
mov ebx , dword [ rbp - 36 ] 
cmp eax , ebx
jg _LOOP_8_EXIT 

mov eax , dword [rbp - 16]
push rax
mov eax , dword [rbp - 4]
push rax
pop rbx
pop rax
cmp eax , ebx 
je _BRANCH_24_IF
jmp _BRANCH_24_ELSE

_BRANCH_24_IF:

mov eax , dword [rbp - 28]
push rax
mov eax , dword [rbp - 8]
push rax
pop rbx
pop rax
cmp eax , ebx 
je _BRANCH_25_IF
jmp _BRANCH_25_ELSE

_BRANCH_25_IF:
call  _red
mov rax , 1
mov rdi , 1
mov rsi , @Sref8
mov rdx , @Sref8_L 
syscall 

call  _reset
jmp _BRANCH_25_EXIT
_BRANCH_25_ELSE:
mov rax , 13
mov rdi , rax
call leftPadding
jmp _BRANCH_25_EXIT

_BRANCH_25_EXIT:
jmp _BRANCH_24_EXIT
_BRANCH_24_ELSE:
mov rax , 13
mov rdi , rax
call leftPadding
jmp _BRANCH_24_EXIT

_BRANCH_24_EXIT:

mov eax , dword [rbp - 16]
push rax
mov eax , dword [rbp - 4]
push rax
pop rbx
pop rax
cmp eax , ebx 
je _BRANCH_26_IF
jmp _BRANCH_26_ELSE

_BRANCH_26_IF:

mov eax , dword [rbp - 28]
push rax
mov eax , dword [rbp - 8]
push rax
pop rbx
pop rax
cmp eax , ebx 
je _BRANCH_27_IF
jmp _BRANCH_27_ELSE

_BRANCH_27_IF:
call  _background_red
mov rax , 1
mov rdi , 1
mov rsi , @Sref9
mov rdx , @Sref9_L 
syscall 

call  _reset
jmp _BRANCH_27_EXIT
_BRANCH_27_ELSE:
mov rax , 1
mov rdi , 1
mov rsi , @Sref9
mov rdx , @Sref9_L 
syscall 

jmp _BRANCH_27_EXIT

_BRANCH_27_EXIT:
jmp _BRANCH_26_EXIT
_BRANCH_26_ELSE:
mov rax , 1
mov rdi , 1
mov rsi , @Sref9
mov rdx , @Sref9_L 
syscall 

jmp _BRANCH_26_EXIT

_BRANCH_26_EXIT:
sub rsp , 4
sub rsp , 4
mov dword [ rbp - 44 ] , 0
sub rsp , 4
mov dword [ rbp - 48 ] , 2
jmp _LOOP_9_ENTRY 
_LOOP_9_ENTRY: 
mov eax , dword [ rbp - 44 ]
mov dword [rbp - 40] , eax
mov eax , dword [ rbp - 44 ] 
mov ebx , dword [ rbp - 48 ] 
cmp eax , ebx
jg _LOOP_9_EXIT 

mov eax , dword [rbp - 16]
push rax
mov eax , dword [rbp - 4]
push rax
pop rbx
pop rax
cmp eax , ebx 
je _BRANCH_28_IF
jmp _BRANCH_28_ELSE

_BRANCH_28_IF:

mov eax , dword [rbp - 28]
push rax
mov eax , dword [rbp - 8]
push rax
pop rbx
pop rax
cmp eax , ebx 
je _BRANCH_29_IF
jmp _BRANCH_29_ELSE

_BRANCH_29_IF:
call  _background_red
mov rax , 1
mov rdi , 1
mov rsi , @Sref10
mov rdx , @Sref10_L 
syscall 

mov ecx , 0
mov eax , dword [rbp - 28]
mov ebx , 30
imul rax , rbx 
add ecx , eax 
mov eax , dword [rbp - 16]
mov ebx , 3
imul rax , rbx 
add ecx , eax 
mov eax , dword [rbp - 40]
mov ebx , 1
imul rax , rbx 
add ecx , eax 
mov eax , ecx 
push rax 
pop rax 
mov rcx , rax
mov eax , [state_dictionary + ecx * 4]
call _printINTEGER
mov rax , 1
mov rdi , 1
mov rsi , @Sref11
mov rdx , @Sref11_L 
syscall 

call  _reset
jmp _BRANCH_29_EXIT
_BRANCH_29_ELSE:
mov rax , 1
mov rdi , 1
mov rsi , @Sref10
mov rdx , @Sref10_L 
syscall 

mov ecx , 0
mov eax , dword [rbp - 28]
mov ebx , 30
imul rax , rbx 
add ecx , eax 
mov eax , dword [rbp - 16]
mov ebx , 3
imul rax , rbx 
add ecx , eax 
mov eax , dword [rbp - 40]
mov ebx , 1
imul rax , rbx 
add ecx , eax 
mov eax , ecx 
push rax 
pop rax 
mov rcx , rax
mov eax , [state_dictionary + ecx * 4]
call _printINTEGER
mov rax , 1
mov rdi , 1
mov rsi , @Sref11
mov rdx , @Sref11_L 
syscall 

jmp _BRANCH_29_EXIT

_BRANCH_29_EXIT:
jmp _BRANCH_28_EXIT
_BRANCH_28_ELSE:
mov rax , 1
mov rdi , 1
mov rsi , @Sref10
mov rdx , @Sref10_L 
syscall 

mov ecx , 0
mov eax , dword [rbp - 28]
mov ebx , 30
imul rax , rbx 
add ecx , eax 
mov eax , dword [rbp - 16]
mov ebx , 3
imul rax , rbx 
add ecx , eax 
mov eax , dword [rbp - 40]
mov ebx , 1
imul rax , rbx 
add ecx , eax 
mov eax , ecx 
push rax 
pop rax 
mov rcx , rax
mov eax , [state_dictionary + ecx * 4]
call _printINTEGER
mov rax , 1
mov rdi , 1
mov rsi , @Sref11
mov rdx , @Sref11_L 
syscall 

jmp _BRANCH_28_EXIT

_BRANCH_28_EXIT:
add dword [ rbp - 44 ] , 1
jmp _LOOP_9_ENTRY 
_LOOP_9_EXIT: 
mov rax , 5
mov rdi , rax
call leftPadding
add dword [ rbp - 32 ] , 1
jmp _LOOP_8_ENTRY 
_LOOP_8_EXIT: 
mov rax , 1
mov rdi , rax
call newLine
add dword [ rbp - 20 ] , 1
jmp _LOOP_7_ENTRY 
_LOOP_7_EXIT: 
mov rax , 23
mov rdi , rax
call leftPadding
mov rax , 1
mov rdi , 1
mov rsi , @Sref4
mov rdx , @Sref4_L 
syscall 

mov rax , 18
mov rdi , rax
call leftPadding
mov rax , 1
mov rdi , 1
mov rsi , @Sref4
mov rdx , @Sref4_L 
syscall 

call _newLine
add rsp , 20
mov rsp , rbp
pop rbp
ret 


renderTape:
push rbp
mov rbp , rsp
sub rsp , 4
mov dword [rbp - 4] , 30
mov eax , dword [rbp - 4]
mov rdi , rax
call leftPadding
sub rsp , 4
mov dword [ rbp - 8 ] , 0
sub rsp , 4
mov dword [ rbp - 12 ] , 40
jmp _LOOP_10_ENTRY 
_LOOP_10_ENTRY: 
mov eax , dword [ rbp - 8 ] 
mov ebx , dword [ rbp - 12 ] 
cmp eax , ebx
jg _LOOP_10_EXIT 
mov rax , 1
mov rdi , 1
mov rsi , @Sref12
mov rdx , @Sref12_L 
syscall 

add dword [ rbp - 8 ] , 1
jmp _LOOP_10_ENTRY 
_LOOP_10_EXIT: 
mov rax , 1
mov rdi , rax
call newLine
mov eax , dword [rbp - 4]
mov rdi , rax
call leftPadding
mov rax , 1
mov rdi , 1
mov rsi , @Sref9
mov rdx , @Sref9_L 
syscall 

sub rsp , 4
sub rsp , 4
mov dword [ rbp - 20 ] , 0
sub rsp , 4
mov dword [ rbp - 24 ] , 9
jmp _LOOP_11_ENTRY 
_LOOP_11_ENTRY: 
mov eax , dword [ rbp - 20 ]
mov dword [rbp - 16] , eax
mov eax , dword [ rbp - 20 ] 
mov ebx , dword [ rbp - 24 ] 
cmp eax , ebx
jg _LOOP_11_EXIT 

mov ecx , 0
mov eax , dword [rbp - 16]
mov ebx , 1
imul rax , rbx 
add ecx , eax 
mov eax , ecx 
push rax 
pop rax 
mov rcx , rax 
mov eax , [tape + ecx * 4]
push rax
push 0
pop rbx
pop rax
cmp eax , ebx 
je _BRANCH_30_IF
jmp _BRANCH_30_ELSE

_BRANCH_30_IF:
call  _red
mov rax , 1
mov rdi , 1
mov rsi , @Sref0
mov rdx , @Sref0_L 
syscall 

mov ecx , 0
mov eax , dword [rbp - 16]
mov ebx , 1
imul rax , rbx 
add ecx , eax 
mov eax , ecx 
push rax 
pop rax 
mov rcx , rax
mov eax , [tape + ecx * 4]
call _printINTEGER
call  _reset
mov rax , 1
mov rdi , 1
mov rsi , @Sref13
mov rdx , @Sref13_L 
syscall 

jmp _BRANCH_30_EXIT
_BRANCH_30_ELSE:
call  _cyan
mov rax , 1
mov rdi , 1
mov rsi , @Sref0
mov rdx , @Sref0_L 
syscall 

mov ecx , 0
mov eax , dword [rbp - 16]
mov ebx , 1
imul rax , rbx 
add ecx , eax 
mov eax , ecx 
push rax 
pop rax 
mov rcx , rax
mov eax , [tape + ecx * 4]
call _printINTEGER
call  _reset
mov rax , 1
mov rdi , 1
mov rsi , @Sref13
mov rdx , @Sref13_L 
syscall 

jmp _BRANCH_30_EXIT

_BRANCH_30_EXIT:
add dword [ rbp - 20 ] , 1
jmp _LOOP_11_ENTRY 
_LOOP_11_EXIT: 
mov rax , 1
mov rdi , rax
call newLine
mov eax , dword [rbp - 4]
mov rdi , rax
call leftPadding
sub rsp , 4
mov dword [ rbp - 28 ] , 0
sub rsp , 4
mov dword [ rbp - 32 ] , 40
jmp _LOOP_12_ENTRY 
_LOOP_12_ENTRY: 
mov eax , dword [ rbp - 28 ] 
mov ebx , dword [ rbp - 32 ] 
cmp eax , ebx
jg _LOOP_12_EXIT 
mov rax , 1
mov rdi , 1
mov rsi , @Sref12
mov rdx , @Sref12_L 
syscall 

add dword [ rbp - 28 ] , 1
jmp _LOOP_12_ENTRY 
_LOOP_12_EXIT: 
add rsp , 4
mov rsp , rbp
pop rbp
ret 


render:
push rbp
mov rbp , rsp
mov rax , rdi
sub rsp , 4
mov dword [rbp - 4] , eax 
mov rax , rsi
sub rsp , 4
mov dword [rbp - 8] , eax 
mov rax , rdx
sub rsp , 4
mov dword [rbp - 12] , eax 
mov rax , rcx
sub rsp , 4
mov dword [rbp - 16] , eax 
call  _cyan
mov rax , 1
mov rdi , 1
mov rsi , @Sref14
mov rdx , @Sref14_L 
syscall 

call  _reset
call _newLine
call  _green
mov rax , 1
mov rdi , 1
mov rsi , @Sref15
mov rdx , @Sref15_L 
syscall 

call  _reset
call renderInput
mov rax , 1
mov rdi , rax
call newLine
call  _green
mov rax , 1
mov rdi , 1
mov rsi , @Sref16
mov rdx , @Sref16_L 
syscall 

mov eax , dword [rbp - 16]
call _printINTEGER

call  _reset
call _newLine
call  _yellow
mov rax , 1
mov rdi , 1
mov rsi , @Sref17
mov rdx , @Sref17_L 
syscall 

mov eax , dword [rbp - 4]
call _printINTEGER

call  _reset
call _newLine
call  _yellow
mov rax , 1
mov rdi , 1
mov rsi , @Sref18
mov rdx , @Sref18_L 
syscall 

call  _reset
call _newLine
mov rax , 1
mov rdi , rax
call newLine
call renderTape
mov eax , dword [rbp - 4]
mov rdi , rax
call renderHead
mov eax , dword [rbp - 8]
mov rdi , rax
mov eax , dword [rbp - 12]
mov rsi , rax
call renderStateDictionary
add rsp , -12
mov rsp , rbp
pop rbp
ret 

