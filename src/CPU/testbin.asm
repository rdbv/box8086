cpu 8086
org 0x7c00

add [0xface], dl

add [bx], cl
add [bx+di], cl

mov ax, 0x7f
mov bl, 0x1

add al, al
add al, al
add al, bl

times 510 - ($-$$) db 0
dw 0xAA55
