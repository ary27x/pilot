; ANSI escape sequences for color output in terminal

section .data

; Reset color to default
reset:  db 27, 91, 48, 109   ; ESC[0m
reset_L: equ $-reset

; Basic colors
red:    db 27, 91, 51, 49, 109 ; ESC[31m
red_L:  equ $-red

green:  db 27, 91, 51, 50, 109 ; ESC[32m
green_L: equ $-green

yellow: db 27, 91, 51, 51, 109 ; ESC[33m
yellow_L: equ $-yellow

blue:   db 27, 91, 51, 52, 109 ; ESC[34m
blue_L:  equ $-blue

purple: db 27, 91, 51, 53, 109 ; ESC[35m
purple_L: equ $-purple

cyan:   db 27, 91, 51, 54, 109 ; ESC[36m
cyan_L:  equ $-cyan

white:  db 27, 91, 51, 55, 109 ; ESC[37m
white_L: equ $-white

; Enhanced colors
; Bright Red
brightRed:    db 27, 91, 49, 59, 51, 49, 109 ; ESC[1;31m
brightRed_L:  equ $-brightRed

; Bright Green
brightGreen:  db 27, 91, 49, 59, 51, 50, 109 ; ESC[1;32m
brightGreen_L: equ $-brightGreen

; Bright Yellow
brightYellow: db 27, 91, 49, 59, 51, 51, 109 ; ESC[1;33m
brightYellow_L: equ $-brightYellow

; Bright Blue
brightBlue:   db 27, 91, 49, 59, 51, 52, 109 ; ESC[1;34m
brightBlue_L:  equ $-brightBlue

; Bright Purple
brightPurple: db 27, 91, 49, 59, 51, 53, 109 ; ESC[1;35m
brightPurple_L: equ $-brightPurple

; Bright Cyan
brightCyan:   db 27, 91, 49, 59, 51, 54, 109 ; ESC[1;36m
brightCyan_L:  equ $-brightCyan

; Bright White
brightWhite:  db 27, 91, 49, 59, 51, 55, 109 ; ESC[1;37m
brightWhite_L: equ $-brightWhite
