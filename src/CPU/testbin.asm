cpu 8086
org 0x7c00

mov cx, 13

p:
    mov al, cl
    dec cx
    cmp cx, 0
    jne p


times 510 - ($-$$) db 0
dw 0xAA55
