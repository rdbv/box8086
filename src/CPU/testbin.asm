cpu 8086
org 0x7c00


mov ax, [bp+di]
mov ax, [bp+si]
mov ax, [bp]

mov al, 0xff
mov bl, 0x1
add al, bl
hlt

times 510 - ($-$$) db 0
dw 0xAA55
