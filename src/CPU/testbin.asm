cpu 8086
org 0x7c00

mov al, 0x7f
mov bl, 0xcc
add al, bl

mov al, 0x3d
mov bl, -32
add al, bl

mov ax, 0xfade
mov bx, 0xface
add ax, bx

mov ax, 0xce01
mov bx, 0x01ce
add bx, ax


times 510 - ($-$$) db 0
dw 0xAA55
