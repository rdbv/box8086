cpu 8086
org 0x7c00

mov al, 127
mov bl, 1
add al, bl



times 510 - ($-$$) db 0
dw 0xAA55
