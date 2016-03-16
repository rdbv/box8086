cpu 8086
org 0x7c00

add [bx+si-0x36], al

mov al, 0xff
mov bl, 0x1
add [bx], al

add al, bl

add al, al
add al, al
add al, al



times 510 - ($-$$) db 0
dw 0xAA55
