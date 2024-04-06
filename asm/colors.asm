; ANSI COLOR CODE TO ASCII BYTE STREAM TRANSLATIONS

section .data:

__reset:
db 27,91,0,109
__reset_L : equ $-__reset

__black:
db 27,91,49,59,51,48,109
__black_L : equ $-__black

__red:
db 27,91,49,59,51,49,109
__red_L : equ $-__red

__green:
db 27,91,49,59,51,50,109
__green_L : equ $-__green

__yellow:
db 27,91,49,59,51,51,109
__yellow_L : equ $-__yellow

__blue:
db 27,91,49,59,51,52,109
__blue_L : equ $-__blue

__purple:
db 27,91,49,59,51,53,109
__purple_L : equ $-__purple
  
__cyan:
db 27,91,49,59,51,54,109
__cyan_L : equ $-__cyan

__white:
db 27,91,49,59,51,55,109
__white_L : equ $-__white

; ANSI CODE FOR BACKGROUND COLORS

__background_black:
db 27,91,49,59,52,48,109
__background_black_L : equ $-__background_black

__background_red:
db 27,91,49,59,52,49,109
__background_red_L : equ $-__background_red

__background_green:
db 27,91,49,59,52,50,109
__background_green_L : equ $-__background_green

__background_yellow:
db 27,91,49,59,52,51,109
__background_yellow_L : equ $-__background_yellow

__background_blue:
db 27,91,49,59,52,52,109
__background_blue_L : equ $-__background_blue

__background_purple:
db 27,91,49,59,52,53,109
__background_purple_L : equ $-__background_purple

__background_cyan:
db 27,91,49,59,52,54,109
__background_cyan_L : equ $-__background_cyan

__background_white:
db 27,91,49,59,52,55,109
__background_white_L : equ $-__background_white
