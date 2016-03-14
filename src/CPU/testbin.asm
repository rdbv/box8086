cpu 8086
org 0x7c00

mov cl, 0xbb

mov bx, 0xffff
mov ds, bx

mov bx, 0x0000
lop:
    mov byte [ds:bx], cl
    inc bx
jmp lop



times 510 - ($-$$) db 0
dw 0xAA55
