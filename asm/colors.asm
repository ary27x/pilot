; ANSI COLOR CODE TO ASCII BYTE STREAM TRANSLATIONS

section .data:

reset:
db 27,91,0,109
reset_L : equ $-reset

red:
db 27,91,49,59,51,49,109
red_L : equ $-red

green:
db 27,91,49,59,51,50,109
green_L : equ $-green

yellow:
db 27,91,49,59,51,51,109
yellow_L : equ $-yellow

blue:
db 27,91,49,59,51,52,109
blue_L : equ $-blue

purple:
db 27,91,49,59,51,53,109
purple_L : equ $-purple
  
cyan:
db 27,91,49,59,51,54,109
cyan_L : equ $-cyan

white:
db 27,91,49,59,51,55,109
white_L : equ $-white


